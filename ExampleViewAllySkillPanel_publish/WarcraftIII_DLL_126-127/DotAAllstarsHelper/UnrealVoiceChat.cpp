#pragma once

#if 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif



/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  ( 8000.0 )
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (0.25)
//#define DITHER_FLAG     (paDitherOff) 
/** Set to 1 if you want to capture the recording to a file. */
#define WRITE_TO_FILE   (1)




#include "Main.h"



#include <stdio.h>
#include <stdlib.h>
#include <concurrent_vector.h>
#define safevector concurrency::concurrent_vector

struct paTestData
{
	unsigned int          frameIndex;  /* Index into sample array. */
									   //unsigned int          recordedSamples.size( );
	safevector<SAMPLE>	  recordedSamples;
	int playerid;
	bool				  PlayStopped;
	bool				  FirstPlay;
	bool				  Muted;

	unsigned int		  LastPlayTime;
	paTestData( )
	{
		frameIndex = /*recordedSamples.size( ) = */0;
		PlayStopped = false;
		FirstPlay = false;
		LastPlayTime = 0;
		Muted = false;
	}
};


safevector< paTestData > paTestDataPlayList;
safevector< paTestData > paTestDataSendList;



safevector< HANDLE > paThreads;


void SendVoicePacketParted( unsigned char * bytes, int bytescount )
{
	while ( bytescount > 256 )
	{
		
		
		std::vector<unsigned char> BytesToSend;
		BytesToSend.push_back( 0x50 );
		// packet header
		BytesToSend.push_back( 0xFF );
		// packet size
		BytesToSend.push_back( 0 );
		BytesToSend.push_back( 0 );
		BytesToSend.push_back( 0 );
		BytesToSend.push_back( 0 );
		// packet data
		int PacketId = 0xA1A2A3A0;

		BytesToSend.insert( BytesToSend.end( ), ( unsigned char * )&PacketId, ( ( unsigned char * )&PacketId ) + 4 );
		*( int* )&BytesToSend[ 2 ] += 4;

		std::vector<unsigned char> SendBytes;
		SendBytes.insert( SendBytes.end( ), bytes, bytes + 1000 );

	
		BytesToSend.insert( BytesToSend.end( ), ( unsigned char * )&PacketId, ( ( unsigned char * )&PacketId ) + 4 );
		*( int* )&BytesToSend[ 2 ] += 4;


		if ( PacketId != 0xA1A2A3A1 )
		{
			BytesToSend.insert( BytesToSend.end( ), bytes, bytes + 1000 );
			*( int* )&BytesToSend[ 2 ] += 1000;
		}
		else
		{
			
		}

	/*

		FILE * f;
		fopen_s( &f, "packetout.txt", "a+" );

		fprintf_s( f, "Send packets %i size (%s):\n", 1000, PacketId != 0xA1A2A3A1 ? "without compress" : "compressed" );
		for ( int i = 0; i < 1000; i++ )
		{
			fprintf_s( f, "%02X", bytes[ i ] );
		}
		fprintf_s( f, "%s", "\n" );
		fclose( f );
*/

		//try
		//{
		SendPacket( &BytesToSend[ 0 ], BytesToSend.size( ) );

		bytescount -= 1000;

		bytes += 1000;
	}

	if ( bytescount > 0 )
	{
		std::vector<unsigned char> BytesToSend;
		BytesToSend.push_back( 0x50 );
		// packet header
		BytesToSend.push_back( 0xFF );
		// packet size
		BytesToSend.push_back( 0 );
		BytesToSend.push_back( 0 );
		BytesToSend.push_back( 0 );
		BytesToSend.push_back( 0 );
		// packet data
		int PacketId = 0xA1A2A3A0;

		BytesToSend.insert( BytesToSend.end( ), ( unsigned char * )&PacketId, ( ( unsigned char * )&PacketId ) + 4 );
		*( int* )&BytesToSend[ 2 ] += 4;

		std::vector<unsigned char> SendBytes;
		SendBytes.insert( SendBytes.end( ), bytes, bytes + bytescount );

		BytesToSend.insert( BytesToSend.end( ), ( unsigned char * )&PacketId, ( ( unsigned char * )&PacketId ) + 4 );
		*( int* )&BytesToSend[ 2 ] += 4;


		if ( PacketId != 0xA1A2A3A1 )
		{
			BytesToSend.insert( BytesToSend.end( ), bytes, bytes + bytescount );
			*( int* )&BytesToSend[ 2 ] += bytescount;
		}
		else
		{
	
		}


		//FILE * f;
		//fopen_s( &f, "packetout.txt", "a+" );
		//fprintf_s( f, "Send packets %i size (%s):\n", bytescount, PacketId != 0xA1A2A3A1 ? "without compress" : "compressed" );

		//for ( int i = 0; i < bytescount; i++ )
		//{
		//	fprintf_s( f, "%02X", bytes[ i ] );
		//}
		//fprintf_s( f, "%s", "\n" );
		//fclose( f );


		//try
		//{
		SendPacket( &BytesToSend[ 0 ], BytesToSend.size( ) );
	}
}


