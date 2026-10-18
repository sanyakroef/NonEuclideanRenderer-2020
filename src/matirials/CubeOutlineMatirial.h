/*
 * AddMatirial.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef MATIRIALS_CUBEOUTLINEMATIRIAL_H_
#define MATIRIALS_CUBEOUTLINEMATIRIAL_H_

#include "../Matirial.h"
#include <float.h>

inline double getBest2(double* data, unsigned size) {
	double best1 = DBL_MAX;
	double best2 = DBL_MAX;
	for (unsigned i = 0;i<size;i++) {
		if (best2 > data[i]) {
			if (best1 > data[i]) {
				best2 = best1;
				best1 = data[i];
			} else
				best2 = data[i];
		}
	}
	return best2;
}

class CubeOutlineMatirial3 : public Matirial3 {
public:
	Matirial3* mat;
	Color outline;
	double outLineSize;
	inline CubeOutlineMatirial3(Matirial3* mat, Color outline, double outLineSize) : mat(mat), outline(outline), outLineSize(outLineSize) {}
	virtual ~CubeOutlineMatirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		if (ray->geromitry.gerom.type == Geromitry3::AABBcubeType) {
			Vector3 intersect = ray->loc+ray->dir*ray->maxDist;
			Vector3 norm = ray->norm;
			Vector3 t0 = (((AABBcube3*)ray->geromitry.gerom.data)->min - intersect).abs();
			Vector3 t1 = (((AABBcube3*)ray->geromitry.gerom.data)->max - intersect).abs();

			double data[6] = {t0.x, t0.y, t0.z, t1.x, t1.y, t1.z};
			if (getBest2(data, 6) <= outLineSize)
				return outline;
		}
		return mat->doShadeColorRay(ray, ecadianRay, env, frame);
	}
};


#endif /* MATIRIALS_ADDMATIRIAL_H_ */
