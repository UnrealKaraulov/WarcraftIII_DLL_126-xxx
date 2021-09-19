#include "stdafx.h"
#ifndef CLASS_DEF_H_INCLUDED_
#define CLASS_DEF_H_INCLUDED_

template <class ClassType = void, class Type1 = void*, class Type2 = void*, class Type3 = void*, class Type4 = void*>
struct ClassMethod {
	typedef void (__thiscall *ArgNone)(ClassType* object);
	typedef void (__thiscall *ArgCount1)(ClassType* object, Type1 arg1);
	typedef void (__thiscall *ArgCount2)(ClassType* object, Type1 arg1, Type2 arg2);
	typedef void (__thiscall *ArgCount3)(ClassType* object, Type1 arg1, Type2 arg2, Type3 arg3);
	typedef void (__thiscall *ArgCount4)(ClassType* object, Type1 arg1, Type2 arg2, Type3 arg3, Type4 arg4);
	template <typename ReturnType>
	static ReturnType getVtableMethodByIndex(void** vtable, uint32_t index) {
		return reinterpret_cast<ReturnType>(vtable[index]);
	}

	template <typename ReturnType>
	static ReturnType getVtableMethodByOffset(void** vtable, uint32_t offset) {
		uint32_t index = offset / sizeof(void*);
		return getVtableMethodByIndex<ReturnType>(vtable, index);
	}
};

template <typename ReturnType>
ReturnType getVtableMethodByIndex(void** vtable, uint32_t index) {
	return ClassMethod<>::getVtableMethodByIndex<ReturnType>(vtable[index]);
}

template <typename ReturnType>
ReturnType getVtableMethodByOffset(void** vtable, uint32_t offset) {
	return ClassMethod<>::getVtableMethodByOffset<ReturnType>(vtable[index]);
}

#endif