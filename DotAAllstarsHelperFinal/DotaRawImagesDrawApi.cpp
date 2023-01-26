#include "RawImageApi.h"

#define MAX_IMAGES_COUNT 100000

std::vector<RawImageStruct> ListOfRawImages;



double pDistance(int x1, int y1, int x2, int y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int __stdcall ClearRawImage(unsigned int RawImage, RGBAPix FillByte)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];
#ifdef OLD_CODE
	RGBAPix* RawImageData = (RGBAPix*)tmpRawImage.img.buf;
	for (unsigned int i = 0; i < tmpRawImage.width * tmpRawImage.height; i++)
	{
		RawImageData[i] = FillByte;
	}
#else 

#endif
	return 1;
}

int __stdcall BackupRawImage(unsigned int RawImage)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];

	if (tmpRawImage.backup_img)
	{
		delete[] tmpRawImage.backup_img;
	}

#ifdef OLD_CODE
	tmpRawImage.backup_img = new char[tmpRawImage.img.length];
	std::memcpy(tmpRawImage.backup_img, tmpRawImage.img.buf, tmpRawImage.img.length);
#else 

#endif
	tmpRawImage.backup_width = tmpRawImage.width;
	tmpRawImage.backup_height = tmpRawImage.height;

	return true;
}

int __stdcall RestoreRawImage(unsigned int RawImage)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];

	if (!tmpRawImage.backup_img)
		return false;

#ifdef OLD_CODE
	tmpRawImage.img.Clear();
	tmpRawImage.img.Resize(tmpRawImage.backup_width * tmpRawImage.backup_height * 4);
	std::memcpy(tmpRawImage.img.buf, tmpRawImage.backup_img, tmpRawImage.backup_width * tmpRawImage.backup_height * 4);
#else

#endif

	return true;
}

// Создает RawImage (RGBA) с указанным цветом
int __stdcall CreateRawImage(int width, int height, RGBAPix defaultcolor)
{
	if (!InitFunctionCalled)
		return 0;

	int resultid = ListOfRawImages.size();

	width = width + (width % 2);
	height = height + (height % 2);

	RawImageStruct tmpRawImage = RawImageStruct();
#ifdef OLD_CODE
	StormBuffer tmpRawImageBuffer = StormBuffer();
	tmpRawImageBuffer.Resize(width * height * 4);
	for (int i = 0; i < width * height; i++)
	{
		*(RGBAPix*)&tmpRawImageBuffer[i * 4] = defaultcolor;
	}

	tmpRawImage.img = tmpRawImageBuffer;

#else 


#endif


	tmpRawImage.width = width;
	tmpRawImage.height = height;
	tmpRawImage.filename = std::string();
	tmpRawImage.RawImage = resultid;
	tmpRawImage.Flipped = false;
	tmpRawImage.needResetTexture = true;

	ListOfRawImages.push_back(tmpRawImage);

	return resultid;
}

// Загружает RawImage из filename (tga,blp)
int __stdcall LoadRawImage(const char* filename)
{
	if (!InitFunctionCalled || !filename || filename[0] == '\0')
		return 0;
	int resultid = ListOfRawImages.size();
	int filenamelen = strlen(filename);


	int PatchFileData = 0;
	size_t PatchFileSize = 0;
	GameGetFile_org(filename, &PatchFileData, &PatchFileSize, true);
	if (PatchFileData == 0 || PatchFileSize == 0)
	{
		//PrintText( ( "|cFFFF0000RawImages: File:" + std::string( filename ) + " not found in game! Try another search methods." ).c_str( ) );
		//MessageBoxA( 0, ( "RawImages: File:" + std::string( filename ) + " not found in game! Try another search methods." ).c_str( ), "READ ERROR", 0 );
		GameGetFile_org(filename, &PatchFileData, &PatchFileSize, true);
		if (PatchFileData == 0 || PatchFileSize == 0)
		{
			GameGetFile_org((filename + std::string(".tga")).c_str(), &PatchFileData, &PatchFileSize, true);
			if (PatchFileData == 0 || PatchFileSize == 0)
			{
				GameGetFile_org((filename + std::string(".blp")).c_str(), &PatchFileData, &PatchFileSize, true);
				if (PatchFileData == 0 || PatchFileSize == 0)
				{
					if (filenamelen > 4)
					{
						char* tmpfilename = new char[filenamelen];
						memset(tmpfilename, 0, filenamelen);
						std::memcpy(tmpfilename, filename, filenamelen - 4);
						GameGetFile_org((tmpfilename + std::string(".blp")).c_str(), &PatchFileData, &PatchFileSize, true);
						if (PatchFileData == 0 || PatchFileSize == 0)
						{
							GameGetFile_org((tmpfilename + std::string(".tga")).c_str(), &PatchFileData, &PatchFileSize, true);
							if (PatchFileData == 0 || PatchFileSize == 0)
							{
								PrintText(("|cFFFF0000RawImages: File:" + std::string(filename) + " not found in game! Try another search methods.").c_str());
							}
						}

						delete[] tmpfilename;
					}

				}
			}
		}
	}
	if (PatchFileData != 0 || PatchFileSize != 0)
	{
		int IsBlp = memcmp((LPCVOID)PatchFileData, "BLP1", 4) == 0;
		int w = 0, h = 0, bpp = 0, mipmaps = 0, alphaflag = 8, compress = 1, alphaenconding = 5;
		unsigned long rawImageSize = 0;

		StormBuffer OutBuffer = StormBuffer();
		StormBuffer InBuffer((char*)PatchFileData, PatchFileSize);

		if (!IsBlp)
			rawImageSize = (unsigned long)TGA2Raw(InBuffer, OutBuffer, w, h, bpp, filename);
		else
			rawImageSize = Blp2Raw(InBuffer, OutBuffer, w, h, bpp, mipmaps, alphaflag, compress, alphaenconding, filename);

		if (rawImageSize > 0)
		{
			RawImageStruct tmpRawImage = RawImageStruct();
#ifdef OLD_CODE
			tmpRawImage.img = OutBuffer;
#else 

#endif
			tmpRawImage.width = w;
			tmpRawImage.height = h;
			tmpRawImage.filename = filename;
			tmpRawImage.RawImage = resultid;
			tmpRawImage.Flipped = false;

			ListOfRawImages.push_back(tmpRawImage);
		}
		else
		{
			PrintText(("|cFFFF0000Error load RawImage file:" + std::string(filename) + ". DECODE ERROR").c_str());
			IsBlp = !IsBlp;
			if (!IsBlp)
				rawImageSize = (unsigned long)TGA2Raw(InBuffer, OutBuffer, w, h, bpp, filename);
			else
				rawImageSize = Blp2Raw(InBuffer, OutBuffer, w, h, bpp, mipmaps, alphaflag, compress, alphaenconding, filename);


			if (rawImageSize > 0)
			{
				RawImageStruct tmpRawImage = RawImageStruct();

#ifdef OLD_CODE
				tmpRawImage.img = OutBuffer;
#else 

#endif
				tmpRawImage.width = w;
				tmpRawImage.height = h;
				tmpRawImage.filename = filename;
				tmpRawImage.RawImage = resultid;
				tmpRawImage.Flipped = false;

				ListOfRawImages.push_back(tmpRawImage);

				PrintText("FU##! Error filetype :O");
				//		MessageBoxA( 0, "FU##! Error filetype :O", "FU##! Error filetype :O", 0 );
			}
		}

	}
	else return 0;

	return resultid;
}


