#include "stdafx.h"
#ifndef FRAME_H_INCLUDED_
#define FRAME_H_INCLUDED_

#include "UIStructs.h"
#include "FunctionTemplate.h"
#include "Offsets.h"

namespace LayoutFrame {
	namespace Position {
		const unsigned int TOP_LEFT = 0;
		const unsigned int TOP_CENTER = 1;
		const unsigned int TOP_RIGHT = 2;
		const unsigned int LEFT = 3;
		const unsigned int CENTER = 4;
		const unsigned int RIGHT = 5;
		const unsigned int BOTTOM_LEFT = 6;
		const unsigned int BOTTOM_CENTER = 7;
		const unsigned int BOTTOM_RIGHT = 8;
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
	inline war3::FRAMENODE* setRelativeFrame(ThisFrameType* thisFrame, TargetFrameType* targetFrame, unsigned int flag = 1) {
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
	inline war3::FRAMENODE* setRelativePosition(ThisFrameType* thisFrame, unsigned int orginPosition, TargetFrameType* targetFrame, unsigned int toPosition, float relativeX, float relativeY, unsigned int flag = 1) {
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
	inline war3::CFramePointAbsolute* setAbsolutePosition(ThisFrameType* thisFrame, unsigned int orginPosition, float absoluteX, float absoluteY, unsigned int flag = 1) {
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
	inline unsigned int setWidth(FrameType* thisFrame, float v) {
		return thisFrame ?
			dreamaero::generic_this_call<unsigned int>(
				Offset(LAYOUTFRAME_SETWIDTH),
				thisFrame,
				v
				) : 0;
	}

	template <typename FrameType>
	inline unsigned int setHeight(FrameType* thisFrame, float v) {
		return thisFrame ?
			dreamaero::generic_this_call<unsigned int>(
				Offset(LAYOUTFRAME_SETHEIGHT),
				thisFrame,
				v
				) : 0;
	}

	template <typename FrameType>
	inline unsigned int updatePosition(FrameType* thisFrame, unsigned int arg) {
		return thisFrame ?
			dreamaero::generic_this_call<unsigned int>(
				Offset(LAYOUTFRAME_UPDATEPOSITION),
				thisFrame,
				arg
				) : 0;
	}
}

#endif