// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <Windows.h>



// TODO: reference additional headers your program requires here
#include <tchar.h>
#include <map>
#include <deque>
#include <set>
#include <list>
#include <cstdint>
#include <vector>
#include <assert.h>
#include <string>
#include "fp_dream_call.h"

// Macros
#define INLINE __forceinline
#define NOINLINE __declspec(noinline)
#ifdef _DEBUG
#define DEBUG_CODE(code) \
	code
#else
#define DEBUG_CODE(code)
#endif

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