int frameid = 0;
static int recordCallback( const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData )
{

	paTestData *data = ( paTestData* )userData;
	SAMPLE *rptr = ( SAMPLE* )inputBuffer;
	SAMPLE *wptr = &data->recordedSamples[ data->frameIndex ];
	long framesToCalc;
	long i;
	int finished;
	unsigned long framesLeft = data->recordedSamples.size( ) - data->frameIndex;

	( void )outputBuffer;
	( void )timeInfo;
	( void )statusFlags;
	( void )userData;


	if ( framesLeft < framesPerBuffer )
	{
		framesToCalc = framesLeft;
		finished = paComplete;
	}
	else
	{
		framesToCalc = framesPerBuffer;
		finished = paContinue;
	}

	if ( IsKeyPressed( VK_OEM_3 ) )
	{
		data->recordedSamples.resize( data->recordedSamples.size( ) + framesPerBuffer );
	}


	if ( IsGame( ) )
	{
		SetTlsForMe( );
		//	PrintText( "Send " + std::to_string( framesToCalc ) + " bytes !" );
		SendVoicePacketParted( ( unsigned char * )rptr, framesToCalc * 2 );
		//}
		//catch ( ... )
		//{
		//	PrintText( "Send packet fatal error!" );
		//}
		//AddNewPaTestData( BytesToSend, 1 );
		// framesToCalc = size , rptr = data
		//CWC3 * gproxy_wc3 = ( CWC3* )wc3classgproxy;
		//for ( auto s : paTestDataSendList )
		//{
		//gproxy_wc3->SendVoicePacket( data->frameIndex, framesToCalc, rptr );
		//}
	}
	else
		finished = paComplete;


	data->frameIndex += framesToCalc;
	return finished;
}




static int playCallback( const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData )
{

	int bufferid = ( int )userData;

	if ( bufferid >= paTestDataPlayList.size( ) )
		return paComplete;

	paTestData & data = paTestDataPlayList[ bufferid ];


	if ( data.Muted )
		return paComplete;

	SAMPLE *rptr = &data.recordedSamples[ data.frameIndex ];
	SAMPLE *wptr = ( SAMPLE* )outputBuffer;
	unsigned int i;
	int finished;
	unsigned int framesLeft = data.recordedSamples.size( ) - data.frameIndex;


	( void )inputBuffer;
	( void )timeInfo;
	( void )statusFlags;
	( void )userData;


	if ( framesLeft < framesPerBuffer )
	{

		for ( i = 0; i < framesLeft; i++ )
		{
			*wptr++ = *rptr++;
		}
		for ( ; i < framesPerBuffer; i++ )
		{
			*wptr++ = 0;
		}
		data.frameIndex += framesLeft;
		data.recordedSamples.clear( );
		data.frameIndex = 0;
		data.PlayStopped = true;
		finished = paComplete;
	}
	else
	{
		for ( i = 0; i < framesPerBuffer; i++ )
		{
			*wptr++ = *rptr++;
		}
		data.frameIndex += framesPerBuffer;
		finished = paContinue;
	}

	return finished;
}





