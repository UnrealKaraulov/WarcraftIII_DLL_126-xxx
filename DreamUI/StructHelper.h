#ifndef STRUCT_HELPER_H
#define STRUCT_HELPER_H

#include "stdafx.h"

inline void** VTBL(void* obj) {
	return *static_cast<void***>(obj);
}

#endif