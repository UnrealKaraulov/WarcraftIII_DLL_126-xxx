#include "Main.h"

unsigned char FixBounds( int i )
{
	if ( i > 0xFF )
		return 0xFF;
	else if ( i < 0x00 )
		return 0x00;
	return ( unsigned char )i;
}

unsigned char FixBounds( float i )
{
	if ( i >( double )0xFF )
		return 0xFF;
	else if ( i < ( double )0x00 )
		return 0x00;
	return ( unsigned char )i;
}

unsigned char FixBounds( double i )
{
	if ( i >( double )0xFF )
		return 0xFF;
	else if ( i < ( double )0x00 )
		return 0x00;
	return ( unsigned char )i;
}

CQuantizer::CQuantizer( unsigned int nMaxColors, unsigned int nColorBits )
{
	m_nColorBits = nColorBits < 8 ? nColorBits : 8;
	m_pTree = 0;
	m_nLeafCount = 0;
	m_lastIndex = 0;
	m_needsAlpha = FALSE;
	for ( int i = 0; i <= ( int )m_nColorBits; i++ )
		m_pReducibleNodes[ i ] = 0;
	m_nMaxColors = m_nOutputMaxColors = nMaxColors;
	if ( m_nMaxColors < 16 )m_nMaxColors = 16;
}

CQuantizer::~CQuantizer( )
{
	if ( m_pTree )
		DeleteTree( &m_pTree );
}

BOOL CQuantizer::ProcessImage( unsigned char* image, unsigned long size, unsigned char bytespp, unsigned char alpha )
{
	for ( unsigned long i = 0; i < size; i++ )
	{
		BGRAPix *pix = ( BGRAPix* )( image + i * bytespp );
		AddColor( &m_pTree, pix->R, pix->G, pix->B, 0, m_nColorBits, 0, &m_nLeafCount, m_pReducibleNodes );
		//if ( pix->A != alpha )
			m_needsAlpha = TRUE;
		if ( m_nLeafCount > m_nMaxColors )
			ReduceTree( m_nColorBits, &m_nLeafCount, m_pReducibleNodes );
	}
	while ( m_nLeafCount > m_nMaxColors )
		ReduceTree( m_nColorBits, &m_nLeafCount, m_pReducibleNodes );
	m_needsAlpha = m_needsAlpha && bytespp == 4;
	return TRUE;
}

