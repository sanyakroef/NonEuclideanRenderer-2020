/*
 * Ray.h
 *
 *  Created on: 6 Dec 2019
 *      Author: christiaan
 */

#ifndef RAY_H_
#define RAY_H_

#include "Vector.h"
#include <float.h>
#include "Geromitry.h"
#include "Matirial.h"



//class Geromitry3;
class Triangle3;
class Sphere3;
class Ray3 {
public:
	Vector3 loc;
	Vector3 dir;

	double maxDist;
	FullGeromitry3 geromitry;
	Vector3 norm;

	inline Ray3() : loc(), dir(), maxDist(DBL_MAX) {}
	inline Ray3(Vector3 loc, Vector3 dir) : loc(loc), dir(dir), maxDist(DBL_MAX) {}
	inline ~Ray3() {}
	void parseGeromitry(Geromitry3* data, unsigned size, Matirial3* mat);
	void parseGeromitry(FullGeromitry3* data, unsigned size);
	void parseGeromitry(Triangle3* data, unsigned size, Matirial3* mar);
	void finish();
};
class Ray3IntersectReturn {
public:
	double dist;
	Vector3 norm;
	inline Ray3IntersectReturn() : dist(-1), norm() {}
	inline Ray3IntersectReturn(double dist, Vector3 norm) : dist(dist), norm(norm) {}
	inline Ray3IntersectReturn(double dist) : dist(dist), norm() {}
	inline ~Ray3IntersectReturn() {}
};

class Ray4 {
public:
	Vector4 loc;
	Vector4 dir;

	double maxDist;
	FullGeromitry4 geromitry;
	Vector4 norm;

	inline Ray4() : loc(), dir(), maxDist(DBL_MAX) {}
	inline Ray4(Vector4 loc, Vector4 dir) : loc(loc), dir(dir), maxDist(DBL_MAX) {}
	inline ~Ray4() {}

	void parseGeromitry(Geromitry4* data, unsigned size, Matirial4* mat);
	void parseGeromitry(FullGeromitry4* data, unsigned size);
	void finish();
};
class Ray4IntersectReturn {
public:
	double dist;
	Vector4 norm;
	inline Ray4IntersectReturn() : dist(-1), norm() {}
	inline Ray4IntersectReturn(double dist, Vector4 norm) : dist(dist), norm(norm) {}
	inline Ray4IntersectReturn(double dist) : dist(dist), norm() {}
	inline ~Ray4IntersectReturn() {}
};



#endif /* RAY_H_ */
