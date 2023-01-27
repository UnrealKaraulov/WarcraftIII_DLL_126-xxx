#include "stdafx.h"
#ifndef SIMPLEREGION_H_INCLUDED
#define SIMPLEREGION_H_INCLUDED

#include "UIStructs.h"
#include "FunctionTemplate.h"
#include "Offsets.h"

namespace SimpleRegion {
	template <typename ThisSimpleRegionType>
	inline unsigned int setColor(ThisSimpleRegionType* t, unsigned int argb) {
		return dreamaero::generic_this_call<unsigned int>(
			Offset(SIMPLEREGION_SETCOLOR),
			t,
			&argb
			);
	}
	/*
	template <typename ThisSimpleRegionType>
	inline void set80(ThisSimpleRegionType* t, float v80) {
		dreamaero::generic_this_call<void>(
			dreamaero::offset_element_get<void*>(VTBL(t), 0x28),
			t,
			v80
		);
	}
	*/
}

#endif