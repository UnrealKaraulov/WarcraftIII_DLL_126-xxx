#pragma once

unsigned char FixBounds(int i);
unsigned char FixBounds(float i);
unsigned char FixBounds(double i);


#include "BlpReadWrite.h"

class CQuantizer
{
	typedef struct tagNode
	{
		int bIsLeaf;
		unsigned int nPixelCount;
		unsigned int nRedSum;
		unsigned int nGreenSum;
		unsigned int nBlueSum;
		unsigned int nAlphaSum;
		unsigned int nIndex;
		struct tagNode* pChild[8];
		struct tagNode* pNext;
	} Node;

protected:
	Node* m_pTree;
	unsigned int m_nLeafCount;
	Node* m_pReducibleNodes[8];
	unsigned int m_nMaxColors;
	unsigned int m_nOutputMaxColors;
	unsigned int m_nColorBits;
	unsigned char m_lastIndex;
	int m_needsAlpha;

public:
	CQuantizer(unsigned int nMaxColors, unsigned int nColorBits);
	virtual ~CQuantizer();
	int ProcessImage(unsigned char* image, unsigned long size, unsigned char bytespp, unsigned char alpha);
	void FloydSteinbergDither(unsigned char* image, long width, long height, unsigned char bytespp, unsigned char* target, COLOR4* pal);
	int NeedsAlphaChannel();
	unsigned int GetColorCount();
	void SetColorTable(COLOR4* prgb);
	unsigned char GetNearestIndex(COLOR4* c, COLOR4* pal);
	unsigned char GetNearestIndexFast(COLOR4* c, COLOR4* pal);

protected:
	unsigned int GetLeafCount(Node* pTree);
	void AddColor(Node** ppNode, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned int nColorBits, int nLevel, unsigned int* pLeafCount, Node** pReducibleNodes);
	void* CreateNode(int nLevel, unsigned int nColorBits, unsigned int* pLeafCount, Node** pReducibleNodes);
	void ReduceTree(unsigned int nColorBits, unsigned int* pLeafCount, Node** pReducibleNodes);
	void DeleteTree(Node** ppNode);
	void GetPaletteColors(Node* pTree, COLOR4* prgb, unsigned int* pIndex, unsigned int* pSum);
	unsigned char GetNextBestLeaf(Node** pTree, unsigned int nLevel, COLOR4* c, COLOR4* pal);
	int ColorsAreEqual(COLOR4* a, COLOR4* b);
};