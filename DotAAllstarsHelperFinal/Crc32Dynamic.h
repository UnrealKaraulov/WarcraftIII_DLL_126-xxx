#pragma once
#include "Common.h"

class CCrc32Dynamic
{
public:
	CCrc32Dynamic();
	virtual ~CCrc32Dynamic();

	void Init(void);
	void Free(void);

	unsigned long StringCrc32(char* szString, unsigned long& dwCrc32) const;
	unsigned long FileCrc32Streams(char* szFilename, unsigned long& dwCrc32) const;
	unsigned long FileCrc32Win32(char* szFilename, unsigned long& dwCrc32) const;
	unsigned long FileCrc32Filemap(char* szFilename, unsigned long& dwCrc32) const;
	unsigned long FileCrc32Assembly(char* szFilename, unsigned long& dwCrc32) const;

protected:
	static int GetFileSizeQW(const void* hFile, QWORD& qwSize);
	inline void CalcCrc32(const unsigned char _byte, unsigned long& dwCrc32) const;

	unsigned long* m_pdwCrc32Table;
};