void CQuantizer::FloydSteinbergDither( unsigned char* image, long width, long height, unsigned char bytespp, unsigned char* target, BGRAPix* pal )
{
	for ( long y = 0; y < height; y++ )
	{
		if ( y % 2 == 1 )
		{
			for ( long x = 0; x < width; x++ )
			{
				int i = width * ( height - y - 1 ) + x;
				int j = ( width * y + x ) * bytespp;
				unsigned char k = GetNearestIndexFast( ( BGRAPix* )( image + j ), pal );
				int diff[ 3 ];
				target[ i ] = k;
				diff[ 0 ] = image[ j ] - pal[ k ].B;
				diff[ 1 ] = image[ j + 1 ] - pal[ k ].G;
				diff[ 2 ] = image[ j + 2 ] - pal[ k ].R;

				if ( y < height - 1 )
				{
					for ( int l = 0; l < 3; l++ )
						image[ j + ( width * bytespp ) + l ] = FixBounds( image[ j + ( width * bytespp ) + l ] + ( diff[ l ] * 5 ) / 16 );
					if ( x > 0 )
						for ( int l = 0; l < 3; l++ )
							image[ j + ( ( width - 1 ) * bytespp ) + l ] = FixBounds( image[ j + ( ( width - 1 ) * bytespp ) + l ] + ( diff[ l ] * 3 ) / 16 );
					if ( x < width - 1 )
						for ( int l = 0; l < 3; l++ )
							image[ j + ( ( width + 1 ) * bytespp ) + l ] = FixBounds( image[ j + ( ( width + 1 ) * bytespp ) + l ] + ( diff[ l ] * 1 ) / 16 );
				}
				if ( x < width - 1 )
					for ( int l = 0; l < 3; l++ )
						image[ j + bytespp + l ] = FixBounds( image[ j + bytespp + l ] + ( diff[ l ] * 7 ) / 16 );

			}
		}
		else
		{
			for ( long x = width - 1; x >= 0; x-- )
			{
				int i = width * ( height - y - 1 ) + x;
				int j = ( width * y + x ) * bytespp;
				unsigned char k = GetNearestIndexFast( ( BGRAPix* )( image + j ), pal );
				int diff[ 3 ];
				target[ i ] = k;
				diff[ 0 ] = image[ j ] - pal[ k ].B;
				diff[ 1 ] = image[ j + 1 ] - pal[ k ].G;
				diff[ 2 ] = image[ j + 2 ] - pal[ k ].R;

				if ( y < height - 1 )
				{
					for ( int l = 0; l < 3; l++ )
						image[ j + ( width * bytespp ) + l ] = FixBounds( image[ j + ( width * bytespp ) + l ] + ( diff[ l ] * 5 ) / 16 );
					if ( x > 0 )
						for ( int l = 0; l < 3; l++ )
							image[ j + ( ( width - 1 ) * bytespp ) + l ] = FixBounds( image[ j + ( ( width - 1 ) * bytespp ) + l ] + ( diff[ l ] * 1 ) / 16 );
					if ( x < width - 1 )
						for ( int l = 0; l < 3; l++ )
							image[ j + ( ( width + 1 ) * bytespp ) + l ] = FixBounds( image[ j + ( ( width + 1 ) * bytespp ) + l ] + ( diff[ l ] * 3 ) / 16 );
				}
				if ( x > 0 )
					for ( int l = 0; l < 3; l++ )
						image[ j - bytespp + l ] = FixBounds( image[ j - bytespp + l ] + ( diff[ l ] * 7 ) / 16 );

			}
		}
	}
}

void CQuantizer::AddColor( Node** ppNode, unsigned char r, unsigned char g, unsigned char b, unsigned char a,
	unsigned int nColorBits, int nLevel, unsigned int*	pLeafCount, Node** pReducibleNodes )
{
	static unsigned char mask[ 8 ] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
	if ( !( *ppNode ) )
		*ppNode = ( Node* )CreateNode( nLevel, nColorBits, pLeafCount, pReducibleNodes );
	if ( ( *ppNode )->bIsLeaf )
	{
		( *ppNode )->nPixelCount++;
		( *ppNode )->nRedSum += r;
		( *ppNode )->nGreenSum += g;
		( *ppNode )->nBlueSum += b;
		( *ppNode )->nAlphaSum += a;
	}
	else
	{
		int	shift = 7 - nLevel;
		int	nIndex = ( ( ( r & mask[ nLevel ] ) >> shift ) << 2 ) | ( ( ( g & mask[ nLevel ] ) >> shift ) << 1 ) | ( ( b & mask[ nLevel ] ) >> shift );
		AddColor( &( ( *ppNode )->pChild[ nIndex ] ), r, g, b, a, nColorBits, nLevel + 1, pLeafCount, pReducibleNodes );
	}
}

void* CQuantizer::CreateNode( int nLevel, unsigned int nColorBits, unsigned int* pLeafCount, Node** pReducibleNodes )
{
	Node* pNode = ( Node* )calloc( 1, sizeof( Node ) );
	if ( !pNode ) return 0;
	pNode->bIsLeaf = ( ( unsigned int )nLevel == nColorBits ) ? TRUE : FALSE;
	pNode->nIndex = 0;
	if ( pNode->bIsLeaf ) ( *pLeafCount )++;
	else
	{
		pNode->pNext = pReducibleNodes[ nLevel ];
		pReducibleNodes[ nLevel ] = pNode;
	}
	return pNode;
}