paTestData RecordMessage( void )
{

	PaStream*           recordstream = nullptr;

	PaStreamParameters  inputParameters;

	PaError             err = paNoError;

	paTestData          data;
	int                 i;
	int                 totalFrames;
	int                 numSamples;
	int                 numBytes;
	data.playerid = GetLocalPlayerId( );

	totalFrames = ( int )( NUM_SECONDS * SAMPLE_RATE );
	data.frameIndex = 0;
	numSamples = totalFrames;
	numBytes = numSamples * sizeof( SAMPLE );
	data.recordedSamples = safevector<SAMPLE>( numSamples, 0 );

	inputParameters.device = Pa_GetDefaultInputDevice( );
	if ( inputParameters.device == paNoDevice ) {

		Sleep( 2000 );

		return paTestData( );
	}
	inputParameters.channelCount = 1;                    /* stereo input */
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
		&recordstream,
		&inputParameters,
		NULL,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		0,
		recordCallback,
		&data );


	if ( err != paNoError || recordstream == nullptr )
	{
		return paTestData( );
	}

	err = Pa_StartStream( recordstream );

	if ( err != paNoError )
	{
		recordstream = nullptr;
		return paTestData( );
	}

	while ( Pa_IsStreamActive( recordstream ) && IsKeyPressed( VK_OEM_3 ) )
	{
		Pa_Sleep( 5 );
	}

	if ( !IsKeyPressed( VK_OEM_3 ) )
	{
		data.recordedSamples.resize( data.frameIndex );
		Pa_AbortStream( recordstream );
	}
	else
		Pa_CloseStream( recordstream );


	return data;
}



DWORD WINAPI PLAYDATAPROCESSTHREAD( LPVOID lpdata )
{
	SetTlsForMe( );
	int bufferid = ( int )lpdata;


	if ( bufferid >= paTestDataPlayList.size( )  )
		return 0;

	paTestData & data = paTestDataPlayList[ bufferid ];

	if ( data.FirstPlay )
	{
		//PrintText( "First play" );
		data.frameIndex = 0;
		Sleep( 1000 );
		data.FirstPlay = false;
	}

	PaError             err = paNoError;
	PaStreamParameters  outputParameters;

	PaStream*           stream;

	outputParameters.device = Pa_GetDefaultOutputDevice( );
	outputParameters.channelCount = 1;
	outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	if ( outputParameters.device == paNoDevice ) {
		Sleep( 2000 );
		paTestDataPlayList[ bufferid ].PlayStopped = true;
		return 0;
	}

	err = Pa_OpenStream(
		&stream,
		NULL,
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		0 ,
		playCallback,
		( LPVOID )bufferid );

	if ( err != paNoError )
	{
		//PrintText( "error" + std::to_string((int)err) );
		paTestDataPlayList[ bufferid ].PlayStopped = true;
		return 0;
	}

	if ( stream )
	{
		err = Pa_StartStream( stream );
		if ( err != paNoError )
		{
			//PrintText( "error stream" );
			paTestDataPlayList[ bufferid ].PlayStopped = true;
			return 0;
		}

		while ( ( err = Pa_IsStreamActive( stream ) ) == 1 )
			Pa_Sleep( 50 );
		if ( err < 0 )
		{
			return 0;
		}

		err = Pa_CloseStream( stream );
		if ( err != paNoError )
		{
			return 0;
		}

	}

	if ( err != paNoError )
	{
		err = 1;
	}
	return 0;
}

