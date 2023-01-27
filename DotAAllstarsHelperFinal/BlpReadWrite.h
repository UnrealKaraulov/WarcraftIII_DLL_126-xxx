#pragma once
#define XMD_H
//#include "jpgwrapper.h"
#include "Main.h"



enum IconType
{
	Active,
	Passive,
	Autocast
};

enum InfocardIconType
{
	Normal,
	Level
};

enum PictureType
{
	bmp,
	tga,
	jpg,
	blp
};
#include <pshpack1.h>

struct BLPHeader
{
	char     ident[4];       // Always 'BLP1'
	unsigned int    compress;           // 0: JPEG, 1: palette
	unsigned int    IsAlpha;          // 8: Alpha
	unsigned int    sizex;          // In pixels, power-of-two
	unsigned int    sizey;
	unsigned int    alphaEncoding;  // 3, 4: Alpha list, 5: Alpha from palette
	unsigned int    flags2;         // Unused
	unsigned int    poffs[16];
	unsigned int    psize[16];
};


struct TGAHeader {
	char  imageIDLength;
	char  colorMapType;
	char  imageType;
	short int colourmaporigin;
	short int colourmaplength;
	char  colourmapdepth;
	short int x_origin;
	short int y_origin;
	short width;
	short height;
	char  bpp;
	char  imagedescriptor;
};



struct tBGRAPixel
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
};
struct COLOR3
{
	unsigned char R;
	unsigned char G;
	unsigned char B;

	COLOR3()
	{
		this->R = 0;
		this->G = 0;
		this->B = 0;
	}

	COLOR3(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char)
	{
		this->R = _R;
		this->G = _G;
		this->B = _B;
	}
	COLOR3(unsigned char _R, unsigned char _G, unsigned char _B)
	{
		this->R = _R;
		this->G = _G;
		this->B = _B;
	}

	COLOR3 COLOR3War3(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char)
	{
		this->R = _B;
		this->G = _G;
		this->B = _R;
		return *this;
	}

	COLOR3 COLOR3War3(unsigned char _R, unsigned char _G, unsigned char _B)
	{
		this->R = _B;
		this->G = _G;
		this->B = _R;
		return *this;
	}


	unsigned int COLOR3War3_u()
	{
		return (unsigned int)(0xFF << 24 | this->R << 16 | this->G << 8 | this->B << 0);
	}

	unsigned int ToUINT()
	{
		return (unsigned int)(0xFF << 24 | this->R << 16 | this->G << 8 | this->B << 0);
	}

	COLOR3 FromUINT(unsigned int colour)
	{
		this->R = (colour >> 16) & 0xff; // red
		this->G = (colour >> 8) & 0xff; // green
		this->B = colour & 0xff; // blue
		return *this;
	}

	COLOR3 FromString(const char* text)
	{
		if (text && strlen(text) == 8)
		{
			char colorstr[11];
			colorstr[0] = '0';// text[ i + 2 ];
			colorstr[1] = 'x';//text[ i + 3 ];
			//A
			colorstr[2] = text[0];
			colorstr[3] = text[1];
			//R
			colorstr[4] = text[6];
			colorstr[5] = text[7];
			//G
			colorstr[6] = text[4];
			colorstr[7] = text[5];
			//B
			colorstr[8] = text[2];
			colorstr[9] = text[3];
			colorstr[10] = '\0';

			// Смысла от прозрачного текста нет так что считаем что FF это 0 прозрачность
			this->FromUINT(strtoul(colorstr, NULL, 0));
		}
		else
		{
			this->R = 0;
			this->G = 0;
			this->B = 0;
		}
		return *this;
	}

	COLOR3 operator * (const COLOR3& pix)
	{
		if (pix.R > 0 && this->R > 0)
			this->R = FixBounds(255.f / (this->R / (float)pix.R));
		else
			this->R = 0;
		if (pix.G > 0 && this->G > 0)
			this->G = FixBounds(255.f / (this->G / (float)pix.G));
		else
			this->G = 0;
		if (pix.B > 0 && this->B > 0)
			this->B = FixBounds(255.f / (this->B / (float)pix.B));
		else
			this->B = 0;


		return (*this);
	}
	COLOR3 operator + (const COLOR3& pix)
	{
		this->R = FixBounds(this->R + pix.R);
		this->G = FixBounds(this->G + pix.G);
		this->B = FixBounds(this->B + pix.B);

		return (*this);
	}



	COLOR3 operator / (const COLOR3& pix)
	{
		if (pix.R > 0 && this->R > 0)
			this->R = FixBounds(255.f / (pix.R / this->R));
		else
			this->R = 255;
		if (pix.G > 0 && this->G > 0)
			this->G = FixBounds(255.f / (pix.G / this->G));
		else
			this->G = 255;
		if (pix.B > 0 && this->B > 0)
			this->B = FixBounds(255.f / (pix.B / this->B));
		else
			this->B = 255;

		return (*this);
	}

	COLOR3 operator - (const COLOR3 & pix)
	{
		this->R = FixBounds(this->R - pix.R);
		this->G = FixBounds(this->G - pix.G);
		this->B = FixBounds(this->B - pix.B);
		return (*this);
	}
};

