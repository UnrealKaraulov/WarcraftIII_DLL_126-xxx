#pragma once


#include "Main.h"
#include "Storm.h"

#pragma pack(push,1)


//extern int memoryleakcheck;

void  __stdcall AddNewLineToJassLog(const char* s);
void __stdcall  AddNewLineToDotaChatLog(const char* s);
void __stdcall  AddNewLineToDotaHelperLog(const char* s, int line);//( const char * s, int line );
void __stdcall  AddNewLineToJassNativesLog(const char* s);
void __stdcall EnableErrorHandler(int);
void __stdcall DisableErrorHandler(int);


//extern int memoryleakcheck;

class StormBuffer
{
private:

public:
	unsigned char* buf;
	unsigned long length;
	bool NeedClear = false;
	~StormBuffer( )
	{
		Clear( );
	}
	StormBuffer()
	{
		buf = 0;
		length = 0;
	}
	StormBuffer(unsigned long l)
	{
		//	memoryleakcheck++;
		length = l;
		buf = (unsigned char*)Storm::MemAlloc(l);
		NeedClear = true;
		buf[l] = '\0';
	}
	StormBuffer(unsigned char* b, unsigned long l)
	{
		buf = b;
		length = l;
	}

	void Resize(unsigned long l)
	{
		Clear();
		buf = (unsigned char*)Storm::MemAlloc(l);
		NeedClear = true;
		buf[l] = '\0';
		length = l;
	}

	unsigned char* GetData()
	{
		return buf;
	}
	unsigned char* GetData(int offset)
	{
		return buf + offset;
	}

	unsigned long GetSize()
	{
		return length;
	}

	void Clear()
	{
		//	memoryleakcheck--;
		length = 0;
		if (buf != NULL)
		{
			if (NeedClear)
				Storm::MemFree(buf);
		}
		buf = NULL;
		NeedClear = false;
	}

	StormBuffer& copy_to_new(const StormBuffer& CopyObject)
	{
		Resize(CopyObject.length);
		std::memcpy(buf, CopyObject.buf, length);
		return (*this);
	}

	StormBuffer& operator =(const StormBuffer& CopyObject)
	{
		Clear();
		/*Resize( CopyObject.length );
		std::memcpy( buf, CopyObject.GetData( ), length );*/
		length = CopyObject.length;
		buf = CopyObject.buf;
		return (*this);
	}
	StormBuffer& operator =(const std::string& CopyString)
	{
		Resize(static_cast<int>(CopyString.size()));
		std::memcpy(buf, CopyString.c_str(), length);
		return (*this);
	}

	unsigned char& operator [](int Index)
	{
		return buf[Index];
	}
};

typedef struct StormBufferList
{
	char** buf;
	unsigned long length;
	StormBufferList()
	{

		buf = 0;
		length = 0;
	}
	StormBufferList(unsigned long l)
	{
		buf = (char**)Storm::MemAlloc(l);
		length = l;
	}
	StormBufferList(char** b, unsigned long l)
	{

		buf = b;
		length = l;
	}
} StormBufferList;


#pragma pack(pop)