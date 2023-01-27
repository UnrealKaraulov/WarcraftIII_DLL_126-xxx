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


struct RGBAPix
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;

	RGBAPix()
	{
		this->R = 0;
		this->G = 0;
		this->B = 0;
		this->A = 0;
	}

	RGBAPix(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
	{
		this->R = R;
		this->G = G;
		this->B = B;
		this->A = A;
	}

	RGBAPix RGBAPixWar3(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
	{
		this->R = B;
		this->G = G;
		this->B = R;
		this->A = A;
		return *this;
	}


	unsigned int RGBAPixWar3_u(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
	{
		return (unsigned int)(this->A << 24 | this->R << 16 | this->G << 8 | this->B << 0);
	}

	unsigned int ToUINT()
	{
		return (unsigned int)(this->A << 24 | this->R << 16 | this->G << 8 | this->B << 0);
	}

	unsigned int FromUINT(unsigned int color)
	{
		return (unsigned int)(this->A << 24 | this->R << 16 | this->G << 8 | this->B << 0);
	}

	RGBAPix FromString(const char* text)
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

			// ������ �� ����������� ������ ��� ��� ��� ������� ��� FF ��� 0 ������������
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

	RGBAPix operator * (RGBAPix pix)
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
	RGBAPix operator + (RGBAPix pix)
	{
		this->A = FixBounds(this->A + pix.A);
		this->R = FixBounds(this->R + pix.R);
		this->G = FixBounds(this->G + pix.G);
		this->B = FixBounds(this->B + pix.B);

		return (*this);
	}



	RGBAPix operator / (RGBAPix pix)
	{
		if (pix.A > 0 && this->A > 0)
			this->A = FixBounds(255.f / (pix.A / this->A));
		else
			this->A = 255;
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

	RGBAPix operator - (RGBAPix pix)
	{
		this->A = FixBounds(this->A - pix.A);
		this->R = FixBounds(this->R - pix.R);
		this->G = FixBounds(this->G - pix.G);
		this->B = FixBounds(this->B - pix.B);

		return (*this);
	}


};
typedef struct RGBAPix RGBAPix;

struct RGBPix
{
	unsigned char R;
	unsigned char G;
	unsigned char B;

	RGBPix()
	{
		this->R = 0;
		this->G = 0;
		this->B = 0;
	}

	RGBPix(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
	{
		this->R = R;
		this->G = G;
		this->B = B;
	}

	RGBPix RGBPixWar3(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
	{
		this->R = B;
		this->G = G;
		this->B = R;
		return *this;
	}



	unsigned int RGBPixWar3_u(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
	{
		return (unsigned int)(0 << 24 | this->R << 16 | this->G << 8 | this->B << 0);
	}

	unsigned int ToUINT()
	{
		return (unsigned int)(0 << 24 | this->R << 16 | this->G << 8 | this->B << 0);
	}

	RGBAPix ToRGBAPix()
	{
		RGBAPix tmp = RGBAPix();
		tmp.R = this->R;
		tmp.G = this->G;
		tmp.B = this->B;
		tmp.A = 255;
		return tmp;
	}


};
typedef struct RGBPix RGBPix;



struct PAPix
{
	unsigned char i;
	unsigned char A;
};
typedef struct PAPix PAPix;

struct PPix
{
	unsigned char i;
};
typedef struct PPix PPix;
#include <poppack.h>

typedef RGBAPix palette[256];
unsigned char* Scale_WithoutResize(unsigned char* pixels, size_t width, size_t height, size_t newwidth, size_t newheight, size_t bytes_per_pixel);
unsigned long Blp2Raw(StormBuffer input, StormBuffer& output, int& width, int& height, int& bpp, int& mipmaps, int& alphaflag, int& compresstype, int& pictype, char const* filename);
int TGA2Raw(StormBuffer input, StormBuffer& output, int& width, int& height, int& bpp, const char* filename);
int BMP2Raw(StormBuffer input, StormBuffer& output, int& width, int& height, int& bpp, const char* filename);
int JPG2Raw(StormBuffer input, StormBuffer& output, int width, int height, int& bpp, const char* filename);
int CreatePalettedBLP(StormBuffer rawData, StormBuffer& output, int colors, char const* filename, int width, int height, int bytespp, int  alphaflag, int& maxmipmaps);
int RAW2Tga(StormBuffer input, StormBuffer& output, int width, int height, int bpp, const char* filename);
int CreateJpgBLP(StormBuffer rawData, StormBuffer& output, int quality, char const* filename, int width, int height, int bytespp, int  alphaflag, int& maxmipmaps);
void textureInvertRBInPlace(RGBAPix* bufsrc, unsigned long srcsize);
void ScaleImage(unsigned char* rawData, int oldW, int oldH, int newW, int newH, int bytespp, StormBuffer& target);
int ApplyOverlay(unsigned char* rawData, unsigned char* mask, int width, int height, int bytespp, int maskBpp);
int ApplyBorder(unsigned char* rawData, unsigned char* mask, int width, int height, int bytespp, int borderBpp);
void flip_vertically(unsigned char* pixels, size_t width, size_t height, size_t bytes_per_pixel);
void flip_vertically(unsigned char* pixels, float width, float height, size_t bytes_per_pixel);
void flip_vertically(unsigned char* pixels, int width, int height, size_t bytes_per_pixel);
int ArrayXYtoId(int width, int x, int y);