void CQuantizer::ReduceTree( unsigned int nColorBits, unsigned int* pLeafCount, Node** pReducibleNodes )
{
	int i = ( int )nColorBits - 1;
	for ( ; ( i > 0 ) && ( !pReducibleNodes[ i ] ); i-- );
	if ( !pReducibleNodes[ i ] ) return;
	Node* pNode = pReducibleNodes[ i ];
	pReducibleNodes[ i ] = pNode->pNext;

	unsigned int nRedSum = 0;
	unsigned int nGreenSum = 0;
	unsigned int nBlueSum = 0;
	unsigned int nAlphaSum = 0;
	unsigned int nChildren = 0;

	for ( i = 0; i < 8; i++ )
	{
		if ( pNode->pChild[ i ] )
		{
			nRedSum += pNode->pChild[ i ]->nRedSum;
			nGreenSum += pNode->pChild[ i ]->nGreenSum;
			nBlueSum += pNode->pChild[ i ]->nBlueSum;
			nAlphaSum += pNode->pChild[ i ]->nAlphaSum;
			pNode->nPixelCount += pNode->pChild[ i ]->nPixelCount;
			free( pNode->pChild[ i ] );
			pNode->pChild[ i ] = 0;
			nChildren++;
		}
	}

	pNode->bIsLeaf = TRUE;
	pNode->nRedSum = nRedSum;
	pNode->nGreenSum = nGreenSum;
	pNode->nBlueSum = nBlueSum;
	pNode->nAlphaSum = nAlphaSum;
	*pLeafCount -= nChildren - 1;
}

void CQuantizer::DeleteTree( Node** ppNode )
{
	for ( int i = 0; i < 8; i++ )
	{
		if ( ( *ppNode )->pChild[ i ] ) DeleteTree( &( ( *ppNode )->pChild[ i ] ) );
	}
	free( *ppNode );
	*ppNode = 0;
}

void CQuantizer::GetPaletteColors( Node* pTree, BGRAPix* prgb, unsigned int* pIndex, unsigned int* pSum )
{
	if ( pTree )
	{
		if ( pTree->bIsLeaf )
		{
			prgb[ *pIndex ].R = ( unsigned char )( ( pTree->nRedSum ) / ( pTree->nPixelCount ) );
			prgb[ *pIndex ].G = ( unsigned char )( ( pTree->nGreenSum ) / ( pTree->nPixelCount ) );
			prgb[ *pIndex ].B = ( unsigned char )( ( pTree->nBlueSum ) / ( pTree->nPixelCount ) );
			prgb[ *pIndex ].A = ( unsigned char )( ( pTree->nAlphaSum ) / ( pTree->nPixelCount ) );
			pTree->nIndex = *pIndex;
			if ( pSum )
				pSum[ *pIndex ] = pTree->nPixelCount;
			( *pIndex )++;
		}
		else
		{
			for ( int i = 0; i < 8; i++ )
			{
				if ( pTree->pChild[ i ] )
					GetPaletteColors( pTree->pChild[ i ], prgb, pIndex, pSum );
			}
		}
	}
}

unsigned int CQuantizer::GetLeafCount( Node* pTree )
{
	if ( pTree )
	{
		if ( pTree->bIsLeaf )
		{
			return 1;
		}
		else
		{
			unsigned int sum = 0;
			for ( int i = 0; i < 8; i++ )
			{
				if ( pTree->pChild[ i ] )
					sum += GetLeafCount( pTree->pChild[ i ] );
			}
			return sum;
		}
	}
	return 0;
}

