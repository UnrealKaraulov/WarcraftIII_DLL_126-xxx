#include "stdafx.h"
#ifndef UI_SIMPLEFRAME_H_INCLUDED
#define UI_SIMPLEFRAME_H_INCLUDED

#include "UILayoutFrame.h"

class UISimpleFrame : public UILayoutFrame {
public:
	static UISimpleFrame* Create(UISimpleFrame* UIParent = 0);	//用来凭空创建一个SimpleFrame，如果不经过这里创建，说明目的是操作游戏中已有的
	static void				Destroy(UISimpleFrame* obj);	//只有使用Create创建的SimpleFrame才需要Destroy！

	void applyTextureSettings();
	void setBackground(const char* path);
	void setBorder(const char* path);
	void setPadding(float top, float bottom, float left, float right);
	void setPadding(float padding = 0.f);
	void setBorderWidth(float width);
	void show();
	void hide();
	bool visible();
	void setParent(UISimpleFrame* parent);
	void setLevel(int level);
	UILayoutFrame* parent();

	UISimpleFrame(void* base = NULL);//TODO 改为private
private:
	void initTextureSettings();
};

#endif