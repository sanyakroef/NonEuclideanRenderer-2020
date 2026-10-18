/*
 * Triangle.cpp
 *
 *  Created on: 7 Dec 2019
 *      Author: christiaan
 */

#include "Triangle.h"

#define kEpsilon 0.00001

Ray3IntersectReturn Triangle3::intersect(const Ray3* ray) {

	Vector3 v0v1 = loc[1] - loc[0];
	Vector3 v0v2 = loc[2] - loc[0];
	Vector3 pvec = ray->dir.crossProduct(v0v2);
	double det = v0v1.dotProduct(pvec);

	if (std::abs(det) <= kEpsilon) return Ray3IntersectReturn();
	//if (det <= kEpsilon) return Ray3IntersectReturn();

	double invDet = 1 / det;

	Vector3 tvec = ray->loc - loc[0];
	double u = tvec.dotProduct(pvec) * invDet;
	if (u < 0 || u > 1) return Ray3IntersectReturn();

	Vector3 qvec = tvec.crossProduct(v0v1);
	double v = ray->dir.dotProduct(qvec) * invDet;
	if (v < 0 || u + v > 1) return Ray3IntersectReturn();


	//double t = locB.dotProduct(qvec) * invDet;

	//Vector3 locIntersect = (u*loc[0]) + (v*loc[1]) + ((1 - u - v)*loc[2]);
	//return locIntersect.lenght();

	//return -(det + ray.loc.dotProduct(pvec)) / (ray.dir.dotProduct(pvec));
	return Ray3IntersectReturn(v0v2.dotProduct(qvec) * invDet, norm);
}

Ray4IntersectReturn Triangle4::intersect(const Ray4* ray) {

	Vector4 v0v1 = loc[1] - loc[0];
	Vector4 v0v2 = loc[2] - loc[0];
	Vector4 v0v3 = loc[3] - loc[0];
	Vector4 pvec = ray->dir.crossProduct(v0v2, v0v3);
	double det = v0v1.dotProduct(pvec);

	if (std::abs(det) <= kEpsilon) return Ray4IntersectReturn();
	//if (det <= kEpsilon) return Ray3IntersectReturn();

	double invDet = 1 / det;

	Vector4 tvec = ray->loc - loc[0];
	double u = tvec.dotProduct(pvec) * invDet;
	if (u < 0 || u > 1) return Ray4IntersectReturn();

	Vector4 qvec = tvec.crossProduct(v0v1, v0v3);
	double v = ray->dir.dotProduct(qvec) * invDet;
	if (v < 0 || u + v > 1) return Ray4IntersectReturn();


	//double t = locB.dotProduct(qvec) * invDet;

	//Vector3 locIntersect = (u*loc[0]) + (v*loc[1]) + ((1 - u - v)*loc[2]);
	//return locIntersect.lenght();

	//return -(det + ray.loc.dotProduct(pvec)) / (ray.dir.dotProduct(pvec));
	return Ray4IntersectReturn(v0v2.dotProduct(qvec) * invDet, norm);
}

