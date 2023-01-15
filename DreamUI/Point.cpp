#include "stdafx.h"
#include "Point.h"

#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Tools.h"

Point::Point(void) : x(), y() { }
Point::Point(float _x, float _y) : x(_x), y(_y) { }
Point::~Point(void) { };

Point Point::offset(float offsetX, float offsetY) {
	return Point(x + offsetX, y + offsetY);
}
Point Point::offsetFacing(float facing, float offsetX, float offsetY) {
	Point p = this->offsetPolar(offsetY, facing);
	p = p.offsetPolar(offsetX, facing - (float)(M_PI / 2));
	return p;
}

Point Point::offsetPolar(float distance, float angle) {//angle in radian
	return Point(x + distance * cos(angle), y + distance * sin(angle));
}
Point Point::offsetPolarDeg(float distance, float angleDegree) {//angle in degree
	return offsetPolar(distance, (float)(M_PI * angleDegree / 180));
}

Point Point::offsetPolarMaxXY(float sideDistance, float angle)
{
	Point p(this->x, this->y);
	float angle2 = fmod(angle, (float)(M_PI * 2));
	if (angle2 < 0) angle2 += (float)(M_PI * 2);

	float sine = sin(angle2);
	float cosine = cos(angle2);
	if (abs(sine) < sqrt(2.0) / 2) //以X方向为准
	{
		if (cosine > 0) p.x += sideDistance;
		else p.x -= sideDistance;

		if (sine > 0)
		{
			p.y += sideDistance * abs(tan(angle2));
		}
		else
		{
			p.y -= sideDistance * abs(tan(angle2));
		}
	}
	else
	{
		if (sine > 0) p.y += sideDistance;
		else p.y -= sideDistance;

		if (abs(cosine) < 0.02)
		{
			//忽略
		}
		else if (cosine > 0)
		{
			p.x += sideDistance / abs(tan(angle2));
		}
		else
		{
			p.x -= sideDistance / abs(tan(angle2));
		}
	}

	return p;
}

float Point::distanceTo(Point other) const {
	return sqrt(pow(x - other.x, 2.0f) + pow(y - other.y, 2.0f));
}

float Point::distanceTo(float otherx, float othery) const {
	return sqrt(pow(x - otherx, 2.0f) + pow(y - othery, 2.0f));
}

float Point::maxXYDistanceTo(Point other) const
{
	return max(abs(x - other.x), abs(y - other.y));
}

float Point::angleTo(Point other) const {
	return atan2((other.y - y), (other.x - x));
}

float Point::angleTo(float otherx, float othery) const {
	return atan2((othery - y), (otherx - x));
}

unsigned int Point::hashValue() const {
	unsigned int seed = 0;
	HashCombine(&seed, this->x);
	HashCombine(&seed, this->y);
	return seed;
}

std::string Point::toString() {
	std::ostringstream s;
	s << "(" << x << ", " << y << ")";
	return s.str();
}

bool Point::isVisible(int whichPlayer) const
{
	return 0;
}

//Region::Region() {}

bool RegionCircle::hasPoint(Point p) { return pow(center.x - p.x, 2) + pow(center.y - p.y, 2) < pow(radius, 2); }
bool RegionCircle::hasPoint(float x, float y) { return pow(center.x - x, 2) + pow(center.y - y, 2) < pow(radius, 2); }