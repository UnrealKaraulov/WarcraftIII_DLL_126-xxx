#ifndef STRUCT_HELPER_H
#define STRUCT_HELPER_H

#include "stdafx.h"

inline void** VTBL(void* obj) {
	return *reinterpret_cast<void***>(obj);
}

#endif