void AddNewPaTestData( std::vector<unsigned char> _samples, int playerid, int packetsize, bool compressed )
{



	SAMPLE * samples = ( SAMPLE * )&_samples[ 0 ];
	int maxframeidx = packetsize / 2 ;

	//PrintText( "Handle pa packet! Samples count:" + std::to_string( maxframeidx ) + ", PID:" + std::to_string( playerid ) );

	int playdataidx = 0;
	for ( auto & s : paTestDataPlayList )
	{
		bool playernamesame = playerid == s.playerid;
		if ( playernamesame )
		{
			s.LastPlayTime = GetTickCount( );

			if ( s.Muted )
				return;

			if ( !s.PlayStopped && playernamesame )
			{
				for ( unsigned int i = 0; i < maxframeidx; i++ )
				{
					s.recordedSamples.push_back( samples[ i ] );
				}
				return;
			}
			else
			{
				s.FirstPlay = true;
				s.frameIndex = 0;
				s.recordedSamples.clear( );
				s.recordedSamples = safevector<SAMPLE>( maxframeidx );

				memcpy( &s.recordedSamples[ 0 ], samples, maxframeidx * sizeof( SAMPLE ) );

				s.PlayStopped = false;
				DWORD thread_id;
				paThreads.push_back( CreateThread( NULL, 8192, PLAYDATAPROCESSTHREAD, ( LPVOID )playdataidx,
					STACK_SIZE_PARAM_IS_A_RESERVATION, &thread_id ) );
				//	PrintText( "Start play" );

				return;
			}
		}

		playdataidx++;
	}

	paTestData tmpNewData = paTestData( );
	tmpNewData.frameIndex = 0;
	tmpNewData.recordedSamples = safevector<SAMPLE>( maxframeidx );
	memcpy( &tmpNewData.recordedSamples[ 0 ], samples, maxframeidx * sizeof( SAMPLE ) );

	tmpNewData.playerid = playerid;
	tmpNewData.PlayStopped = false;
	tmpNewData.FirstPlay = true;
	paTestDataPlayList.push_back( tmpNewData );
	DWORD thread_id;
	paThreads.push_back( CreateThread( NULL, 8192, PLAYDATAPROCESSTHREAD, ( LPVOID )playdataidx,
		STACK_SIZE_PARAM_IS_A_RESERVATION, &thread_id ) );

	safevector<HANDLE> outpaThreads;

	for ( auto s : paThreads )
	{
		DWORD exitcode = 0;
		GetExitCodeThread( s, &exitcode );
		if ( exitcode != STILL_ACTIVE )
		{
			CloseHandle( s );
		}
		else
			outpaThreads.push_back( s );
	}

	paThreads.clear( );
	paThreads = outpaThreads;
}


DWORD WINAPI VoiceClientRecordThread( LPVOID )
{
	SetTlsForMe( );

	PaError             err = paNoError;
	err = Pa_Initialize( );
	if ( err != paNoError ) {
		Pa_Terminate( );
		return 0;
	}

	while ( true )
	{
		Sleep( 100 );

		while ( IsKeyPressed( VK_OEM_3 ) )
		{
			if ( IsGame( )/* && !IsChatActive( )*/ )
			{
				PrintText( "Start recording messages..." );
				RecordMessage( );
			}
			else
				break;
		}
	}

	return 0;
}


HANDLE UnrealVoiceClientThreadId;
void InitVoiceClientThread( )
{
	UnrealVoiceClientThreadId = CreateThread( 0, 0, VoiceClientRecordThread, 0, 0, 0 );
	Packet_Initialize( 0 );
}

void UninitializeVoiceClient( )
{
	if ( UnrealVoiceClientThreadId != NULL )
	{
		Pa_Terminate( );
		TerminateThread( UnrealVoiceClientThreadId, 0 );
	}
	UnrealVoiceClientThreadId = NULL;
}
