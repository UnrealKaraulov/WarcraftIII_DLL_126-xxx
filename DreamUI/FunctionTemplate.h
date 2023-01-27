#include "stdafx.h"
#ifndef FUNCTION_TEMPLATE_H_INCLUDED_
#define FUNCTION_TEMPLATE_H_INCLUDED_
#include "fp_dream_call.h"

inline void* vtableFpAtIndex(void** vtable, unsigned int index) {
	return vtable ? vtable[index] : NULL;
}

inline void* vtableFpAtOffset(void** vtable, unsigned int offset) {
	unsigned int index = offset / sizeof(void*);
	return vtableFpAtIndex(vtable, index);
}

#endif