#include "stdafx.h"
#ifndef UI_SIMPLEFRAME_H_INCLUDED
#define UI_SIMPLEFRAME_H_INCLUDED

#include "UILayoutFrame.h"

class UISimpleFrame : public UILayoutFrame {
public:
	static UISimpleFrame* Create(UISimpleFrame* UIParent = 0);	//����ƾ�մ���һ��SimpleFrame��������������ﴴ����˵��Ŀ���ǲ�����Ϸ�����е�
	static void				Destroy(UISimpleFrame* obj);	//ֻ��ʹ��Create������SimpleFrame����ҪDestroy��

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

	UISimpleFrame(void* base = NULL);//TODO ��Ϊprivate
private:
	void initTextureSettings();
};

#endif