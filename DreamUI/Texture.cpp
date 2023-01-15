#include "stdafx.h"
#include "Texture.h"
#include "Frame.h"
#include "UISimpleFrame.h"
#include "UISimpleTexture.h"
#include "Tools.h"

void Texture::setColor(float r, float g, float b, float a /* = 1.f */)
{
	Color c(r, g, b, a);
	if (colorMult != c)
	{
		colorMult = c;
		refresh();
	}
}

void Texture::setColor(const Color& c)
{
	if (colorMult != c)
	{
		colorMult = c;
		refresh();
	}
}

void Texture::setColorDeactivatedFactor(float factorR, float factorG, float factorB, float factorAlpha /* = 1.f */)
{
	Color c(factorR, factorG, factorB, factorAlpha);
	if (colorDisabledMult != c)
	{
		colorDisabledMult = c;
		refresh();
	}
}

void Texture::setColorDeactivatedFactor(const Color& c)
{
	if (colorDisabledMult != c)
	{
		colorDisabledMult = c;
		refresh();
	}
}

void Texture::setTexture(const char* texPath, const char* disTexPath /* = NULL */)
{
	if (texPath || disTexPath)
	{
		bUseFillColor = false;
		this->texturePath.assign(texPath ? texPath : "");
		this->disabledTexturePath.assign(disTexPath ? disTexPath : "");
		refresh();
	}
}

void Texture::fillColor(float r, float g, float b, float a /* = 1.f */)
{
	bUseFillColor = true;
	colorFill = Color(r, g, b, a);
	refresh();
}

void Texture::fillColor(const Color& c)
{
	bUseFillColor = true;
	colorFill = c;
	refresh();
}

Texture::Texture(
	IUIObject* parent,
	float		width,
	float		height,
	const char* texturePath,
	const char* disabledTexturePath)
	: Frame(parent, width, height), colorFill(), bUseFillColor(false)
{
	colorMult = Color(1.f, 1.f, 1.f);
	colorDisabledMult = Color(.5f, .5f, .5f);

	simpleTexture = UISimpleTexture::Create(this->getFrame());
	simpleTexture->setWidth(width);
	simpleTexture->setHeight(height);
	simpleTexture->setRelativePosition(POS_C, this->getFrame(), POS_C, 0, 0);
	setTexture(texturePath, disabledTexturePath);
	//show(true);
}

Texture::Texture(UISimpleFrame* parent, float width, float height, const char* texturePath /*= NULL*/, const char* disabledTexturePath /*= NULL */)
	: Frame(NULL, width, height), colorFill(), bUseFillColor(false)
{
	setParent(parent);
	colorMult = Color(1.f, 1.f, 1.f);
	colorDisabledMult = Color(.5f, .5f, .5f);

	simpleTexture = UISimpleTexture::Create(Frame::getFrame());
	simpleTexture->setWidth(width);
	simpleTexture->setHeight(height);
	simpleTexture->setRelativePosition(POS_C, Frame::getFrame(), POS_C, 0, 0);
	setTexture(texturePath, disabledTexturePath);
	//show(true);
}

Texture::~Texture()
{
	this->texturePath.clear();
	this->disabledTexturePath.clear();
	UISimpleTexture::Destroy(simpleTexture);
}

void Texture::setWidth(float width)
{
	Frame::setWidth(width);
	simpleTexture->setWidth(width);
}

void Texture::setHeight(float height)
{
	Frame::setHeight(height);
	simpleTexture->setHeight(height);
}

void Texture::setSize(float size)
{
	Frame::setSize(size);
	simpleTexture->setWidth(size);
	simpleTexture->setHeight(size);
}

void Texture::setScale(float scale)
{
	Frame::setScale(scale);
	simpleTexture->setScale(scale);
}

void Texture::activate(bool flag)
{
	if (isActive() != flag)
	{
		Frame::activate(flag);
		refresh();
	}
}

void Texture::refresh()
{
	//OutputScreen(1, "Texture::refresh()");
	float w = getShouldWidth();
	simpleTexture->setWidth(w);

	bool active = isActive();
	if (active)
	{
		if (!bUseFillColor && texturePath.length())
		{
			simpleTexture->fillBitmap(texturePath.c_str());
			simpleTexture->setColor(colorMult.toUint32());
		}
		else if (bUseFillColor)
		{
			simpleTexture->fillColor(colorFill.toUint32());
		}
	}
	else
	{
		if (!bUseFillColor && disabledTexturePath.length())
		{
			simpleTexture->fillBitmap(disabledTexturePath.c_str());
			simpleTexture->setColor(colorMult.toUint32());
		}
		else
		{
			if (bUseFillColor)
			{
				simpleTexture->fillColor(colorFill.toUint32());
			}
			simpleTexture->setColor(colorMult.mulRGB(colorDisabledMult).toUint32());
		}
	}
}

Color Texture::getColor() const
{
	return colorMult;
}