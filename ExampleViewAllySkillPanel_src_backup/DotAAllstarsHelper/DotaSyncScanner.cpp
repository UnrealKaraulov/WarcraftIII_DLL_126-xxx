#include "Main.h"


int ScanId = 0;
int MaxScanId = 0;

struct ScanStructUnit
{
	int unitid;
	int unitowner;
	float unitx;
	float unity;
	float unithpreg;
	float unitmpreg;
};

struct ScanStructItem
{
	int itemid;
	float itemx;
	float itemy;
};


struct ScanStructHead
{
	int UnitsScanCount;
	int ItemsScanCount;
};


vector<ScanStructUnit> ScanUnitsForDump;
vector<ScanStructItem> ScanItemsForDump;

vector<ScanStructUnit> ScanUnitsForDump_fromdump;
vector<ScanStructItem> ScanItemsForDump_fromdump;


void FillScanList( )
{
	ScanUnitsForDump.clear( );
	ScanItemsForDump.clear( );


	int * itemsarray = 0;
	int * ItemsCount = GetItemCountAndItemArray( &itemsarray );
	if ( ItemsCount > 0 && * ItemsCount > 0 && itemsarray )
	{
		for ( int i = 0; i < *ItemsCount; i++ )
		{
			if ( itemsarray[ i ] && IsNotBadItem( itemsarray[ i ], TRUE ) )
			{
				ScanStructItem tmpstr = ScanStructItem( );
				tmpstr.itemid = *( int* )( itemsarray[ i ] + 0x30 );
				GetItemLocation2D( itemsarray[ i ], &tmpstr.itemx, &tmpstr.itemy );

				if ( tmpstr.itemx != 0.0f &&
					tmpstr.itemy != 0.0f )
					ScanItemsForDump.push_back( tmpstr );
			}
		}
	}

	int * unitsarray = 0;
	int * UnitsCount = GetUnitCountAndUnitArray( &unitsarray );
	if ( UnitsCount > 0 && * UnitsCount > 0 && unitsarray )
	{
		for ( int i = 0; i < *UnitsCount; i++ )
		{
			if ( unitsarray[ i ] && IsNotBadUnit( unitsarray[ i ] ) )
			{


				ScanStructUnit tmpstr = ScanStructUnit( );
				tmpstr.unitid = *( int* )( unitsarray[ i ] + 0x30 );

				tmpstr.unitx = GetUnitX_real( unitsarray[ i ] );
				tmpstr.unity = GetUnitY_real( unitsarray[ i ] );
				//GetUnitLocation2D( unitsarray[ i ], &tmpstr.unitx, &tmpstr.unity );
				tmpstr.unithpreg = GetUnitHPregen( unitsarray[ i ] );
				tmpstr.unitmpreg = GetUnitMPregen( unitsarray[ i ] );
				tmpstr.unitowner = GetUnitOwnerSlot( unitsarray[ i ] );

				int unitdataoffset = *( int * )( unitsarray[ i ] + 0x28 );
				if ( unitdataoffset )
				{
					if ( tmpstr.unitx != 0.0f &&
						tmpstr.unity != 0.0f && tmpstr.unitx != 256.0f &&
						tmpstr.unity != 256.0f && tmpstr.unitowner >= 0 && tmpstr.unitowner <= 15 )
						ScanUnitsForDump.push_back( tmpstr );
				}
			}
		}
	}

}

ScanStructUnit lastbadunit = ScanStructUnit( );
ScanStructItem lastbaditem = ScanStructItem( );
char lastresultstr[ 512 ];

string GetStrID( int id )
{
	char buff[ 7 ];
	char buff2[ 4 ];
	std::memcpy( buff2, &id, 4 );
	buff[ 0 ] = '\'';
	buff[ 1 ] = buff2[ 3 ];
	buff[ 2 ] = buff2[ 2 ];
	buff[ 3 ] = buff2[ 1 ];
	buff[ 4 ] = buff2[ 0 ];
	buff[ 5 ] = '\'';
	buff[ 6 ] = '\0';
	bool needreturnid = true;
	for ( int i = 1; i < 5; i++ )
	{
		if ( !isalnum( buff[ i ] ) )
		{
			needreturnid = false;
			break;
		}
	}
	if ( needreturnid )
		return string( buff );
	return "BAD:" + to_string( id );
}