enum BlendModes : int
{
	BlendNormal,
	BlendAdd,
	BlendSubtract,
	BlendMultiple

};

int AutoFixImagesSize = true;

int __stdcall RawImage_EnableAutoFix(int enable)
{
	AutoFixImagesSize = enable;
	return 0;
}

// Рисует RawImage2 на RawImage
int __stdcall RawImage_DrawImg(unsigned int RawImage, unsigned int RawImage2, int drawx, int drawy, int blendmode)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	if (RawImage2 >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];
	RawImageStruct& tmpRawImage2 = ListOfRawImages[RawImage2];

#ifdef OLD_CODE
	RGBAPix* RawImageData = (RGBAPix*)tmpRawImage.img.buf;
	RGBAPix* RawImageData2 = (RGBAPix*)tmpRawImage2.img.buf;

	for (unsigned int x = drawx, x2 = 0; x < tmpRawImage.width && x2 < tmpRawImage2.width; x++, x2++)
	{
		for (unsigned int y = drawy, y2 = 0; y < tmpRawImage.height && y2 < tmpRawImage2.height; y++, y2++)
		{
			if (blendmode == BlendModes::BlendNormal)
				RawImageData[ArrayXYtoId(tmpRawImage.width, x, y)] = RawImageData2[ArrayXYtoId(tmpRawImage2.width, x2, y2)];
			else if (blendmode == BlendModes::BlendAdd)
				RawImageData[ArrayXYtoId(tmpRawImage.width, x, y)] =
				RawImageData[ArrayXYtoId(tmpRawImage.width, x, y)] + RawImageData2[ArrayXYtoId(tmpRawImage2.width, x2, y2)];
			else if (blendmode == BlendModes::BlendSubtract)
				RawImageData[ArrayXYtoId(tmpRawImage.width, x, y)] =
				RawImageData[ArrayXYtoId(tmpRawImage.width, x, y)] - RawImageData2[ArrayXYtoId(tmpRawImage2.width, x2, y2)];
			else if (blendmode == BlendModes::BlendMultiple)
				RawImageData[ArrayXYtoId(tmpRawImage.width, x, y)] =
				RawImageData[ArrayXYtoId(tmpRawImage.width, x, y)] * RawImageData2[ArrayXYtoId(tmpRawImage2.width, x2, y2)];
			else
				RawImageData[ArrayXYtoId(tmpRawImage.width, x, y)] =
				RawImageData[ArrayXYtoId(tmpRawImage.width, x, y)] / RawImageData2[ArrayXYtoId(tmpRawImage2.width, x2, y2)];
		}
	}
#else 


#endif
	//	if ( tmpRawImage.used_for_overlay )
	//	{
	tmpRawImage.needResetTexture = true;
	//	}
	return true;
}

// Заполняет выбранный пиксель указанным цветом
int __stdcall RawImage_DrawPixel(unsigned int RawImage, unsigned int x, unsigned int y, RGBAPix color)//RGBAPix = unsigned int
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}


	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];
#ifdef OLD_CODE
	RGBAPix* RawImageData = (RGBAPix*)tmpRawImage.img.buf;
	if (x >= 0 && y >= 0 && x < tmpRawImage.width && y < tmpRawImage.height)
	{
		RawImageData[ArrayXYtoId(tmpRawImage.width, x, y)] = color;
	}
#else 

#endif
	//	if ( tmpRawImage.used_for_overlay )
	//	{
	tmpRawImage.needResetTexture = true;
	//	}

	return true;
}


int __stdcall RawImage_FillRectangle(unsigned int RawImage, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, RGBAPix color)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}
	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];
#ifdef OLD_CODE
	RGBAPix* RawImageData = (RGBAPix*)tmpRawImage.img.buf;

	for (unsigned int xsize = 0; xsize < x2; xsize++)
	{
		for (unsigned int ysize = 0; ysize < y2; ysize++)
		{
			RawImage_DrawPixel(RawImage, x1 + xsize, y1 + ysize, color);
		}
	}
#else 

#endif
	//	if ( tmpRawImage.used_for_overlay )
	//	{
	tmpRawImage.needResetTexture = true;
	//	}

	return true;
}


// Рисует прямоугольник с указанным цветом и размером
int __stdcall RawImage_DrawRect(unsigned int RawImage, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int size, RGBAPix color)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

#ifdef OLD_CODE
	// fixme
	for (unsigned int xsize = 0; xsize < x2; xsize++)
	{
		for (unsigned int ysize = 0; ysize < y2; ysize++)
		{
			RawImage_DrawPixel(RawImage, x1 + xsize, y1 + ysize, color);
		}
	}
#else 

#endif
	return true;
}

#pragma region DrawLineAlgorithm

/*
*
* @date 25.03.2013
* @author Armin Joachimsmeyer
* https://github.com/ArminJo/STMF3-Discovery-Demos/blob/master/lib/graphics/src/thickLine.cpp
*
*/

#define LINE_OVERLAP_NONE 0 	// No line overlap, like in standard Bresenham
#define LINE_OVERLAP_MAJOR 0x01 // Overlap - first go major then minor direction. Pixel is drawn as extension after actual line
#define LINE_OVERLAP_MINOR 0x02 // Overlap - first go minor then major direction. Pixel is drawn as extension before next line
#define LINE_OVERLAP_BOTH 0x03  // Overlap - both

#define LINE_THICKNESS_MIDDLE 0                 // Start point is on the line at center of the thick line
#define LINE_THICKNESS_DRAW_CLOCKWISE 1         // Start point is on the counter clockwise border line
#define LINE_THICKNESS_DRAW_COUNTERCLOCKWISE 2  // Start point is on the clockwise border line

