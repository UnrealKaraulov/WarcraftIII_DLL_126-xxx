#ifndef LABEL_H_
#define LABEL_H_

#include "Frame.h"

class Texture;
class UISimpleFontString;
class UISimpleTexture;

static Color	LABEL_TEXT_COLOR = Color(0.99f, 0.827f, 0.0705f);
static Color	LABEL_SHADOW_COLOR = Color(0.f, 0.f, 0.f, 0.9f);
static Color	LABEL_TEXT_DISABLED_COLOR = Color::GREY;

class Label : public Frame
{
public:
	Label(
		IUIObject* parent,
		const char* text,
		float		textSize,
		Color		textColor = LABEL_TEXT_COLOR,
		Color		textDisabledColor = LABEL_TEXT_DISABLED_COLOR,
		Color		shadowColor = LABEL_SHADOW_COLOR,
		const char* fontPath = NULL,
		bool		withBackground = false
	);

	Label(
		UISimpleFrame* parent,
		const char* text,
		float		textSize,
		Color		textColor = LABEL_TEXT_COLOR,
		Color		textDisabledColor = LABEL_TEXT_DISABLED_COLOR,
		Color		shadowColor = LABEL_SHADOW_COLOR,
		const char* fontPath = NULL,
		bool		withBackground = false
	);

	virtual ~Label();

	char* getText() const;
	float	getTextWidth() const;
	float   getTextHeight() const;
	void	showBackground(bool flag);
	void	setFont(const char* fontPath);
	bool	setText(const char* format, ...);
	void	setTextSize(float size);
	void	setTextColor(Color textColor, Color textDisabledColor = LABEL_TEXT_DISABLED_COLOR, Color shadowColor = LABEL_SHADOW_COLOR);

	virtual void	setScale(float scale);
	virtual void	activate(bool flag);
	virtual void	tick();
	virtual void	refresh();


private:
	UISimpleTexture* lbBackground;
	UISimpleFontString* lbText;
	UISimpleFontString* lbTextShadow[8];
	std::string			lbTextContent;
	std::string			lbTextFontPath;
	float				lbTextSize;
	bool				bWantShowBackground;
	Color				lbTextColor;
	Color				lbTextDisabledColor;
	Color				lbTextShadowColor;

	bool bWantApplyUpdate;
	void initLabel(const char* text, const char* fontPath, bool withBackground, float textSize);
};

#endif