unsigned char CQuantizer::GetNextBestLeaf( Node** pTree, unsigned int nLevel, BGRAPix* c, BGRAPix* pal )
{
	if ( ( *pTree )->bIsLeaf )
	{
		return FixBounds( ( int )( *pTree )->nIndex );
	}
	else
	{
		static unsigned char mask[ 8 ] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
		int	shift = 7 - ( int )nLevel;
		int	nIndex = ( ( ( c->R & mask[ nLevel ] ) >> shift ) << 2 ) | ( ( ( c->G & mask[ nLevel ] ) >> shift ) << 1 ) | ( ( c->B & mask[ nLevel ] ) >> shift );
		if ( ( *pTree )->pChild[ nIndex ] )
			return GetNextBestLeaf( &( ( *pTree )->pChild[ nIndex ] ), nLevel + 1, c, pal );
		else
			return GetNearestIndex( c, pal );
	}
}

unsigned int CQuantizer::GetColorCount( )
{
	return m_nLeafCount;
}

void CQuantizer::SetColorTable( BGRAPix* prgb )
{
	unsigned int nIndex = 0;
	if ( m_nOutputMaxColors<16 && m_nLeafCount>m_nOutputMaxColors )
	{
		unsigned int nSum[ 16 ];
		BGRAPix tmppal[ 16 ];
		GetPaletteColors( m_pTree, tmppal, &nIndex, nSum );
		unsigned int j, k, nr, ng, nb, na, ns, a, b;
		for ( j = 0; j < m_nOutputMaxColors; j++ )
		{
			a = ( j*m_nLeafCount ) / m_nOutputMaxColors;
			b = ( ( j + 1 )*m_nLeafCount ) / m_nOutputMaxColors;
			nr = ng = nb = na = ns = 0;
			for ( k = a; k < b; k++ )
			{
				nr += tmppal[ k ].R * nSum[ k ];
				ng += tmppal[ k ].G * nSum[ k ];
				nb += tmppal[ k ].B * nSum[ k ];
				na += tmppal[ k ].A * nSum[ k ];
				ns += nSum[ k ];
			}
			prgb[ j ].R = FixBounds( ( int )( nr / ns ) );
			prgb[ j ].G = FixBounds( ( int )( ng / ns ) );
			prgb[ j ].B = FixBounds( ( int )( nb / ns ) );
			prgb[ j ].A = FixBounds( ( int )( na / ns ) );
		}
	}
	else
	{
		GetPaletteColors( m_pTree, prgb, &nIndex, 0 );
	}
}

BOOL CQuantizer::ColorsAreEqual( BGRAPix* a, BGRAPix* b )
{
	return ( a->B == b->B && a->G == b->G && a->R == b->R && a->A == b->A );
}

unsigned char CQuantizer::GetNearestIndex( BGRAPix* c, BGRAPix* pal )
{
	if ( !c || !pal ) return 0;
	if ( ColorsAreEqual( c, &pal[ m_lastIndex ] ) )
		return m_lastIndex;
	unsigned long cur = 0;
	for ( unsigned long i = 0, k = 0, distance = 2147483647; i < m_nLeafCount; i++ )
	{
		k = ( unsigned long )( ( pal[ i ].B - c->B )*( pal[ i ].B - c->B ) + ( pal[ i ].G - c->G )*( pal[ i ].G - c->G ) + ( pal[ i ].R - c->R )*( pal[ i ].R - c->R ) );
		if ( k == 0 )
		{
			m_lastIndex = ( unsigned char )i;
			return ( unsigned char )i;
		}
		if ( k < distance )
		{
			distance = k;
			cur = i;
		}
	}
	m_lastIndex = ( unsigned char )cur;
	return m_lastIndex;
}

unsigned char CQuantizer::GetNearestIndexFast( BGRAPix* c, BGRAPix* pal )
{
	if ( m_nOutputMaxColors<16 && m_nLeafCount>m_nOutputMaxColors )
		return GetNearestIndex( c, pal );
	if ( !c || !pal ) return 0;
	if ( ColorsAreEqual( c, &pal[ m_lastIndex ] ) )
		return m_lastIndex;
	m_lastIndex = ( unsigned char )GetNextBestLeaf( &m_pTree, 0, c, pal );
	return m_lastIndex;
}

BOOL CQuantizer::NeedsAlphaChannel( )
{
	return m_needsAlpha;
}