void drawLineOverlap(unsigned int RawImage, int aXStart, int aYStart, int aXEnd, int aYEnd, uint8_t aOverlap,
	RGBAPix aColor) {
	int16_t tDeltaX, tDeltaY, tDeltaXTimes2, tDeltaYTimes2, tError, tStepX, tStepY;
	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];
	int maxwidth = tmpRawImage.width;
	int maxheight = tmpRawImage.height;
	/*
	* Clip to display size
	*/
	if (aXStart >= maxwidth) {
		aXStart = maxwidth - 1;
	}
	if (aXStart < 0) {
		aXStart = 0;
	}
	if (aXEnd >= maxwidth) {
		aXEnd = maxwidth - 1;
	}
	if (aXEnd < 0) {
		aXEnd = 0;
	}
	if (aYStart >= maxheight) {
		aYStart = maxheight - 1;
	}
	if (aYStart < 0) {
		aYStart = 0;
	}
	if (aYEnd >= maxheight) {
		aYEnd = maxheight - 1;
	}
	if (aYEnd < 0) {
		aYEnd = 0;
	}

	if ((aXStart == aXEnd) || (aYStart == aYEnd)) {
		//horizontal or vertical line -> fillRect() is faster

		if (aXEnd >= aXStart && aYEnd >= aYStart)
		{
			RawImage_FillRectangle(RawImage, aXStart, aYStart, aXEnd - aXStart + 1, aYEnd - aYStart + 1, aColor);
		}
		else if (aXEnd >= aXStart)
		{
			RawImage_FillRectangle(RawImage, aXStart, aYEnd, aXEnd - aXStart + 1, aYStart - aYEnd + 1, aColor);
		}
		else if (aYEnd >= aYStart)
		{
			RawImage_FillRectangle(RawImage, aXEnd, aYStart, aXStart - aXEnd + 1, aYEnd - aYStart + 1, aColor);
		}
	}
	else {
		//calculate direction
		tDeltaX = aXEnd - aXStart;
		tDeltaY = aYEnd - aYStart;
		if (tDeltaX < 0) {
			tDeltaX = -tDeltaX;
			tStepX = -1;
		}
		else {
			tStepX = +1;
		}
		if (tDeltaY < 0) {
			tDeltaY = -tDeltaY;
			tStepY = -1;
		}
		else {
			tStepY = +1;
		}
		tDeltaXTimes2 = tDeltaX << 1;
		tDeltaYTimes2 = tDeltaY << 1;
		//draw start pixel
		RawImage_DrawPixel(RawImage, aXStart, aYStart, aColor);
		if (tDeltaX > tDeltaY) {
			// start value represents a half step in Y direction
			tError = tDeltaYTimes2 - tDeltaX;
			while (aXStart != aXEnd) {
				// step in main direction
				aXStart += tStepX;
				if (tError >= 0) {
					if (aOverlap & LINE_OVERLAP_MAJOR) {
						// draw pixel in main direction before changing
						RawImage_DrawPixel(RawImage, aXStart, aYStart, aColor);
					}
					// change Y
					aYStart += tStepY;
					if (aOverlap & LINE_OVERLAP_MINOR) {
						// draw pixel in minor direction before changing
						RawImage_DrawPixel(RawImage, aXStart - tStepX, aYStart, aColor);
					}
					tError -= tDeltaXTimes2;
				}
				tError += tDeltaYTimes2;
				RawImage_DrawPixel(RawImage, aXStart, aYStart, aColor);
			}
		}
		else {
			tError = tDeltaXTimes2 - tDeltaY;
			while (aYStart != aYEnd) {
				aYStart += tStepY;
				if (tError >= 0) {
					if (aOverlap & LINE_OVERLAP_MAJOR) {
						// draw pixel in main direction before changing
						RawImage_DrawPixel(RawImage, aXStart, aYStart, aColor);
					}
					aXStart += tStepX;
					if (aOverlap & LINE_OVERLAP_MINOR) {
						// draw pixel in minor direction before changing
						RawImage_DrawPixel(RawImage, aXStart, aYStart - tStepY, aColor);
					}
					tError -= tDeltaYTimes2;
				}
				tError += tDeltaXTimes2;
				RawImage_DrawPixel(RawImage, aXStart, aYStart, aColor);
			}
		}
	}
}

/**
* Bresenham with thickness
* no pixel missed and every pixel only drawn once!
*/
void drawThickLine(unsigned int RawImage, int aXStart, int aYStart, int aXEnd, int aYEnd, int aThickness,
	uint8_t aThicknessMode, RGBAPix aColor) {
	int16_t i, tDeltaX, tDeltaY, tDeltaXTimes2, tDeltaYTimes2, tError, tStepX, tStepY;
	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];
	int maxwidth = tmpRawImage.width;
	int maxheight = tmpRawImage.height;

	if (aThickness <= 1) {
		drawLineOverlap(RawImage, aXStart, aYStart, aXEnd, aYEnd, LINE_OVERLAP_NONE, aColor);
	}
	/*
	* Clip to display size
	*/
	if (aXStart >= maxwidth) {
		aXStart = maxwidth - 1;
	}
	if (aXStart < 0) {
		aXStart = 0;
	}
	if (aXEnd >= maxwidth) {
		aXEnd = maxwidth - 1;
	}
	if (aXEnd < 0) {
		aXEnd = 0;
	}
	if (aYStart >= maxheight) {
		aYStart = maxheight - 1;
	}
	if (aYStart < 0) {
		aYStart = 0;
	}
	if (aYEnd >= maxheight) {
		aYEnd = maxheight - 1;
	}
	if (aYEnd < 0) {
		aYEnd = 0;
	}

	/**
	* For coordinate system with 0.0 top left
	* Swap X and Y delta and calculate clockwise (new delta X inverted)
	* or counterclockwise (new delta Y inverted) rectangular direction.
	* The right rectangular direction for LINE_OVERLAP_MAJOR toggles with each octant
	*/
	tDeltaY = aXEnd - aXStart;
	tDeltaX = aYEnd - aYStart;
	// mirror 4 quadrants to one and adjust deltas and stepping direction
	int tSwap = true; // count effective mirroring
	if (tDeltaX < 0) {
		tDeltaX = -tDeltaX;
		tStepX = -1;
		tSwap = !tSwap;
	}
	else {
		tStepX = +1;
	}
	if (tDeltaY < 0) {
		tDeltaY = -tDeltaY;
		tStepY = -1;
		tSwap = !tSwap;
	}
	else {
		tStepY = +1;
	}
	tDeltaXTimes2 = tDeltaX << 1;
	tDeltaYTimes2 = tDeltaY << 1;
	int tOverlap;
	// adjust for right direction of thickness from line origin
	int tDrawStartAdjustCount = aThickness / 2;
	if (aThicknessMode == LINE_THICKNESS_DRAW_COUNTERCLOCKWISE) {
		tDrawStartAdjustCount = aThickness - 1;
	}
	else if (aThicknessMode == LINE_THICKNESS_DRAW_CLOCKWISE) {
		tDrawStartAdjustCount = 0;
	}

	// which octant are we now
	if (tDeltaX >= tDeltaY) {
		if (tSwap) {
			tDrawStartAdjustCount = (aThickness - 1) - tDrawStartAdjustCount;
			tStepY = -tStepY;
		}
		else {
			tStepX = -tStepX;
		}
		/*
		* Vector for draw direction of lines is rectangular and counterclockwise to original line
		* Therefore no pixel will be missed if LINE_OVERLAP_MAJOR is used
		* on changing in minor rectangular direction
		*/
		// adjust draw start point
		tError = tDeltaYTimes2 - tDeltaX;
		for (i = tDrawStartAdjustCount; i > 0; i--) {
			// change X (main direction here)
			aXStart -= tStepX;
			aXEnd -= tStepX;
			if (tError >= 0) {
				// change Y
				aYStart -= tStepY;
				aYEnd -= tStepY;
				tError -= tDeltaXTimes2;
			}
			tError += tDeltaYTimes2;
		}
		//draw start line
		drawLineOverlap(RawImage, aXStart, aYStart, aXEnd, aYEnd, LINE_OVERLAP_NONE, aColor);
		// draw aThickness lines
		tError = tDeltaYTimes2 - tDeltaX;
		for (i = aThickness; i > 1; i--) {
			// change X (main direction here)
			aXStart += tStepX;
			aXEnd += tStepX;
			tOverlap = LINE_OVERLAP_NONE;
			if (tError >= 0) {
				// change Y
				aYStart += tStepY;
				aYEnd += tStepY;
				tError -= tDeltaXTimes2;
				/*
				* change in minor direction reverse to line (main) direction
				* because of choosing the right (counter)clockwise draw std::vector
				* use LINE_OVERLAP_MAJOR to fill all pixel
				*
				* EXAMPLE:
				* 1,2 = Pixel of first lines
				* 3 = Pixel of third line in normal line mode
				* - = Pixel which will additionally be drawn in LINE_OVERLAP_MAJOR mode
				*           33
				*       3333-22
				*   3333-222211
				* 33-22221111
				*  221111                     /\
				*  11                          Main direction of draw std::vector
				*  -> Line main direction
				*  <- Minor direction of counterclockwise draw std::vector
				*/
				tOverlap = LINE_OVERLAP_MAJOR;
			}
			tError += tDeltaYTimes2;
			drawLineOverlap(RawImage, aXStart, aYStart, aXEnd, aYEnd, tOverlap, aColor);
		}
	}
	else {
		// the other octant
		if (tSwap) {
			tStepX = -tStepX;
		}
		else {
			tDrawStartAdjustCount = (aThickness - 1) - tDrawStartAdjustCount;
			tStepY = -tStepY;
		}
		// adjust draw start point
		tError = tDeltaXTimes2 - tDeltaY;
		for (i = tDrawStartAdjustCount; i > 0; i--) {
			aYStart -= tStepY;
			aYEnd -= tStepY;
			if (tError >= 0) {
				aXStart -= tStepX;
				aXEnd -= tStepX;
				tError -= tDeltaYTimes2;
			}
			tError += tDeltaXTimes2;
		}
		//draw start line
		drawLineOverlap(RawImage, aXStart, aYStart, aXEnd, aYEnd, LINE_OVERLAP_NONE, aColor);
		tError = tDeltaXTimes2 - tDeltaY;
		for (i = aThickness; i > 1; i--) {
			aYStart += tStepY;
			aYEnd += tStepY;
			tOverlap = LINE_OVERLAP_NONE;
			if (tError >= 0) {
				aXStart += tStepX;
				aXEnd += tStepX;
				tError -= tDeltaYTimes2;
				tOverlap = LINE_OVERLAP_MAJOR;
			}
			tError += tDeltaXTimes2;
			drawLineOverlap(RawImage, aXStart, aYStart, aXEnd, aYEnd, tOverlap, aColor);
		}
	}
}

