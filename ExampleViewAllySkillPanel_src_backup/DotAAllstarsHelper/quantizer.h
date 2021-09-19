#pragma once

unsigned char FixBounds( int i );
unsigned char FixBounds( float i );
unsigned char FixBounds( double i );


union BGRAPix
{
	struct
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
		unsigned char A;
	};
	unsigned long bgra;

	BGRAPix( unsigned char B, unsigned char G, unsigned char R, unsigned char A )
	{
		this->bgra = ( unsigned long ) ( ( A << 24 ) | ( R << 16 ) | ( G << 8 ) | B );
	}
	BGRAPix( )
	{
		this->bgra = 0;
	}
};


class CQuantizer
{
	typedef struct tagNode
	{
		BOOL bIsLeaf;
		unsigned int nPixelCount;
		unsigned int nRedSum;
		unsigned int nGreenSum;
		unsigned int nBlueSum;
		unsigned int nAlphaSum;
		unsigned int nIndex;
		struct tagNode* pChild[ 8 ];
		struct tagNode* pNext;
	} Node;

protected:
	Node* m_pTree;
	unsigned int m_nLeafCount;
	Node* m_pReducibleNodes[ 8 ];
	unsigned int m_nMaxColors;
	unsigned int m_nOutputMaxColors;
	unsigned int m_nColorBits;
	unsigned char m_lastIndex;
	BOOL m_needsAlpha;

public:
	CQuantizer( unsigned int nMaxColors, unsigned int nColorBits );
	virtual ~CQuantizer( );
	BOOL ProcessImage( unsigned char* image, unsigned long size, unsigned char bytespp, unsigned char alpha );
	void FloydSteinbergDither( unsigned char* image, long width, long height, unsigned char bytespp, unsigned char* target, BGRAPix* pal );
	BOOL NeedsAlphaChannel( );
	unsigned int GetColorCount( );
	void SetColorTable( BGRAPix* prgb );
	unsigned char GetNearestIndex( BGRAPix* c, BGRAPix* pal );
	unsigned char GetNearestIndexFast( BGRAPix* c, BGRAPix* pal );

protected:
	unsigned int GetLeafCount( Node* pTree );
	void AddColor( Node** ppNode, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned int nColorBits, int nLevel, unsigned int* pLeafCount, Node** pReducibleNodes );
	void* CreateNode( int nLevel, unsigned int nColorBits, unsigned int* pLeafCount, Node** pReducibleNodes );
	void ReduceTree( unsigned int nColorBits, unsigned int* pLeafCount, Node** pReducibleNodes );
	void DeleteTree( Node** ppNode );
	void GetPaletteColors( Node* pTree, BGRAPix* prgb, unsigned int* pIndex, unsigned int* pSum );
	unsigned char GetNextBestLeaf( Node** pTree, unsigned int nLevel, BGRAPix* c, BGRAPix* pal );
	BOOL ColorsAreEqual( BGRAPix* a, BGRAPix* b );
};