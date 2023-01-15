#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Frame.h"

class UISimpleTexture;

class Texture : public Frame
{
public:
	Texture(
		IUIObject* parent,
		float		width,
		float		height,
		const char* texturePath = NULL,
		const char* disabledTexturePath = NULL
	);

	Texture(
		UISimpleFrame* parent,
		float		width,
		float		height,
		const char* texturePath = NULL,
		const char* disabledTexturePath = NULL
	);

	virtual ~Texture();

	virtual void	setWidth(float width);
	virtual void	setHeight(float height);
	virtual void	setSize(float size);
	virtual void	setScale(float scale);

	virtual void	activate(bool flag);
	virtual void	refresh();

	void setColor(float r, float g, float b, float a = 1.f);
	void setColor(const Color& c);
	Color getColor() const;
	void setColorDeactivatedFactor(float factorR, float factorG, float factorB, float factorAlpha = 1.f);
	void setColorDeactivatedFactor(const Color& c);
	void setTexture(const char* texPath, const char* disTexPath = NULL);
	void fillColor(float r, float g, float b, float a = 1.f);
	void fillColor(const Color& c);

private:
	UISimpleTexture* simpleTexture;
	std::string texturePath;
	std::string disabledTexturePath;

	bool bUseFillColor;
	Color colorFill;
	Color colorMult;
	Color colorDisabledMult;
};

#endif