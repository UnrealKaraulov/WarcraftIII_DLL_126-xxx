#include "stdafx.h"
#ifndef FRAME_H_INCLUDED_
#define FRAME_H_INCLUDED_

#include "UIStructs.h"
#include "FunctionTemplate.h"
#include "Offsets.h"

namespace LayoutFrame {
	namespace Position {
		const uint32_t TOP_LEFT = 0;
		const uint32_t TOP_CENTER = 1;
		const uint32_t TOP_RIGHT = 2;
		const uint32_t LEFT = 3;
		const uint32_t CENTER = 4;
		const uint32_t RIGHT = 5;
		const uint32_t BOTTOM_LEFT = 6;
		const uint32_t BOTTOM_CENTER = 7;
		const uint32_t BOTTOM_RIGHT = 8;
	};

	//获取相对点对象
	template <typename FrameType>
	war3::CFramePointRelative* getRelativePoint(FrameType* frame, float relativeX, float relativeY) {
		return frame ? 
		dreamaero::generic_fast_call<war3::CFramePointRelative*>(
			Offset(LAYOUTFRAME_GETRELPOINT),
			frame,
			relativeX,
			relativeY
		) : NULL;
	}

	//TODO 加入直接操作点改变大小和位置的API

	template <typename ThisFrameType, typename TargetFrameType>
	inline war3::FRAMENODE* setRelativeFrame(ThisFrameType* thisFrame, TargetFrameType* targetFrame, uint32_t flag = 1) {
		return thisFrame ? 
		dreamaero::generic_this_call<war3::FRAMENODE*>(
			Offset(LAYOUTFRAME_SETRELFRAME),
			thisFrame,
			targetFrame,
			flag			//TODO 搞清楚这个是什么
		) : NULL;
	}

	//设定相对位置
	template <typename ThisFrameType, typename TargetFrameType>
	inline war3::FRAMENODE* setRelativePosition(ThisFrameType* thisFrame, uint32_t orginPosition, TargetFrameType* targetFrame, uint32_t toPosition, float relativeX, float relativeY, uint32_t flag = 1) {
		return thisFrame ? 
		dreamaero::generic_this_call<war3::FRAMENODE*>(
			Offset(LAYOUTFRAME_SETRELPOS), 
			thisFrame, 
			orginPosition,
			targetFrame,
			toPosition,
			relativeX,
			relativeY,
			flag			//TODO 搞清楚这个是什么
		) : NULL;
	}

	//设定绝对位置
	template <typename ThisFrameType>
	inline war3::CFramePointAbsolute* setAbsolutePosition(ThisFrameType* thisFrame, uint32_t orginPosition, float absoluteX, float absoluteY, uint32_t flag = 1) {
		return thisFrame ? 
		dreamaero::generic_this_call<war3::CFramePointAbsolute*>(
			Offset(LAYOUTFRAME_SETABSPOS), 
			thisFrame, 
			orginPosition, 
			absoluteX, 
			absoluteY, 
			flag			//TODO 搞清楚这个是什么
		) : NULL;
	}
	
	template <typename FrameType>
	inline float getScaledWidth(FrameType* thisFrame) {
		return thisFrame ? 
		dreamaero::generic_this_call<float>(
			dreamaero::offset_element_get<void*>(reinterpret_cast<war3::CLayoutFrame*>(thisFrame)->vtable, 0x18), 
			thisFrame
		) : 0;
	}

	template <typename FrameType>
	inline float getScaledHeight(FrameType* thisFrame) {
		return thisFrame ?
		dreamaero::generic_this_call<float>(
			dreamaero::offset_element_get<void*>(reinterpret_cast<war3::CLayoutFrame*>(thisFrame)->vtable, 0x1C), 
			thisFrame
		) : 0;
	}

	template <typename FrameType>
	inline uint32_t setWidth(FrameType* thisFrame, float v) {
		return thisFrame ? 
		dreamaero::generic_this_call<uint32_t>(
			Offset(LAYOUTFRAME_SETWIDTH), 
			thisFrame,
			v
		) : 0;
	}

	template <typename FrameType>
	inline uint32_t setHeight(FrameType* thisFrame, float v) {
		return thisFrame ? 
		dreamaero::generic_this_call<uint32_t>(
			Offset(LAYOUTFRAME_SETHEIGHT), 
			thisFrame,
			v
		) : 0;
	}

	template <typename FrameType>
	inline uint32_t updatePosition(FrameType* thisFrame, uint32_t arg) {
		return thisFrame ? 
		dreamaero::generic_this_call<uint32_t>(
			Offset(LAYOUTFRAME_UPDATEPOSITION), 
			thisFrame,
			arg
		) : 0;
	}
}

#endif