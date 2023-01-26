#ifndef FRAME_H_
#define FRAME_H_

#include "IUIObject.h"
#include "UILayoutFrame.h"
class UISimpleFrame;

class Frame : public IUIObject
{
public:
	Frame(
		IUIObject* parent,
		float		width,
		float		height,
		const char* bgPath = NULL,
		const char* borderPath = NULL,
		float		borderWidth = 0,
		float		padding = 0
	);

	virtual ~Frame();

	virtual IUIObject* parent() const;
	virtual IUIObject* owner() const;

	virtual void			setAbsolutePosition(unsigned int originPos, float absoluteX, float absoluteY);
	virtual void			setRelativePosition(unsigned int originPos, IUIObject* target, unsigned int toPos, float relativeX = 0, float relativeY = 0);
	virtual void 			setRelativePosition(unsigned int originPos, UILayoutFrame* target, unsigned int toPos, float relativeX = 0, float relativeY = 0);
	virtual UISimpleFrame* getFrame() const;
	virtual UISimpleFrame* getTopFrame() const;

	virtual float	top() const;
	virtual float	bottom() const;
	virtual float	left() const;
	virtual float	right() const;
	virtual float	width() const;
	virtual float	height() const;
	virtual float	scale() const;
	virtual float	scaledWidth() const;
	virtual float	scaledHeight() const;
	virtual bool	isShown() const;
	virtual bool	isActive() const;

	virtual void	setWidth(float width);
	virtual void	setHeight(float height);
	virtual void	setSize(float size);
	virtual void	setScale(float scale);
	virtual void	setParent(IUIObject* parent);
	virtual void	setParent(UISimpleFrame* parent);
	virtual void	setOwner(IUIObject* owner);

	virtual void	show(bool flag);
	virtual void	activate(bool flag);
	virtual void	tick();
	virtual void	refresh();
	virtual bool	isRefreshing();

	void setBorder(const char* path, float width = 0.002f);
	void setBorderWidth(float width);
	void setBackground(const char* path);
	void setPadding(float top, float bottom, float left, float right);
	void setPadding(float padding = 0.f);

	float getShouldWidth() const;
private:
	UISimpleFrame* simpleFrame;
	IUIObject* parentUI;
	IUIObject* ownerUI;
	bool			bShowInit;
	bool			bShow;
	bool			bWantApplyShow;
	bool			bActive;
	bool			bRefreshing;

	bool			bWantApplyPosition;
	bool			bWantApplyTexture;


	std::string		backgroundPath;
	std::string		borderPath;
	float			borderWidth;
	float			paddingT;
	float			paddingB;
	float			paddingL;
	float			paddingR;
	bool			bPosUseAbsolute;
	unsigned int		posOriginAnchor;
	UILayoutFrame	posRelativeTarget;
	unsigned int		posToAnchor;
	float			posToX;
	float			posToY;
	float			_width;

	void refreshTexture();
	void refreshPosition();
	void refreshShow();
};

#endif