void CompareResultsAndPrintInfo( )
{
	int foundunits = 0;
	int founditems = 0;

	lastbadunit = ScanStructUnit( );
	lastbaditem = ScanStructItem( );

	int foundbadunits = 0;
	int foundbaditems = 0;


	for ( ScanStructUnit s : ScanUnitsForDump )
	{
		bool found = true;
		for ( ScanStructUnit s2 : ScanUnitsForDump_fromdump )
		{
			found = false;
			if ( memcmp( &s, &s2, sizeof( ScanStructUnit ) ) == 0 )
			{
				foundunits++;
				found = true;
				break;
			}
		}
		if ( !found )
		{
			lastbadunit = s;
			foundbadunits++;
		}
	}

	for ( ScanStructItem s : ScanItemsForDump )
	{
		bool found = true;
		for ( ScanStructItem s2 : ScanItemsForDump_fromdump )
		{
			found = false;
			if ( memcmp( &s, &s2, sizeof( ScanStructItem ) ) == 0 )
			{
				founditems++;
				found = true;
				break;
			}
		}
		if ( !found )
		{
			lastbaditem = s;
			foundbaditems++;
		}
	}

	bool nodesync = true;

	if ( foundbadunits > 0 )
	{
		sprintf_s( lastresultstr, "Found bad unit [%s], X/Y: %f / %f.", GetStrID( lastbadunit.unitid ).c_str( ), lastbadunit.unitx, lastbadunit.unity );
		nodesync = false;
		PrintText( lastresultstr );
		MessageBoxA( 0, lastresultstr, lastresultstr, 0 );
	}
	else
	{
		sprintf_s( lastresultstr, "Unit compare success.[%i-%i] No desync.", ScanUnitsForDump.size( ), ScanUnitsForDump_fromdump.size( ) );
		PrintText( lastresultstr );
	}

	if ( foundbaditems > 0 )
	{
		sprintf_s( lastresultstr, "Found bad item [%s], X/Y: %f / %f.", GetStrID( lastbaditem.itemid ).c_str( ), lastbaditem.itemx, lastbaditem.itemy );

		nodesync = false;
		PrintText( lastresultstr );
		MessageBoxA( 0, lastresultstr, lastresultstr, 0 );
	}
	else
	{
		sprintf_s( lastresultstr, "Item compare success.[%i-%i] No desync.", ScanItemsForDump.size( ), ScanItemsForDump_fromdump.size( ) );
		PrintText( lastresultstr );
	}

	/*if ( nodesync )
	{
		if ( ScanUnitsForDump.size( ) != ScanUnitsForDump_fromdump.size( ) )
		{
			MessageBoxA( 0, "Detect wrong unit number.", "Detect wrong unit number.", 0 );
		}
		if ( ScanItemsForDump.size( ) != ScanItemsForDump_fromdump.size( ) )
		{
			MessageBoxA( 0, "Detect wrong item number.", "Detect wrong item number.", 0 );
		}

	}*/

}



int __stdcall ReplayDesyncScan( BOOL scan_or_check )
{
	unsigned char * replayscandata = NULL;
	long size = 0;
	
	try
	{

		if ( scan_or_check == TRUE )
		{
			FILE * f;
			fopen_s( &f, "ReplayScan.bin", "rb" );
			if ( f )
			{
				fseek( f, 0, SEEK_END );
				size = ftell( f );
				fseek( f, 0, SEEK_SET );

				if ( size > 0 )
				{
					replayscandata = new unsigned char[ size ];
					fread( replayscandata, size, 1, f );
				}

				fclose( f );
				f = NULL;
			}

			fopen_s( &f, "ReplayScan.bin", "wb" );

			if ( f )
			{
				if ( ScanId != 0 )
					fwrite( replayscandata, size, 1, f );
				FillScanList( );
				ScanStructHead tmpcountstr = ScanStructHead( );
				tmpcountstr.UnitsScanCount = ScanUnitsForDump.size( );
				tmpcountstr.ItemsScanCount = ScanItemsForDump.size( );

				fwrite( &tmpcountstr, sizeof( ScanStructHead ), 1, f );

				for ( ScanStructUnit s : ScanUnitsForDump )
				{
					fwrite( &s, sizeof( ScanStructUnit ), 1, f );
				}

				for ( ScanStructItem s : ScanItemsForDump )
				{
					fwrite( &s, sizeof( ScanStructItem ), 1, f );
				}

				fclose( f );
				f = NULL;
			}
			else
			{
				PrintText( "Error! No access to ReplayScan.bin file." );
			}

		}
		else
		{

			ScanUnitsForDump_fromdump.clear( );
			ScanItemsForDump_fromdump.clear( );

			FILE * f;
			fopen_s( &f, "ReplayScan.bin", "rb" );
			if ( f )
			{
				fseek( f, 0, SEEK_END );
				size = ftell( f );
				fseek( f, 0, SEEK_SET );
				int currentscanid = 0;
				bool needscan = false;
				while ( size > 0 )
				{
					ScanStructHead tmpcountstr = ScanStructHead( );
					fread( &tmpcountstr, sizeof( ScanStructHead ), 1, f );
					size -= sizeof( ScanStructHead );
					for ( int i = 0; i < tmpcountstr.UnitsScanCount; i++ )
					{
						ScanStructUnit tmpunitstr = ScanStructUnit( );
						fread( &tmpunitstr, sizeof( ScanStructUnit ), 1, f );
						if ( ScanId == currentscanid )
						{
							ScanUnitsForDump_fromdump.push_back( tmpunitstr );
							needscan = true;
						}
						size -= sizeof( ScanStructUnit );
					}
					for ( int i = 0; i < tmpcountstr.ItemsScanCount; i++ )
					{
						ScanStructItem tmpitemstr = ScanStructItem( );
						fread( &tmpitemstr, sizeof( ScanStructItem ), 1, f );
						if ( ScanId == currentscanid )
						{
							ScanItemsForDump_fromdump.push_back( tmpitemstr );
							needscan = true;
						}
						size -= sizeof( ScanStructItem );
					}
					currentscanid++;
				}

				if ( needscan )
				{
					FillScanList( );
					CompareResultsAndPrintInfo( );
				}

				fclose( f );
			}
		}

	}
	catch ( ... )
	{
		PrintText( "FATAL ERROR FATAL ERROR!! WARNING FATAL ERROR WHILE REPLAY DESYNC SCAN!" );

	}

	ScanId++;
	return 0;
}