#include "stdafx.h"
#include "RCString.h"

#include "FunctionTemplate.h"
#include "Offsets.h"
#include "Storm.h"
#include "StructHelper.h"

namespace RCString {
	war3::RCString* create(const char* str) {
		war3::RCString* t = DreamStorm::MemAllocStruct<war3::RCString>();
		return dreamaero::generic_this_call<war3::RCString*>(
			Offset(RCSTRING_CONSTRUCT),
			t,
			str
			);
	}

	void destroy(war3::RCString* t) {
		dreamaero::generic_this_call<void>(
			VTBL(t)[1],
			t,
			1
			);
	}

	void setString(war3::RCString* t, const char* str) {
		dreamaero::generic_this_call<war3::RCString*>(
			Offset(RCSTRING_SETSTRING),
			t,
			str
			);
	}

	const char* getString(war3::RCString* t) {
		return t->stringRep->text;
	}
}