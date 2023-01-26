#include "stdafx.h"
#include "Frame.h"
#include "Tools.h"

#include "UISimpleFrame.h"

Frame::Frame(
	IUIObject* parent,
	float		width,
	float		height,
	const char* bgPath,
	const char* borderPath,
	float		borderWidth,
	float		padding)
	: simpleFrame(), parentUI(), ownerUI(),
	bWantApplyPosition(false), bWantApplyTexture(false),
	backgroundPath(), borderPath(), borderWidth(0), paddingT(0), paddingB(0), paddingL(0), paddingR(0),
	bPosUseAbsolute(true), posOriginAnchor(0), posRelativeTarget(), posToAnchor(0), posToX(0), posToY(0),
	bShow(true), bRefreshing(false),
	bActive(true),
	bShowInit(false)
{
	simpleFrame = UISimpleFrame::Create(parent ? parent->getFrame() : NULL);
	parentUI = parent;
	if (simpleFrame)
	{
		simpleFrame->setWidth(width);
		simpleFrame->setHeight(height > 0 ? height : 0);
		setBackground(bgPath);
		setPadding(padding);
		setBorder(borderPath, borderWidth);
	}
	UI_Add(this);
}

Frame::~Frame()
{
	UI_Remove(this);
	UISimpleFrame::Destroy(simpleFrame);
}

IUIObject* Frame::parent() const
{
	return parentUI;
}

void Frame::setParent(IUIObject* parent)
{
	parentUI = parent;
	simpleFrame->setParent(parentUI ? parentUI->getFrame() : NULL);
	bWantApplyPosition = true;
	bWantApplyTexture = true;
}

void Frame::setParent(UISimpleFrame* parent)
{
	parentUI = NULL; //没有对应的IUIObject
	simpleFrame->setParent(parent);
	bWantApplyPosition = true;
	bWantApplyTexture = true;
}

void Frame::setAbsolutePosition(unsigned int originPos, float absoluteX, float absoluteY)
{
	bPosUseAbsolute = true;
	posOriginAnchor = originPos;
	posToX = absoluteX;
	posToY = absoluteY;
	bWantApplyPosition = true;
}

void Frame::setRelativePosition(unsigned int originPos, IUIObject* target, unsigned int toPos, float relativeX, float relativeY)
{
	setRelativePosition(originPos, target->getFrame(), toPos, relativeX, relativeY);
}

void Frame::setRelativePosition(unsigned int originPos, UILayoutFrame* target, unsigned int toPos, float relativeX /* = 0 */, float relativeY /* = 0 */)
{
	bPosUseAbsolute = false;
	posOriginAnchor = originPos;
	posRelativeTarget = UILayoutFrame(target->base());
	posToAnchor = toPos;
	posToX = relativeX;
	posToY = relativeY;
	bWantApplyPosition = true;
}

UISimpleFrame* Frame::getFrame() const
{
	return simpleFrame;
}

UISimpleFrame* Frame::getTopFrame() const
{
	return simpleFrame;
}


float Frame::top() const
{
	
	return simpleFrame->base<war3::CSimpleFrame*>()->baseLayoutFrame.borderU;
}

float Frame::bottom() const
{
	
	return simpleFrame->base<war3::CSimpleFrame*>()->baseLayoutFrame.borderB;
}

float Frame::left() const
{
	
	return simpleFrame->base<war3::CSimpleFrame*>()->baseLayoutFrame.borderL;
}

float Frame::right() const
{
	
	return simpleFrame->base<war3::CSimpleFrame*>()->baseLayoutFrame.borderR;
}

float Frame::width() const
{
	
	//OutputScreen(1, "Frame::width() returns %.3f", simpleFrame->width());
	return simpleFrame->width();
}

float Frame::height() const
{
	
	//OutputScreen(1, "Frame::height() returns %.3f", simpleFrame->height());
	return simpleFrame->height();
}

float Frame::scale() const
{
	
	return simpleFrame->scale();
}

