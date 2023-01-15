#include "stdafx.h"
#include "Skin.h"

#include "FunctionTemplate.h"
#include "Offsets.h"

namespace Skin {
	const char* getPathByName(const char* name, const char* type) {
		return dreamaero::generic_fast_call<const char*>(
			Offset(SKIN_GETPATHBYNAME),
			name,
			type
			);
	}
}