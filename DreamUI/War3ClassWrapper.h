#include "stdafx.h"
#ifndef WAR3_CLASS_WRAPPER
#define WAR3_CLASS_WRAPPER

#include "FunctionTemplate.h"

class War3ClassWrapper {
public:
	void setBase(void* base) { this->baseObject_ = base; }
	void* base(unsigned int offset = 0) { return dreamaero::pointer_calc<void*>(this->baseObject_, offset); }
	template <typename ReturnType>
	ReturnType base(unsigned int offset = 0) { return dreamaero::pointer_calc<ReturnType>(this->baseObject_, offset); }
private:
	void* baseObject_;
};

#endif