#pragma endregion

// Рисует линию с указанным цветом и размером
int __stdcall RawImage_DrawLine(unsigned int RawImage, unsigned int x1, unsigned int y1, unsigned int x2
	, unsigned int y2, unsigned int size, RGBAPix color)
{
	if (!InitFunctionCalled)
		return 0;

	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}
#ifdef OLD_CODE
	drawThickLine(RawImage, x1, y1, x2, y2, size, 0, color);
#else 

#endif

	return true;
}

// Рисует круг с указанным радиусом и толщиной
int __stdcall RawImage_DrawCircle(unsigned int RawImage, unsigned int x, unsigned int y, unsigned int radius,
	unsigned int size, RGBAPix color)
{
	if (!InitFunctionCalled)
		return 0;

	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	size /= 2;
	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];

#ifdef OLD_CODE
	RGBAPix* RawImageData = (RGBAPix*)tmpRawImage.img.buf;
	for (unsigned int x2 = 0; x2 < tmpRawImage.width; x2++)
	{
		for (unsigned int y2 = 0; y2 < tmpRawImage.height; y2++)
		{
			double dist = pDistance(x, y, x2, y2);
			if (pDistance(x, y, x2, y2) >= radius - size && pDistance(x, y, x2, y2) <= radius + size)
			{
				RawImageData[ArrayXYtoId(tmpRawImage.width, x2, y2)] = color;
			}

		}

	}
#else 

#endif

	//	if ( tmpRawImage.used_for_overlay )
	//	{
	tmpRawImage.needResetTexture = true;
	//	}
	return true;
}




// Заполняет круг указанным цветом
int __stdcall RawImage_FillCircle(unsigned int RawImage, unsigned int x, unsigned int y, unsigned int radius, RGBAPix color)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}
	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];


#ifdef OLD_CODE
	RGBAPix* RawImageData = (RGBAPix*)tmpRawImage.img.buf;



	for (unsigned int x2 = 0; x2 < tmpRawImage.width; x2++)
	{
		for (unsigned int y2 = 0; y2 < tmpRawImage.height; y2++)
		{
			if (pDistance(x, y, x2, y2) <= radius)
			{
				RawImageData[ArrayXYtoId(tmpRawImage.width, x2, y2)] = color;
			}
		}
	}

#else 

#endif

	//	if ( tmpRawImage.used_for_overlay )
	//	{
	tmpRawImage.needResetTexture = true;
	//	}
	return true;
}

// Оставляет только круг с указанным радиусом
int __stdcall RawImage_EraseCircle(unsigned int RawImage, unsigned int x, unsigned int y, unsigned int radius, int inverse)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}
	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];

	RGBAPix tmpPix = RGBAPix();

	if (!inverse)
	{
		return RawImage_FillCircle(RawImage, x, y, radius, tmpPix);
	}

#ifdef OLD_CODE

	RGBAPix* RawImageData = (RGBAPix*)tmpRawImage.img.buf;

	for (unsigned int x2 = 0; x2 < tmpRawImage.width; x2++)
	{
		for (unsigned int y2 = 0; y2 < tmpRawImage.height; y2++)
		{
			if (pDistance(x, y, x2, y2) > radius)
			{
				RawImageData[ArrayXYtoId(tmpRawImage.width, x2, y2)] = tmpPix;
			}
		}
	}
#else 

#endif
	//	if ( tmpRawImage.used_for_overlay )
	//	{
	tmpRawImage.needResetTexture = true;
	//	}

	return true;
}

// Делает пиксели с цветом color - прозрачными, power от 0 до 255
int __stdcall RawImage_EraseColor(unsigned int RawImage, RGBAPix color, int power)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];

#ifdef OLD_CODE
	RGBAPix* RawImageData = (RGBAPix*)tmpRawImage.img.buf;
#else 

#endif
	RGBAPix tmpPix = RGBAPix();
	unsigned char// A = color.A,
		R = color.R,
		G = color.G,
		B = color.B;

	for (unsigned int x2 = 0; x2 < tmpRawImage.width; x2++)
	{
		for (unsigned int y2 = 0; y2 < tmpRawImage.height; y2++)
		{
			unsigned char// A2 = RawImageData[ ArrayXYtoId( tmpRawImage.width, x2, y2 ) ].A,
				R2 = RawImageData[ArrayXYtoId(tmpRawImage.width, x2, y2)].R,
				G2 = RawImageData[ArrayXYtoId(tmpRawImage.width, x2, y2)].G,
				B2 = RawImageData[ArrayXYtoId(tmpRawImage.width, x2, y2)].B;

			if ( //( A >= A2 - power && A <= A2 + power ) &&
				(R >= R2 - power && R <= R2 + power) &&
				(G >= G2 - power && G <= G2 + power) &&
				(B >= B2 - power && B <= B2 + power))
			{
				RawImageData[ArrayXYtoId(tmpRawImage.width, x2, y2)] = tmpPix;
			}
		}
	}
	//if ( tmpRawImage.used_for_overlay )
	//{
	tmpRawImage.needResetTexture = true;
	//	}

	return true;
}

const char* _fontname = "Arial";
int _fontsize = 20;
unsigned int _flags = 0;
// 0x1 = BOLD


// Устанавливает настройки шрифта для RawImage_DrawText
int __stdcall RawImage_LoadFontFromResource(const char* filepath)
{
	if (!InitFunctionCalled)
		return 0;

	int PatchFileData = 0;
	size_t PatchFileSize = 0;
	GameGetFile_ptr(filepath, &PatchFileData, &PatchFileSize, true);
	unsigned long Font = NULL;//Globals, this is the Font in the RAM
	AddFontMemResourceEx((void*)PatchFileData, PatchFileSize, NULL, &Font);

	return true;
}


