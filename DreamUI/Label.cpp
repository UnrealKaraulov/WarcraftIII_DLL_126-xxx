#include "stdafx.h"
#include "Label.h"
#include "UISimpleFontString.h"
#include "UISimpleTexture.h"

const float LABEL_TEXT_OFFSET = 0.0003f;

void FilterStringColor(std::string& inString)
{
	int pos = std::string::npos;
	while (
		(pos = inString.find("|c")) != std::string::npos ||
		(pos = inString.find("|C")) != std::string::npos
		)
	{
		inString.replace(pos, 10, "");
	}
	while (
		(pos = inString.find("|r")) != std::string::npos ||
		(pos = inString.find("|R")) != std::string::npos
		)
	{
		inString.replace(pos, 2, "");
	}
}

void Label::tick()
{
	Frame::tick();
	if (bWantApplyUpdate)
	{
		bWantApplyUpdate = false;
		refresh();
	}
}

void Label::setScale(float scale)
{
	Frame::setScale(scale);
	lbBackground->setScale(scale);
	for (int i = 0; i < 8; i++)
	{
		if (lbTextShadow[i]) lbTextShadow[i]->setScale(scale);
	}
	lbText->setScale(scale);
}

void Label::activate(bool flag)
{
	Frame::activate(flag);
	bWantApplyUpdate = true;
}

void Label::showBackground(bool flag)
{
	bWantShowBackground = flag;
	bWantApplyUpdate = true;
}

void Label::setFont(const char* fontPath)
{
	lbTextFontPath.assign(fontPath);
	bWantApplyUpdate = true;
}

bool Label::setText(const char* format, ...)
{
	if (format)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, 512, format, args);
		va_end(args);
		if (lbTextContent.compare(buffer) != 0)
		{
			lbTextContent = buffer;
			bWantApplyUpdate = true;
			return true;
		}
	}
	else
	{
		if (lbTextContent.compare("") != 0)
		{
			lbTextContent = "";
			bWantApplyUpdate = true;
			return true;
		}
	}
	return false;
}

char* Label::getText() const
{
	return (char*)(lbTextContent.c_str());
}

void Label::setTextSize(float size)
{
	lbTextSize = size;
	bWantApplyUpdate = true;
}

void Label::setTextColor(Color textColor, Color textDisabledColor /* = LABEL_TEXT_DISABLED_COLOR */, Color shadowColor /* = LABEL_SHADOW_COLOR */)
{
	bool changed = false;
	if (lbTextColor != textColor)
	{
		lbTextColor = textColor;
		changed = true;
	}
	if (lbTextDisabledColor != textDisabledColor)
	{
		lbTextDisabledColor = textDisabledColor;
		changed = true;
	}
	if (lbTextShadowColor != shadowColor)
	{
		lbTextShadowColor = shadowColor;
		changed = true;
	}
	if (changed)
	{
		bWantApplyUpdate = true;
	}
}

