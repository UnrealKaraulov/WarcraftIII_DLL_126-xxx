#include "stdafx.h"
#include "IUIObject.h"
#include "Timer.h"
#include "Tools.h"

const Color Color::WHITE = Color((uint8_t)0xFF, 0xFF, 0xFF, 0xFF);
const Color Color::BLACK = Color((uint8_t)0x0, 0x0, 0x0, 0xFF);
const Color Color::RED = Color((uint8_t)0xFF, 0, 0, 0xFF);
const Color Color::GREEN = Color((uint8_t)0, 0xFF, 0, 0xFF);
const Color Color::BLUE = Color((uint8_t)0, 0, 0xFF, 0xFF);
const Color Color::LIGHTBLUE = Color((uint8_t)0, 0x40, 0xFF, 0xFF);
const Color Color::CYAN = Color((uint8_t)0, 0xFF, 0xFF, 0xFF);
const Color Color::YELLOW = Color((uint8_t)0xFF, 0xFF, 0, 0xFF);
const Color Color::GOLD = Color((uint8_t)0xFF, 0xCC, 0, 0xFF);
const Color Color::GREY = Color((uint8_t)0x80, 0x80, 0x80, 0xFF);
const Color Color::DARKGREY = Color((uint8_t)0x40, 0x40, 0x40, 0xFF);


static std::set<IUIObject*> UIObjectsSet;
static Timer* UIObjectTickTimer;
static Timer* UIObjectInitTimer;

void UI_Tick(Timer* tm)
{
	for (std::set<IUIObject*>::iterator iter = UIObjectsSet.begin();
		iter != UIObjectsSet.end(); ++iter)
	{
		if (*(unsigned int*)(&iter) != NULL)//TODO crash
		{
			if (*iter)
			{
				(*iter)->tick();
			}
		}
	}
}

void UI_InitRefresh(Timer* tm)
{
	UI_Refresh();
}

void UI_Refresh()
{
	for (std::set<IUIObject*>::iterator iter = UIObjectsSet.begin();
		iter != UIObjectsSet.end(); ++iter)
	{
		(*iter)->refresh();
	}
}

void UI_Add(IUIObject* obj)
{
	UIObjectsSet.insert(obj);
}

void UI_Remove(IUIObject* obj)
{
	UIObjectsSet.erase(obj);
}

void UI_Init()
{
	UIObjectsSet.clear();
	//MUSTDO: 改为用本地timer
	UIObjectTickTimer = new Timer(5, UI_Tick, true); //每tick
	UIObjectTickTimer->startImmediate();
	UIObjectInitTimer = new Timer(5, UI_InitRefresh, false);
}

void UI_Cleanup()
{
	while (UIObjectsSet.size())
	{
		IUIObject* obj = *(UIObjectsSet.begin());
		while (obj->owner())
		{
			obj = obj->owner();
		}
		delete obj;
	}
	UIObjectsSet.clear();
	UIObjectTickTimer = NULL;
	UIObjectInitTimer = NULL;
}

#define CHOP(c) min(max(c, 0), 1)

Color::Color(float red, float green, float blue, float alpha /* = 1.f */)
{
	r = (uint8_t)(CHOP(red) * 0xFF);
	g = (uint8_t)(CHOP(green) * 0xFF);
	b = (uint8_t)(CHOP(blue) * 0xFF);
	a = (uint8_t)(CHOP(alpha) * 0xFF);
}

Color Color::mulRGB(Color& other)
{
	Color c(this);
	c.a = (uint8_t)(((unsigned int)c.a) * (((unsigned int)other.a) / ((unsigned int)0xFF)));
	c.r = (uint8_t)(((unsigned int)c.r) * (((unsigned int)other.r) / ((unsigned int)0xFF)));
	c.g = (uint8_t)(((unsigned int)c.g) * (((unsigned int)other.g) / ((unsigned int)0xFF)));
	c.b = (uint8_t)(((unsigned int)c.b) * (((unsigned int)other.b) / ((unsigned int)0xFF)));
	return c;
}

Color Color::mulRGB(float val)
{
	Color c(this);
	c.a = uint8_t(CHOP(((float)c.a / 255.f) * val) * 0xFF);
	c.r = uint8_t(CHOP(((float)c.r / 255.f) * val) * 0xFF);
	c.g = uint8_t(CHOP(((float)c.g / 255.f) * val) * 0xFF);
	c.b = uint8_t(CHOP(((float)c.b / 255.f) * val) * 0xFF);
	return c;
}

void Color::operator*=(float val)
{
	r = uint8_t(CHOP(((float)r / 255.f) * val) * 0xFF);
	g = uint8_t(CHOP(((float)g / 255.f) * val) * 0xFF);
	b = uint8_t(CHOP(((float)b / 255.f) * val) * 0xFF);
}


unsigned int Color::toUint32()
{
	//AARRGGBB
	return (unsigned int(a) << 0x18) | (unsigned int(r) << 0x10) | (unsigned int(g) << 0x8) | (unsigned int(b));
}