struct COLOR4
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;

	COLOR4()
	{
		this->R = 0;
		this->G = 0;
		this->B = 0;
		this->A = 0;
	}

	COLOR4(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
	{
		this->R = _R;
		this->G = _G;
		this->B = _B;
		this->A = _A;
	}

	COLOR4 COLOR4War3(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
	{
		this->R = _B;
		this->G = _G;
		this->B = _R;
		this->A = _A;
		return *this;
	}


	unsigned int COLOR4War3_u()
	{
		return (unsigned int)(this->A << 24 | this->R << 16 | this->G << 8 | this->B << 0);
	}

	unsigned int ToUINT()
	{
		return (unsigned int)(this->A << 24 | this->R << 16 | this->G << 8 | this->B << 0);
	}

	COLOR4 FromUINT(unsigned int colour)
	{
		this->R = (colour >> 16) & 0xff; // red
		this->G = (colour >> 8) & 0xff; // green
		this->B = colour & 0xff; // blue
		this->A = 0xff; // blue
		return *this;
	}

	COLOR4 FromString(const char* text)
	{
		if (text && strlen(text) == 8)
		{
			char colorstr[11];
			colorstr[0] = '0';// text[ i + 2 ];
			colorstr[1] = 'x';//text[ i + 3 ];
			//A
			colorstr[2] = text[0];
			colorstr[3] = text[1];
			//R
			colorstr[4] = text[6];
			colorstr[5] = text[7];
			//G
			colorstr[6] = text[4];
			colorstr[7] = text[5];
			//B
			colorstr[8] = text[2];
			colorstr[9] = text[3];
			colorstr[10] = '\0';

			// Смысла от прозрачного текста нет так что считаем что FF это 0 прозрачность
			this->FromUINT(strtoul(colorstr, NULL, 0));
		}
		else
		{
			this->R = 0;
			this->G = 0;
			this->B = 0;
			this->A = 0;
		}
		return *this;
	}

	COLOR4 operator * (const COLOR4& pix)
	{
		if (pix.A > 0 && this->A > 0)
			this->A = FixBounds(255.f / (this->A / (float)pix.A));
		else
			this->A = 0;
		if (pix.R > 0 && this->R > 0)
			this->R = FixBounds(255.f / (this->R / (float)pix.R));
		else
			this->R = 0;
		if (pix.G > 0 && this->G > 0)
			this->G = FixBounds(255.f / (this->G / (float)pix.G));
		else
			this->G = 0;
		if (pix.B > 0 && this->B > 0)
			this->B = FixBounds(255.f / (this->B / (float)pix.B));
		else
			this->B = 0;


		return (*this);
	}
	COLOR4 operator + (const COLOR4& pix)
	{
		this->A = FixBounds(this->A + pix.A);
		this->R = FixBounds(this->R + pix.R);
		this->G = FixBounds(this->G + pix.G);
		this->B = FixBounds(this->B + pix.B);

		return (*this);
	}



	COLOR4 operator / (const COLOR4& pix)
	{
		if (pix.A > 0 && this->A > 0)
			this->A = (unsigned char)(255.f / (pix.A / this->A));
		else
			this->A = 255;
		if (pix.R > 0 && this->R > 0)
			this->R = (unsigned char)(255.f / (pix.R / this->R));
		else
			this->R = 255;
		if (pix.G > 0 && this->G > 0)
			this->G = (unsigned char)(255.f / (pix.G / this->G));
		else
			this->G = 255;
		if (pix.B > 0 && this->B > 0)
			this->B = (unsigned char)(255.f / (pix.B / this->B));
		else
			this->B = 255;


		return (*this);
	}

	COLOR4 operator - (const COLOR4& pix)
	{
		this->A = FixBounds(this->A - pix.A);
		this->R = FixBounds(this->R - pix.R);
		this->G = FixBounds(this->G - pix.G);
		this->B = FixBounds(this->B - pix.B);

		return (*this);
	}
};
#include <poppack.h>

typedef COLOR4 palette[256];
unsigned char* Scale_WithoutResize(unsigned char* pixels, size_t width, size_t height, size_t newwidth, size_t newheight, size_t bytes_per_pixel);
unsigned long Blp2Raw(StormBuffer input, StormBuffer& output, int& width, int& height, int& bpp, int& mipmaps, int& alphaflag, int& compresstype, int& pictype, char const* filename);
int TGA2Raw(StormBuffer input, StormBuffer& output, int& width, int& height, int& bpp, const char* filename);
int BMP2Raw(StormBuffer input, StormBuffer& output, int& width, int& height, int& bpp, const char* filename);
int JPG2Raw(StormBuffer input, StormBuffer& output, int width, int height, int& bpp, const char* filename);
int CreatePalettedBLP(StormBuffer rawData, StormBuffer& output, int colors, char const* filename, int width, int height, int bytespp, int  alphaflag, int& maxmipmaps);
int RAW2Tga(StormBuffer input, StormBuffer& output, int width, int height, int bpp, const char* filename);
int CreateJpgBLP(StormBuffer rawData, StormBuffer& output, int quality, char const* filename, int width, int height, int bytespp, int  alphaflag, int& maxmipmaps);
void textureInvertRBInPlace(COLOR4* bufsrc, unsigned long srcsize);
void ScaleImage(unsigned char* rawData, int oldW, int oldH, int newW, int newH, int bytespp, StormBuffer& target);
int ApplyOverlay(unsigned char* rawData, unsigned char* mask, int width, int height, int bytespp, int maskBpp);
int ApplyBorder(unsigned char* rawData, unsigned char* mask, int width, int height, int bytespp, int borderBpp);
void flip_vertically(unsigned char* pixels, size_t width, size_t height, size_t bytes_per_pixel);
void flip_vertically(unsigned char* pixels, float width, float height, size_t bytes_per_pixel);
void flip_vertically(unsigned char* pixels, int width, int height, size_t bytes_per_pixel);
int ArrayXYtoId(int width, int x, int y);