// Устанавливает настройки шрифта для RawImage_DrawText
int __stdcall RawImage_SetFontSettings(const char* fontname, int fontsize, unsigned int flags)
{
	_fontname = fontname;
	_fontsize = fontsize;
	_flags = flags;
	return true;
}

// Пишет текст в указанных координатах с указанными цветом и настройками шрифта RawImage_SetFontSettings
int __stdcall RawImage_DrawText(unsigned int RawImage, const char* text, unsigned int x, unsigned int y, RGBAPix color)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];

#ifdef OLD_CODE

	RGBAPix* RawImageData = (RGBAPix*)tmpRawImage.img.buf;
	HDC hDC = CreateCompatibleDC(NULL);
	char* pSrcData = 0;
	BITMAPINFO bmi = { sizeof(BITMAPINFOHEADER), (LONG)tmpRawImage.width,  (LONG)tmpRawImage.height, 1, 24, BI_RGB, 0, 0, 0, 0, 0 };
	HBITMAP hTempBmp = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void**)&pSrcData, NULL, 0);
	if (!hTempBmp)
	{
		DeleteDC(hDC);
		return false;
	}
	RECT rect = RECT();
	rect.left = x;
	rect.top = y;
	rect.bottom = tmpRawImage.height;
	rect.right = tmpRawImage.width;

	HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hTempBmp);
	HFONT NewFont = CreateFontA(_fontsize, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _fontname);
	HFONT TempFont = NULL;
	unsigned int textcolor = color.ToUINT();
	unsigned int oldcolor = color.ToUINT();


	RGBAPix tmpPix = RGBAPix();


	SetBkColor(hDC, 0x00000000);
	SetBkMode(hDC, TRANSPARENT);

	SelectObject(hDC, NewFont);
	SetTextColor(hDC, color.ToUINT());

	int len = strlen(text);
	int boldenabled = false;
	int italicenabled = false;
	int underlineenabled = false;
	int strikeoutenabled = false;

	int newline = false;
	for (int i = 0; i < len; )
	{
		if (len - i > 1)
		{
			if (text[i] == '|' && (text[i + 1] == 'n' || text[i + 1] == 'N'))
			{
				i += 2;
				newline = true;
				continue;
			}
			else if (text[i] == '|' && (text[i + 1] == 'b' || text[i + 1] == 'B'))
			{
				i += 2;
				boldenabled = true;
				TempFont = CreateFontA(_fontsize, 0, 0, 0, boldenabled ? FW_BOLD : 0, italicenabled, underlineenabled, strikeoutenabled, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _fontname);
				SelectObject(hDC, TempFont);
				DeleteObject(NewFont);
				NewFont = TempFont;
				TempFont = NULL;
				continue;
			}
			else if (text[i] == '|' && (text[i + 1] == 'u' || text[i + 1] == 'U'))
			{
				i += 2;
				underlineenabled = true;
				TempFont = CreateFontA(_fontsize, 0, 0, 0, boldenabled ? FW_BOLD : 0, italicenabled, underlineenabled, strikeoutenabled, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _fontname);
				SelectObject(hDC, TempFont);
				DeleteObject(NewFont);
				NewFont = TempFont;
				TempFont = NULL;
				continue;
			}
			else if (text[i] == '|' && (text[i + 1] == 's' || text[i + 1] == 'S'))
			{
				i += 2;
				strikeoutenabled = true;
				TempFont = CreateFontA(_fontsize, 0, 0, 0, boldenabled ? FW_BOLD : 0, italicenabled, underlineenabled, strikeoutenabled, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _fontname);
				SelectObject(hDC, TempFont);
				DeleteObject(NewFont);
				NewFont = TempFont;
				TempFont = NULL;
				continue;
			}
			else if (text[i] == '|' && (text[i + 1] == 'i' || text[i + 1] == 'I'))
			{
				i += 2;
				italicenabled = true;
				TempFont = CreateFontA(_fontsize, 0, 0, 0, boldenabled ? FW_BOLD : 0, italicenabled, underlineenabled, strikeoutenabled, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _fontname);
				SelectObject(hDC, TempFont);
				DeleteObject(NewFont);
				NewFont = TempFont;
				TempFont = NULL;
				continue;
			}
			else if (text[i] == '|' && (text[i + 1] == 'r' || text[i + 1] == 'R'))
			{
				i += 2;
				textcolor = oldcolor;
				SetTextColor(hDC, color.ToUINT());
				TempFont = CreateFontA(_fontsize, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _fontname);
				SelectObject(hDC, TempFont);
				DeleteObject(NewFont);
				NewFont = TempFont;
				TempFont = NULL;
				boldenabled = false;
				italicenabled = false;
				underlineenabled = false;
				strikeoutenabled = false;
				continue;
			}
			else if (text[i] == '|' && (text[i + 1] == 'c' || text[i + 1] == 'C'))
			{
				oldcolor = textcolor;
				i += 2;
				if (len - i > 7)
				{
					char colorstr[11];
					colorstr[0] = '0';// text[ i + 2 ];
					colorstr[1] = 'x';//text[ i + 3 ];
					//A
					colorstr[2] = text[i];
					colorstr[3] = text[i + 1];
					//R
					colorstr[4] = text[i + 6];
					colorstr[5] = text[i + 7];
					//G
					colorstr[6] = text[i + 4];
					colorstr[7] = text[i + 5];
					//B
					colorstr[8] = text[i + 2];
					colorstr[9] = text[i + 3];
					colorstr[10] = '\0';

					// Смысла от прозрачного текста нет так что считаем что FF это 0 прозрачность
					textcolor = strtoul(colorstr, NULL, 0);
					if ((textcolor & 0xFF000000) == 0xFF000000)
						textcolor -= 0xFF000000;

					SetTextColor(hDC, textcolor);
					i += 8;
				}
				continue;
			}
		}

		std::ostringstream strfordraw;

		for (; i < len; i++)
		{
			if (text[i] != '|' || len - i < 2)
				strfordraw << text[i];
			else if (text[i] == '|')
			{
				break;
			}
		}

		if (strfordraw.str().length() > 0)
		{
			RECT newsize = { 0,0,0,0 };
			DrawTextA(hDC, strfordraw.str().c_str(), -1, &newsize, DT_CALCRECT);
			if (newline)
			{
				newline = false;
				rect.left = x;
				rect.top += newsize.top + newsize.bottom;
			}
			DrawTextA(hDC, strfordraw.str().c_str(), -1, &rect, DT_LEFT | DT_SINGLELINE);
			rect.left += newsize.right - newsize.left;
			strfordraw.clear();
		}
	}

	DeleteObject(NewFont);

	SelectObject(hDC, hBmpOld);
	GdiFlush();
	ReleaseDC(NULL, hDC);

	RGBPix* tmpBitmapPixList = (RGBPix*)pSrcData;


	for (unsigned int x0 = 0; x0 < tmpRawImage.width; x0++)
	{
		for (unsigned int y0 = 0; y0 < tmpRawImage.height; y0++)
		{
			if (tmpBitmapPixList[ArrayXYtoId(tmpRawImage.width, x0, y0)].ToUINT() != 0)
			{
				RawImageData[ArrayXYtoId(tmpRawImage.width, x0, y0)] = tmpBitmapPixList[ArrayXYtoId(tmpRawImage.width, x0, y0)].ToRGBAPix();
			}
		}
	}

	DeleteDC(hDC);
	DeleteObject(hBmpOld);
	DeleteObject(hTempBmp);
#else 

#endif
	//if ( tmpRawImage.used_for_overlay )
