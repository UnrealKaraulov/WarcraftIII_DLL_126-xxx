//#include "stdafx.h"
#ifndef IUIObject_H_
#define IUIObject_H_

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif // _MSC_VER

class UISimpleFrame;
class UILayoutFrame;


enum AnchorPos
{
	POS_UL = 0,
	POS_T = 1,
	POS_UR = 2,
	POS_L = 3,
	POS_C = 4,
	POS_R = 5,
	POS_BL = 6,
	POS_B = 7,
	POS_BR = 8
};

enum TextAlignmentHorizontal
{
	ALIGN_HORIZ_LEFT,
	ALIGN_HORIZ_CENTER,
	ALIGN_HORIZ_RIGHT
};

enum TextAlignmentVertical
{
	ALIGN_VERT_TOP,
	ALIGN_VERT_CENTER,
	ALIGN_VERT_BOTTOM
};

struct Color
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;

	Color() : b(), g(), r(), a(0xFF) { }
	Color(Color* src) : b(src->b), g(src->g), r(src->r), a(src->a) {}
	Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 0xFF) : b(_b), g(_g), r(_r), a(_a) { }
	Color(float red, float green, float blue, float alpha = 1.f);
	Color mulRGB(Color& other);
	Color mulRGB(float val);
	unsigned int toUint32();
	bool operator==(const Color& rhs) const { return b == rhs.b && g == rhs.g && r == rhs.r && a == rhs.a; }
	bool operator!=(const Color& rhs) const { return b != rhs.b || g != rhs.g || r != rhs.r || a != rhs.a; }
	void operator *= (float val);

	static const Color WHITE;
	static const Color BLACK;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color LIGHTBLUE;
	static const Color CYAN;
	static const Color YELLOW;
	static const Color GOLD;
	static const Color GREY;
	static const Color DARKGREY;
};

#define UI_NULL ((IUIObject *)NULL)

class IUIObject {
public:

	virtual IUIObject* parent() const = 0;
	virtual IUIObject* owner() const = 0;

	virtual void 			setAbsolutePosition(unsigned int originPos, float absoluteX, float absoluteY) = 0;
	virtual void 			setRelativePosition(unsigned int originPos, IUIObject* target, unsigned int toPos, float relativeX = 0, float relativeY = 0) = 0;
	virtual void 			setRelativePosition(unsigned int originPos, UILayoutFrame* target, unsigned int toPos, float relativeX = 0, float relativeY = 0) = 0;
	virtual UISimpleFrame* getFrame() const = 0;
	virtual UISimpleFrame* getTopFrame() const = 0;

	virtual float	top() const = 0;
	virtual float	bottom() const = 0;
	virtual float	left() const = 0;
	virtual float	right() const = 0;
	virtual float	width() const = 0;
	virtual float	height() const = 0;
	virtual float	scale() const = 0;
	virtual float	scaledWidth() const = 0;
	virtual float	scaledHeight() const = 0;
	virtual bool	isShown() const = 0;
	virtual bool	isActive() const = 0;

	virtual void	setWidth(float width) = 0;
	virtual void	setHeight(float height) = 0;
	virtual void	setSize(float size) = 0;
	virtual void	setScale(float scale) = 0;
	virtual void	setParent(IUIObject* parent) = 0;
	virtual void	setParent(UISimpleFrame* parent) = 0;
	virtual void	setOwner(IUIObject* owner) = 0;

	virtual void	show(bool flag) = 0;
	virtual void	activate(bool flag) = 0;

	virtual void	tick() = 0;
	virtual void	refresh() = 0;
	virtual bool	isRefreshing() = 0;

	virtual ~IUIObject() { }
};

void UI_Init();
void UI_Cleanup();
void UI_Add(IUIObject* obj);
void UI_Remove(IUIObject* obj);
void UI_Refresh();

#ifdef _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

#endif

//TODO: class
//Button, Text, Label, ToolTip, CheckBox, PullDown
//MultiIcon
//Menu
//InfoPanel

