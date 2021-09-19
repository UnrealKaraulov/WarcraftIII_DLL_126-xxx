#include <iostream>
#include <chrono>

auto Timer1 = std::chrono::high_resolution_clock::now( );

int __stdcall StartTimer( int id )
{
	Timer1 = std::chrono::high_resolution_clock::now( );
	return 0;
}


unsigned int __stdcall StopTimer( int id )
{
	std::chrono::microseconds retval = std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::high_resolution_clock::now( ) - Timer1 );
	return ( unsigned int ) retval.count( );
}