//	{
	tmpRawImage.needResetTexture = true;
	//}

	return true;
}


// Сохраняет RawImage в blp и делает доступным для использования в игре
int __stdcall SaveRawImageToGameFile(unsigned int RawImage, const char* filename, int IsTga, int enabled)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}


	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];
	tmpRawImage.filename = filename;

	StormBuffer inbuffer = tmpRawImage.img;
	StormBuffer ResultBuffer = StormBuffer();

	if (tmpRawImage.ingamebuffer.buf)
	{
		tmpRawImage.ingame = false;
		tmpRawImage.ingamebuffer.Clear();
	}

	if (enabled)
	{
		int mipmaps = 0;
		if (IsTga)
			RAW2Tga(inbuffer, ResultBuffer, tmpRawImage.width, tmpRawImage.height, 4, filename);
		else
			CreatePalettedBLP(inbuffer, ResultBuffer, 256, filename, tmpRawImage.width, tmpRawImage.height, 4, 8, mipmaps);
		tmpRawImage.ingamebuffer = ResultBuffer;
	}

	tmpRawImage.ingame = enabled;
	return true;
}


// Сохраняет RawImage на диск в TGA по выбранному пути
int __stdcall DumpRawImageToFile(unsigned int RawImage, const char* filename)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct tmpRawImage = ListOfRawImages[RawImage];
	StormBuffer outbuffer;
#ifdef OLD_CODE
	StormBuffer inbuffer = tmpRawImage.img;
	RAW2Tga(inbuffer, outbuffer, tmpRawImage.width, tmpRawImage.height, 4, filename);
#else 


#endif 

	FILE* f;
	fopen_s(&f, filename, "wb");
	if (f)
	{
		fwrite(outbuffer.buf, 1, outbuffer.length, f);
		fclose(f);
	}

	return true;
}


// Получает RawImage из списка RawImages по имени файла.
int __stdcall GetRawImageByFile(const char* filename)
{
	if (!InitFunctionCalled)
		return 0;
	int id = 0;
	for (RawImageStruct& s : ListOfRawImages)
	{
		if (ToLower(s.filename) == ToLower(filename))
			return id;
		id++;
	}

	return 0;
}

// Получает ширину RawImage
int __stdcall RawImage_GetWidth(unsigned int RawImage)
{
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return 64;
	}

	return ListOfRawImages[RawImage].width;
}

// Получает высоту RawImage
int __stdcall RawImage_GetHeight(unsigned int RawImage)
{
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return 64;
	}

	return ListOfRawImages[RawImage].height;
}

// Изменяет размер RawImage
int __stdcall RawImage_Resize(unsigned int RawImage, unsigned int newwidth, unsigned int newheight)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	newwidth = newwidth + (newwidth % 2);
	newheight = newheight + (newheight % 2);


	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];

#ifdef OLD_CODE

	StormBuffer tmpOldBuffer = tmpRawImage.img;
	StormBuffer tmpNewBuffer = StormBuffer();
	ScaleImage((unsigned char*)tmpOldBuffer.buf, tmpRawImage.width, tmpRawImage.height, newwidth, newheight, 4, tmpNewBuffer);
	tmpOldBuffer.Clear();
	tmpRawImage.img = tmpNewBuffer;
#else 

#endif


	tmpRawImage.height = newheight;
	tmpRawImage.width = newwidth;

	//if ( tmpRawImage.used_for_overlay )
	tmpRawImage.needResetTexture = true;

	return true;
}



int PowerOfTwo(int Value)
{
	int InitValue = 1;
	while (InitValue < Value)
		InitValue *= 2;
	return InitValue;
}

// Рисует RawImage по заданным координатам (от 0.0 до 1.0) в игре. 
int __stdcall RawImage_DrawOverlay(unsigned int RawImage, int enabled, float xpos, float ypos)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];

	if (AutoFixImagesSize)
	{
		if (tmpRawImage.width != PowerOfTwo(tmpRawImage.width)
			|| tmpRawImage.height != PowerOfTwo(tmpRawImage.height))
		{
			int oldwidth = tmpRawImage.width;
			int oldheight = tmpRawImage.height;

			tmpRawImage.width = PowerOfTwo(tmpRawImage.width);
			tmpRawImage.height = PowerOfTwo(tmpRawImage.height);
#ifdef OLD_CODE
			char* newimage = (char*)Scale_WithoutResize((unsigned char*)tmpRawImage.img.buf, oldwidth, oldheight,
				tmpRawImage.width, tmpRawImage.height, 4);
			tmpRawImage.img.buf = newimage;
			tmpRawImage.img.length = tmpRawImage.width * tmpRawImage.height * 4;
#else 

#endif

			//RawImage_Resize( RawImage, PowerOfTwo( tmpRawImage.width ), PowerOfTwo( tmpRawImage.height ) );
		}
	}
	tmpRawImage.used_for_overlay = enabled;
	tmpRawImage.overlay_x = xpos;
	tmpRawImage.overlay_y = ypos;



	return true;
}

int __stdcall RawImage_EnableOverlay(unsigned int RawImage, int enabled)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];

	tmpRawImage.used_for_overlay = enabled;

	return true;
}

int __stdcall RawImage_MoveTimed(unsigned int RawImage, float x2, float y2, unsigned int Time1, unsigned int Time2, unsigned int SleepTime)
{
	if (!InitFunctionCalled)
		return 0;
	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];
	tmpRawImage.overlay_x0 = tmpRawImage.overlay_x;
	tmpRawImage.overlay_y0 = tmpRawImage.overlay_y;
	tmpRawImage.overlay_x2 = x2;
	tmpRawImage.overlay_y2 = y2;
	tmpRawImage.MoveTime1 = Time1;
	tmpRawImage.MoveTime2 = Time2;
	tmpRawImage.SleepTime = SleepTime;
	tmpRawImage.StartTimer = GetTickCount();
	return true;
}


RawImageCallbackData* GlobalRawImageCallbackData = NULL;




int __stdcall RawImage_SetPacketCallback(unsigned int RawImage, int enable, unsigned int events)
{
	if (!InitFunctionCalled)
		return 0;


	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];


	tmpRawImage.PacketCallback = enable;
	tmpRawImage.events = events;
	tmpRawImage.IsMouseDown = false;
	tmpRawImage.IsMouseEntered = false;

	return 0;
}

int __stdcall RawImage_AddCallback(unsigned int RawImage, const char* MouseActionCallback, RawImageCallbackData* callbackdata, unsigned int events)
{
	if (!InitFunctionCalled)
		return 0;

	if (SetInfoObjDebugVal)
	{
		PrintText("Add callback");
	}
	GlobalRawImageCallbackData = callbackdata;

	if (RawImage >= (int)ListOfRawImages.size())
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("Error!");
		}
		return false;
	}


	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];

	if (!MouseActionCallback || MouseActionCallback[0] == '\0')
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("Error callback disabled");
		}
		tmpRawImage.MouseCallback = false;
		tmpRawImage.MouseActionCallback = jRCString();
		tmpRawImage.MouserExecuteFuncCallback = false;
	}
	else
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("Ok. Added for function:" + std::string(MouseActionCallback));
		}
		tmpRawImage.MouseActionCallback = jRCString();
		str2jstr(&tmpRawImage.MouseActionCallback, MouseActionCallback);
		tmpRawImage.MouseCallback = true;
		tmpRawImage.MouserExecuteFuncCallback = true;
	}

	tmpRawImage.events = events;
	tmpRawImage.IsMouseDown = false;
	tmpRawImage.IsMouseEntered = false;

	return true;
}

