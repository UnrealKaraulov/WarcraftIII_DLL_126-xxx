#ifndef STRUCT_HELPER_H
#define STRUCT_HELPER_H

inline void** VTBL(void* obj) {
	return *static_cast<void***>(obj);
}

#endif