void Label::initLabel(const char* text, const char* fontPath, bool withBackground, float textSize)
{
	lbTextContent.assign(text ? text : "");
	lbTextFontPath.assign(fontPath ? fontPath : "");
	bWantShowBackground = withBackground;
	lbTextSize = textSize;

	lbBackground = UISimpleTexture::Create(getFrame());

	const char* fontPathUsed = (lbTextFontPath.length()) ?
		lbTextFontPath.c_str() : Skin::getPathByName("MasterFont");

	for (int i = 0; i < 8; i++)
	{
		lbTextShadow[i] = UISimpleFontString::Create(getFrame());
		lbTextShadow[i]->setColor(lbTextShadowColor.toUint32());
		lbTextShadow[i]->initFont(fontPathUsed, textSize, 0);
		std::string lbTextContentNoFormat;
		lbTextContentNoFormat.assign(lbTextContent);
		FilterStringColor(lbTextContentNoFormat);
		lbTextShadow[i]->setText(lbTextContentNoFormat.c_str());
	}

	lbText = UISimpleFontString::Create(getFrame()); //TODO 能否改成覆盖在前一个
	lbText->setColor(lbTextColor.toUint32());
	lbText->initFont(fontPathUsed, textSize, 0);
	lbText->setText(lbTextContent.c_str());

	float bgWidth, bgHeight;
	bgWidth = lbText->getTextWidth() * War3WindowRatioX() + 0.002f / War3WindowRatioX();
	bgHeight = lbTextSize + 0.002f;

	lbBackground->setWidth(bgWidth);
	lbBackground->setHeight(bgHeight);
	if (bWantShowBackground) {
		lbBackground->fillColor(0xC0000000);
	}

	Frame::setWidth(bgWidth);
	Frame::setHeight(bgHeight);
	lbText->setRelativePosition(POS_C, this->getFrame(), POS_C, 0, 0);
	lbBackground->setRelativePosition(POS_C, lbText, POS_C, 0, 0);

	lbTextShadow[0]->setRelativePosition(POS_C, lbText, POS_C, LABEL_TEXT_OFFSET, -LABEL_TEXT_OFFSET);
	lbTextShadow[1]->setRelativePosition(POS_C, lbText, POS_C, LABEL_TEXT_OFFSET, 0);
	lbTextShadow[2]->setRelativePosition(POS_C, lbText, POS_C, LABEL_TEXT_OFFSET, LABEL_TEXT_OFFSET);
	lbTextShadow[3]->setRelativePosition(POS_C, lbText, POS_C, 0, LABEL_TEXT_OFFSET);
	lbTextShadow[4]->setRelativePosition(POS_C, lbText, POS_C, -LABEL_TEXT_OFFSET, LABEL_TEXT_OFFSET);
	lbTextShadow[5]->setRelativePosition(POS_C, lbText, POS_C, -LABEL_TEXT_OFFSET, 0);
	lbTextShadow[6]->setRelativePosition(POS_C, lbText, POS_C, -LABEL_TEXT_OFFSET, -LABEL_TEXT_OFFSET);
	lbTextShadow[7]->setRelativePosition(POS_C, lbText, POS_C, 0, -LABEL_TEXT_OFFSET);

}

Label::Label(UISimpleFrame* parent, const char* text, float textSize, Color textColor /* = LABEL_TEXT_COLOR */, Color textDisabledColor /* = LABEL_TEXT_DISABLED_COLOR */, Color shadowColor /* = LABEL_SHADOW_COLOR */, const char* fontPath /* = NULL */, bool withBackground /* = false */)
	: Frame(NULL, 0.001f, 0.001f),
	lbTextColor(textColor), lbTextDisabledColor(textDisabledColor), lbTextShadowColor(shadowColor),
	bWantApplyUpdate(false)
{
	setParent(parent);
	initLabel(text, fontPath, withBackground, textSize);
	UI_Add(this);
}

Label::Label(IUIObject* parent, const char* text, float textSize, Color textColor /* = LABEL_TEXT_COLOR */, Color textDisabledColor /* = LABEL_TEXT_DISABLED_COLOR */, Color shadowColor /* = LABEL_SHADOW_COLOR */, const char* fontPath /* = NULL */, bool withBackground /* = false */)
	: Frame(parent, 0.001f, 0.001f),
	lbTextColor(textColor), lbTextDisabledColor(textDisabledColor), lbTextShadowColor(shadowColor),
	bWantApplyUpdate(false)
{
	initLabel(text, fontPath, withBackground, textSize);
}

Label::~Label()
{
	for (int i = 0; i < 8; i++)
	{
		if (lbTextShadow[i]) UISimpleFontString::Destroy(lbTextShadow[i]);
	}
	UISimpleFontString::Destroy(lbText);
	UISimpleTexture::Destroy(lbBackground);
}