int __stdcall RawImage_IsBtn(unsigned int RawImage, int enabled)
{
	if (!InitFunctionCalled)
		return 0;

	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];
	tmpRawImage.button = enabled;
	return true;
}

int __stdcall RawImage_UseImageCoords(unsigned int RawImage, int enabled)
{
	if (!InitFunctionCalled)
		return 0;

	if (RawImage >= (int)ListOfRawImages.size())
	{
		return false;
	}

	RawImageStruct& tmpRawImage = ListOfRawImages[RawImage];
	tmpRawImage.UseImageCoords = enabled;
	return true;
}




void SendRawImagePacket(RawImageCallbackData* callbackdata)
{
	std::vector<unsigned char>SendKeyEvent;
	SendKeyEvent.push_back(0x50);
	// header custom packets
	SendKeyEvent.push_back(0xFF);
	// size custom packets 
	SendKeyEvent.push_back(0);
	SendKeyEvent.push_back(0);
	SendKeyEvent.push_back(0);
	SendKeyEvent.push_back(0);
	// packet type
	int packettype = 'IIMG';
	SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)&packettype, ((unsigned char*)&packettype) + 4);
	*(int*)&SendKeyEvent[2] += 4;
	// data
	int locpid = GetLocalPlayerId();
	SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)&locpid, ((unsigned char*)&locpid) + 4);
	*(int*)&SendKeyEvent[2] += 4;
	SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)callbackdata, ((unsigned char*)callbackdata) + sizeof(RawImageCallbackData));
	*(int*)&SendKeyEvent[2] += sizeof(RawImageCallbackData);

	SendPacket((unsigned char*)&SendKeyEvent[0], SendKeyEvent.size());
	SendKeyEvent.clear();
}

int RawImageGlobalCallbackFunc(RawImageEventType callbacktype, float mousex, float mousey)
{
	if (!GlobalRawImageCallbackData)
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("Callback not initialzied!");
		}
		return false;
	}
	GlobalRawImageCallbackData->IsAltPressed = IsKeyPressed(VK_MENU);
	GlobalRawImageCallbackData->IsCtrlPressed = IsKeyPressed(VK_CONTROL);
	GlobalRawImageCallbackData->EventType = callbacktype;

	float ScreenX = *GetWindowXoffset;
	float ScreenY = *GetWindowYoffset;

	float scalex = ScreenX / DefaultSceenWidth;
	float scaley = ScreenY / DefaultSceenHeight;

	//scalex *= DesktopScreen_Width / DefaultSceenWidth;
	//scaley *= DesktopScreen_Height / DefaultSceenHeight;

	float mouseposx = mousex /*/ ScreenX*/;
	float mouseposy = mousey /*/ ScreenY*/;

	GlobalRawImageCallbackData->mousex = mouseposx;
	GlobalRawImageCallbackData->mousey = mouseposy;

	for (auto& img : ListOfRawImages)
	{
		int NeedSkipEvent = rawimage_skipmouseevent;
#ifdef OLD_CODE
		RGBAPix* RawImageData = (RGBAPix*)img.img.buf;
#else 


#endif

		if (img.used_for_overlay &&
			img.MouseCallback &&
			((img.events & (unsigned int)callbacktype) > 0 || (callbacktype == RawImageEventType::MouseDown && img.button)))
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("Callback need!");
			}

			if (img.button)
			{
				if (SetInfoObjDebugVal)
				{
					PrintText("Button found!");
				}
			}
			else
			{
				if (SetInfoObjDebugVal)
				{
					PrintText("No bo found!");
				}
			}

			int MouseEnteredInRawImage = false;

			float posx = ScreenX * img.overlay_x;
			float posy = ScreenY * img.overlay_y;

			float sizex = (float)img.width * scalex;
			float sizey = (float)img.height * scaley;

			float img_x, img_y;
			//posy -= sizey;
			GlobalRawImageCallbackData->RawImage = img.RawImage;
			GlobalRawImageCallbackData->RawImageCustomId = img.RawImageCustomId;

			if (mousex > posx && mousex < posx + sizex && mousey > posy && mousey < posy + sizey)
			{

				img_x = sizex - (posx + sizex - mousex);
				img_y = sizey - (posy + sizey - mousey);

				img_x /= scalex;
				img_y /= scaley;

				if (img_x > img.width)
					img_x = (float)img.width;

				if (img_y > img.height)
					img_y = (float)img.height;

				if (img_x < 1)
					img_x = 0;

				if (img_y < 1)
					img_y = 0;

				if (img.UseImageCoords)
				{
					GlobalRawImageCallbackData->mousex = img_x;
					GlobalRawImageCallbackData->mousey = img_y;
				}

				if (SetInfoObjDebugVal)
				{
					PrintText("Mouse x/y:" + std::to_string(mouseposx) + "/" + std::to_string(mouseposy));
					PrintText("Mouse pos x/y:" + std::to_string(mousex) + "/" + std::to_string(mousey));
					PrintText("Image pos x/y:" + std::to_string(posx) + "/" + std::to_string(posy));
					PrintText("Image click x/y:" + std::to_string(img_x) + "/" + std::to_string(img_y));
				}
#ifdef OLD_CODE
				RGBAPix* RawImageData = (RGBAPix*)img.img.buf;
				RGBAPix eventpix = RawImageData[ArrayXYtoId(img.width, (int)img_x, img.height - (int)img_y)];
#else 

#endif
				if (eventpix.A >= 20)
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("Click can be here!");
					}
					NeedSkipEvent = false;
				}
				else
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("Found bad pixel");
					}
				}
				MouseEnteredInRawImage = true;
			}
			else
			{
				img_x = 0;
				img_y = 0;
			}

			GlobalRawImageCallbackData->offsetx = (int)img_x;
			GlobalRawImageCallbackData->offsety = (int)img_y;

			switch (callbacktype)
			{
			case RawImageEventType::MouseUp:
				if (img.IsMouseDown || !(img.events & (unsigned int)RawImageEventType::MouseDown))
				{
					img.IsMouseDown = false;

					if (MouseEnteredInRawImage)
						GlobalRawImageCallbackData->EventType = RawImageEventType::MouseClick;

					if ((img.events & (unsigned int)RawImageEventType::MouseUp) > 0
						|| (img.events & (unsigned int)RawImageEventType::MouseClick) > 0)
					{
						if (!NeedSkipEvent && img.MouserExecuteFuncCallback)
						{
							if (SetInfoObjDebugVal)
							{
								PrintText("ExecuteFunc MouseUp");
							}

							ExecuteFunc(&img.MouseActionCallback);

						}
						if (!NeedSkipEvent && img.PacketCallback)
							SendRawImagePacket(GlobalRawImageCallbackData);
					}
					return !NeedSkipEvent && (rawimage_skipmouseevent || img.button);
				}
				break;
			case RawImageEventType::MouseDown:
				if ((!img.IsMouseDown || !(img.events & (unsigned int)RawImageEventType::MouseUp))
					&& MouseEnteredInRawImage)
				{
					img.IsMouseDown = true;

					GlobalRawImageCallbackData->EventType = RawImageEventType::MouseDown;

					if ((img.events & (unsigned int)RawImageEventType::MouseDown) > 0)
					{
						if (!NeedSkipEvent && img.MouserExecuteFuncCallback)
						{
							if (SetInfoObjDebugVal)
							{
								PrintText("ExecuteFunc MouseDown");
							}
							ExecuteFunc(&img.MouseActionCallback);

						}
						if (!NeedSkipEvent && img.PacketCallback)
							SendRawImagePacket(GlobalRawImageCallbackData);
					}
					return !NeedSkipEvent && (rawimage_skipmouseevent || img.button);
				}
				break;
			case RawImageEventType::MouseClick:
				break;
			case RawImageEventType::MouseEnter:
				break;
			case RawImageEventType::MouseLeave:
				break;
			case RawImageEventType::MouseMove:
				if (img.IsMouseEntered)
				{
					if (!MouseEnteredInRawImage)
					{
						img.IsMouseEntered = false;
						GlobalRawImageCallbackData->EventType = RawImageEventType::MouseLeave;
						if ((img.events & (unsigned int)RawImageEventType::MouseLeave) > 0)
						{
							if (img.MouserExecuteFuncCallback)
							{
								if (SetInfoObjDebugVal)
								{
									PrintText("ExecuteFunc MouseLeave");
								}
								ExecuteFunc(&img.MouseActionCallback);
							}
							if (img.PacketCallback)
								SendRawImagePacket(GlobalRawImageCallbackData);
						}
					}
				}
				else
				{
					if (MouseEnteredInRawImage)
					{
						if (!NeedSkipEvent)
						{
							img.IsMouseEntered = true;
							GlobalRawImageCallbackData->EventType = RawImageEventType::MouseEnter;
							if ((img.events & (unsigned int)RawImageEventType::MouseEnter) > 0)
							{
								if (img.MouserExecuteFuncCallback)
								{
									if (SetInfoObjDebugVal)
									{
										PrintText("ExecuteFunc MouseEnter");
									}
									ExecuteFunc(&img.MouseActionCallback);
								}
								if (img.PacketCallback)
									SendRawImagePacket(GlobalRawImageCallbackData);
							}
						}
					}
				}
				break;
			case RawImageEventType::ALL:
				if (img.IsMouseDown)
				{
					img.IsMouseDown = false;
					GlobalRawImageCallbackData->EventType = RawImageEventType::MouseUp;
					if ((img.events & (unsigned int)RawImageEventType::MouseUp) > 0)
					{
						if (img.MouserExecuteFuncCallback)
						{
							if (SetInfoObjDebugVal)
							{
								PrintText("ExecuteFunc MouseUp2");
							}
							ExecuteFunc(&img.MouseActionCallback);
						}
						if (img.PacketCallback)
							SendRawImagePacket(GlobalRawImageCallbackData);
					}
				}
				if (img.IsMouseEntered)
				{
					img.IsMouseEntered = false;
					GlobalRawImageCallbackData->EventType = RawImageEventType::MouseLeave;
					if ((img.events & (unsigned int)RawImageEventType::MouseLeave) > 0)
					{
						if (img.MouserExecuteFuncCallback)
						{
							if (SetInfoObjDebugVal)
							{
								PrintText("ExecuteFunc MouseLeave");
							}
							ExecuteFunc(&img.MouseActionCallback);
						}
						if (img.PacketCallback)
							SendRawImagePacket(GlobalRawImageCallbackData);
					}
				}
				break;
			default:
				break;
			}
		}
		else if (SetInfoObjDebugVal)
		{
			if (IsKeyPressed(VK_LMENU))
			{
				PrintText("Callback for:" + std::to_string(GlobalRawImageCallbackData->RawImage) + " not initalized!");
			}
		}
	}

	return false;
}


