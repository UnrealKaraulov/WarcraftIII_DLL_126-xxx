#ifndef POINT_H_
#define POINT_H_

class Point {
public:
	float x;
	float y;

	Point(void);
	Point(float _x, float _y);
	~Point(void);

	bool operator==(const Point rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Point rhs) const { return x != rhs.x || y != rhs.y; }
	bool operator<(const Point rhs) const { return this->hashValue() < rhs.hashValue(); }

	Point offset(float offsetX, float offsetY);
	Point offsetFacing(float facing, float offsetX, float offsetY);
	Point offsetPolar(float distance, float angle);//angle in radian
	Point offsetPolarDeg(float distance, float angleDegree);//angle in degree
	Point offsetPolarMaxXY(float sideDistance, float angle);

	float distanceTo(Point other) const;
	float distanceTo(float otherx, float othery) const;
	float maxXYDistanceTo(Point other) const;
	float angleTo(Point other) const;
	float angleTo(float otherx, float othery) const;

	unsigned int hashValue() const;

	std::string toString();

	bool isVisible(int whichPlayer) const;

	//add methods like Point <-> Region, Pathing check, etc...
};

const Point POINT_NONE = Point();

class Region {
public:
	Region() {}
	virtual bool operator==(int iNULL) { return true; }
	virtual bool hasPoint(Point p) { return false; }
	virtual bool hasPoint(float x, float y) { return false; }
};

class RegionCircle : public Region {
public:
	Point center;
	float radius;

	RegionCircle(Point p, float f) : center(p), radius(f) { }
	RegionCircle(float x, float y, float f) : center(Point(x, y)), radius(f) { }

	bool operator==(int iNULL) { return iNULL == NULL ? (radius <= 0) : false; }

	bool hasPoint(Point p);
	bool hasPoint(float x, float y);
};


#endif