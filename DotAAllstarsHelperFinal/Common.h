#pragma once
#include "Main.h"

#ifndef _QWORD_DEFINED
#define _QWORD_DEFINED
typedef __int64 QWORD, * LPQWORD;
#endif

#define MAKEQWORD(a, b)	\
	((QWORD)( ((QWORD) ((unsigned long) (a))) << 32 | ((unsigned long) (b))))

#define LODWORD(l) \
	((unsigned long)(l))
#define HIDWORD(l) \
	((unsigned long)(((QWORD)(l) >> 32) & 0xFFFFFFFF))

// Read 4K of data at a time (used in the C++ streams, Win32 I/O, and assembly functions)
#define MAX_BUFFER_SIZE	4096

// Map a "view" size of 10MB (used in the filemap function)
#define MAX_VIEW_SIZE	10485760