//
//void ApplyIconFrameFilter2( std::string filename, int * OutDataPointer, size_t * OutSize )
//{
//	int RawImage = CreateRawImage( 128, 128, RGBAPix( ) );
//	//RawImage_Resize( RawImage, 128, 128 );
//	int RawImage2 = LoadRawImage( filename.c_str( ) );
//	RawImage_DrawImg( RawImage, RawImage2, 32, 32 );
//	RGBAPix tmppix = RGBAPix( );
//	RawImage_EraseCircle( RawImage, 64, 64, 29, true );
//	RawImage_DrawCircle( RawImage, 64, 64, 35, 6, tmppix.RGBAPixWar3( 0, 255, 0, 255 ) );
//
//
//	RawImage_DrawText( RawImage, "|C00FF0000RED|r |CFF00FF00GREEN|r |CFF0000FFBLUE|r", 10, 10, tmppix.RGBAPixWar3( 255, 0, 0, 0 ) );
//
//	SaveRawImageToGameFile( RawImage, ( filename + "_frame.blp" ).c_str( ), false, true );
//	DumpRawImageToFile( RawImage, "temp.tga" );
//
//	RawImage_DrawOverlay( RawImage, true, 0.1f, 0.1f, 0, 0 );
//	ApplyIconFrameFilter3( filename, OutDataPointer, OutSize );
//}
//
//

void ApplyIconFrameFilter(std::string filename, int* OutDataPointer, size_t* OutSize)
{
	int RawImage = CreateRawImage(128, 128, RGBAPix());
	int RawImage2 = LoadRawImage(filename.c_str());
	RawImage_DrawImg(RawImage, RawImage2, 32, 32, 0);
	RGBAPix tmppix = RGBAPix();
	SaveRawImageToGameFile(RawImage, (filename + "_frame.blp").c_str(), false, true);
}




void ClearAllRawImages()
{
	int i = sizeof(RawImageStruct);
	AutoFixImagesSize = true;
	for (RawImageStruct& s : ListOfRawImages)
	{
		s.used_for_overlay = false;
#ifdef OLD_CODE
		if (s.img.buf)
			s.img.Clear();
#else 

#endif
		if (s.ingame)
		{
			if (s.ingamebuffer.buf)
				s.ingamebuffer.Clear();
			s.ingame = false;
		}

		if (s.backup_img)
			delete[] s.backup_img;
	}
	ListOfRawImages.clear();

	ListOfRawImages.reserve(MAX_IMAGES_COUNT); // reserve 15MB for RawImageStruct

	RGBAPix tmppix = RGBAPix();
	CreateRawImage(64, 64, tmppix.RGBAPixWar3(0, 255, 0, 255));
}


/* Only for game. int return = fix missing eax */
int __stdcall GetScreenWidth(int)
{
	return *(int*)&DesktopScreen_Width;
}
int __stdcall GetScreenHeight(int)
{
	return  *(int*)&DesktopScreen_Height;
}

int __stdcall GetWindowWidth(int)
{
	if (IsGame())
		return   *(int*)GetWindowXoffset;
	return *(int*)&DesktopScreen_Width;
}
int __stdcall GetWindowHeight(int)
{
	if (IsGame())
		return   *(int*)GetWindowYoffset;
	return *(int*)&DesktopScreen_Height;
}
/* end */

float DefaultSceenWidth = (float)GetSystemMetrics(SM_CXSCREEN);
float DefaultSceenHeight = (float)GetSystemMetrics(SM_CYSCREEN);

int __stdcall SetDefaultSceenSize(int w, int h)
{
	DefaultSceenWidth = (float)w;
	DefaultSceenHeight = (float)h;

	return 0;
}


float RawImageOffsetToWar3_X(float raw_offset_x)
{
	return (float)(0.8 / 1.0 * (double)raw_offset_x);
}

float RawImageOffsetToWar3_Y(float raw_offset_y)
{
	return (float)(0.6 - (0.6 / 1.0 * (double)raw_offset_y));
}