void Label::refresh()
{
	Frame::refresh();
	//设置字体，字号
	const char* fontPathUsed = (lbTextFontPath.length()) ?
		lbTextFontPath.c_str() : Skin::getPathByName("MasterFont");
	for (int i = 0; i < 8; i++)
	{
		if (lbTextShadow[i]) lbTextShadow[i]->initFont(fontPathUsed, lbTextSize, 1);
	}

	lbText->initFont(fontPathUsed, lbTextSize, 1);

	//设置颜色
	for (int i = 0; i < 8; i++)
	{
		if (lbTextShadow[i]) lbTextShadow[i]->setColor(lbTextShadowColor.toUint32());
	}
	lbText->setColor(isActive() ? lbTextColor.toUint32() : lbTextDisabledColor.toUint32());

	//刷新文本
	const char* contentStr = lbTextContent.c_str();
	//std::string textContentNoFormat;
	//textContentNoFormat.assign(lbTextContent);
	//int pos;
	//while ( 
	//	(pos = textContentNoFormat.find("|c")) != std::string::npos	||
	//	(pos = textContentNoFormat.find("|C")) != std::string::npos
	//)
	//{
	//	textContentNoFormat.replace(pos, 10, "");
	//}
	//while ( 
	//	(pos = textContentNoFormat.find("|r")) != std::string::npos	||
	//	(pos = textContentNoFormat.find("|R")) != std::string::npos
	//	)
	//{
	//	textContentNoFormat.replace(pos, 2, "");
	//}

	std::string lbTextContentNoFormat;
	lbTextContentNoFormat.assign(lbTextContent);
	FilterStringColor(lbTextContentNoFormat);
	for (int i = 0; i < 8; i++)
	{
		if (lbTextShadow[i]) lbTextShadow[i]->setText(lbTextContentNoFormat.c_str());
	}
	lbText->setText(contentStr);

	//重设frame大小
	float bgWidth, bgHeight;
	bgWidth = lbText->getTextWidth() * War3WindowRatioX() + 0.001f / War3WindowRatioX();
	bgHeight = lbTextSize + 0.001f;

	lbBackground->setWidth(bgWidth);
	lbBackground->setHeight(bgHeight);
	Frame::setWidth(bgWidth);
	Frame::setHeight(bgHeight);
	lbText->setRelativePosition(POS_C, this->getFrame(), POS_C, 0, 0);
	lbBackground->setRelativePosition(POS_C, lbText, POS_C, 0, 0);

	lbTextShadow[0]->setRelativePosition(POS_C, lbText, POS_C, LABEL_TEXT_OFFSET, -LABEL_TEXT_OFFSET);
	lbTextShadow[1]->setRelativePosition(POS_C, lbText, POS_C, LABEL_TEXT_OFFSET, 0);
	lbTextShadow[2]->setRelativePosition(POS_C, lbText, POS_C, LABEL_TEXT_OFFSET, LABEL_TEXT_OFFSET);
	lbTextShadow[3]->setRelativePosition(POS_C, lbText, POS_C, 0, LABEL_TEXT_OFFSET);
	lbTextShadow[4]->setRelativePosition(POS_C, lbText, POS_C, -LABEL_TEXT_OFFSET, LABEL_TEXT_OFFSET);
	lbTextShadow[5]->setRelativePosition(POS_C, lbText, POS_C, -LABEL_TEXT_OFFSET, 0);
	lbTextShadow[6]->setRelativePosition(POS_C, lbText, POS_C, -LABEL_TEXT_OFFSET, -LABEL_TEXT_OFFSET);
	lbTextShadow[7]->setRelativePosition(POS_C, lbText, POS_C, 0, -LABEL_TEXT_OFFSET);

	//设置背景
	lbBackground->fillColor(bWantShowBackground ? 0xC0000000 : 0x00000000);
}

float Label::getTextWidth() const
{
	return lbText->getTextWidth() * War3WindowRatioX();
}
float Label::getTextHeight() const
{
	return lbText->getTextHeight() * War3WindowRatioY();
}
