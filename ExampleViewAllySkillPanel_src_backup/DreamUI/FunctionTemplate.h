#include "stdafx.h"
#ifndef FUNCTION_TEMPLATE_H_INCLUDED_
#define FUNCTION_TEMPLATE_H_INCLUDED_
#include "fp_call.h"

inline void* vtableFpAtIndex(void** vtable, uint32_t index) {
	return vtable ? vtable[index] : NULL;
}

inline void* vtableFpAtOffset(void** vtable, uint32_t offset) {
	uint32_t index = offset / sizeof(void*);
	return vtableFpAtIndex(vtable, index);
}

#endif