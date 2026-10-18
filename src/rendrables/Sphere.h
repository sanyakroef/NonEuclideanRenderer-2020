/*
 * Sphere.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef RENDRABLES_SPHERE_H_
#define RENDRABLES_SPHERE_H_

#include "../Vector.h"
#include "../Ray.h"

class Sphere3 {
public:
	Vector3 loc;
	double size2;
	inline Sphere3() : loc(), size2(0) {}
	inline Sphere3(Vector3 loc, double size): loc(loc), size2(size*size) {}
	inline ~Sphere3() {}

	Ray3IntersectReturn intersect(const Ray3* ray) {
		Vector3 L = loc - ray->loc;
		double tca = L.dotProduct(ray->dir);
		// if (tca < 0) return false;
		double d2 = L.dotProduct(L) - tca * tca;
		if (d2 > size2) return Ray3IntersectReturn();
		double thc = sqrt(size2 - d2);
		double t0 = tca - thc;
		double t1 = tca + thc;

		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead
			if (t0 < 0) return Ray3IntersectReturn(); // both t0 and t1 are negative
			else return Ray3IntersectReturn(t0, (L-ray->dir*t0).normalize());
		}

		return Ray3IntersectReturn(t0, (ray->loc+ray->dir*t0-loc).normalize());
	}
};





class Sphere4 {
public:
	Vector4 loc;
	double size2;
	inline Sphere4() : loc(), size2(0) {}
	inline Sphere4(Vector4 loc, double size): loc(loc), size2(size*size) {}
	inline ~Sphere4() {}

	Ray4IntersectReturn intersect(const Ray4* ray) {
		Vector4 L = loc - ray->loc;
		double tca = L.dotProduct(ray->dir);
		// if (tca < 0) return false;
		double d2 = L.dotProduct(L) - tca * tca;
		if (d2 > size2) return Ray4IntersectReturn();
		double thc = sqrt(size2 - d2);
		double t0 = tca - thc;
		double t1 = tca + thc;

		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead
			if (t0 < 0) return Ray4IntersectReturn(); // both t0 and t1 are negative
			else return Ray4IntersectReturn(t0, (L-ray->dir*t0).normalize());
		}

		return Ray4IntersectReturn(t0, (ray->loc+ray->dir*t0-loc).normalize());
	}
};

#endif /* RENDRABLES_SPHERE_H_ */