float Frame::scaledWidth() const
{
	
	return scale() * width();
}

float Frame::scaledHeight() const
{
	
	return scale() * height();
}

bool Frame::isShown() const
{
	return simpleFrame->visible();
}

bool Frame::isActive() const
{
	return bActive;
}

void Frame::setWidth(float width)
{
	float w = width;
	if (w < 0) w = 0;
	this->_width = w;
	simpleFrame->setWidth(w);
}

void Frame::setHeight(float height)
{
	simpleFrame->setHeight(height);
}

void Frame::setSize(float size)
{
	setWidth(size); setHeight(size);
}

void Frame::setScale(float scale)
{
	simpleFrame->setScale(scale);
}

void Frame::show(bool flag)
{
	if (!bShowInit)
	{
		bShowInit = true;
		bShow = flag;
		bWantApplyShow = true;
	}
	else
	{
		if (bShow != flag)
		{
			bShow = flag;
			bWantApplyShow = true;
		}
	}
}

void Frame::activate(bool flag) {
	bActive = flag; //没有其他的行为
}

void Frame::setBorder(const char* path, float width /* = 0.002f */)
{
	borderPath.assign(path ? path : "");
	borderWidth = width;
	bWantApplyTexture = true;
}

void Frame::setBorderWidth(float width)
{
	borderWidth = width;
	bWantApplyTexture = true;
}

void Frame::setBackground(const char* path)
{
	backgroundPath.assign(path ? path : "");
	bWantApplyTexture = true;
}

void Frame::setPadding(float top, float bottom, float left, float right)
{
	paddingT = top;
	paddingB = bottom;
	paddingL = left;
	paddingR = right;
	bWantApplyTexture = true;
}

void Frame::setPadding(float padding)
{
	setPadding(padding, padding, padding, padding);
}

void Frame::refreshTexture()
{
	bool valid = false;
	if (backgroundPath.length())
	{
		valid = true;
		simpleFrame->setBackground(backgroundPath.c_str());
		simpleFrame->setPadding(paddingT, paddingB, paddingL, paddingR);
	}
	if (borderPath.length())
	{
		valid = true;
		simpleFrame->setBorder(borderPath.c_str());
		simpleFrame->setBorderWidth(borderWidth);
	}
	if (valid)
	{
		simpleFrame->applyTextureSettings();
	}
}

void Frame::refreshPosition()
{
	if (bPosUseAbsolute)
	{
		simpleFrame->setAbsolutePosition(posOriginAnchor, posToX, posToY);
	}
	else
	{
		simpleFrame->setRelativePosition(posOriginAnchor,
			&posRelativeTarget, posToAnchor, posToX, posToY);
	}
	simpleFrame->applyPosition();
}

void Frame::refreshShow()
{
	if (bShow) simpleFrame->show();
	else simpleFrame->hide();
}

void Frame::tick()
{
	if (bWantApplyTexture)
	{
		refreshTexture();
		bWantApplyTexture = false;
	}
	if (bWantApplyPosition)
	{
		refreshPosition();
		bWantApplyPosition = false;
	}
	if (bWantApplyShow)
	{
		refreshShow();
		bWantApplyShow = false;
	}
}

void Frame::refresh()
{
	if (bRefreshing) return;
	//OutputScreen(1, "Frame::refresh() WidthRatioX = %.3f", War3WindowRatioX());
	bRefreshing = true;

	setWidth(this->_width);

	refreshTexture();
	refreshPosition();
	refreshShow();
	bWantApplyTexture = false;
	bWantApplyPosition = false;
	bWantApplyShow = false;
	bRefreshing = false;
}

void Frame::setOwner(IUIObject* owner)
{
	ownerUI = owner;
}

IUIObject* Frame::owner() const
{
	return ownerUI;
}

bool Frame::isRefreshing()
{
	return bRefreshing;
}

float Frame::getShouldWidth() const
{
	return _width;
}
