/*
 * SphereEdgeNonedianMatirial.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef MATIRIALS_NONEDIAN_MATRIXNONEDIANMATIRIAL_H_
#define MATIRIALS_NONEDIAN_MATRIXNONEDIANMATIRIAL_H_

//SphereEdgeNonedianMatirial
#include "../../Matirial.h"
#include "../../RenderEnv.h"
#include "../../Matrix.h"
#include <iostream>

class MatrixNonedianMatirial3 : public Matirial3 {
public:
	Matrix4 mat;
	inline MatrixNonedianMatirial3(Matrix4 mat) : mat(mat) {}
	virtual ~MatrixNonedianMatirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		Vector3 loc = mat * (ray->loc+ray->dir*ray->maxDist);
		Vector3 dir = singleMult(ray->dir, mat);
		loc = loc + dir*0.00001;

		Ray3 newRay (loc, dir);

		if (ecadianRay == nullptr)
			ecadianRay = ray;

		return env->doRayColor(&newRay, ecadianRay, frame);
	}/*
	virtual double doObstructionRay(Ray3* ray, RenderEnv3* env, RenderStackFrame3* frame, Sphere3* dest) {
		if (ray->geromitry.gerom.type == Geromitry3::AABBcubeType) {
			//std::cout << "cube bouns!!" << std::endl;
			//std::cout << "- " << ray->norm.x << ' ' << ray->norm.y << ' ' << ray->norm.z << std::endl;

			Vector3 locCube = ((AABBcube3*)ray->geromitry.gerom.data)->min;
			Vector3 sizeCube = ((AABBcube3*)ray->geromitry.gerom.data)->max - locCube;
			Vector3 loc = (ray->loc+ray->dir*ray->maxDist) + sizeCube*ray->norm;
			Ray3 newRay (loc+ray->dir*0.000001, ray->dir);

			double ocl = env->doShadowRay(&newRay, frame, dest);
			ray->maxDist += newRay.maxDist;
			return ocl;
		} else
			return 0;
	}*/
};


#endif /* MATIRIALS_NONEDIAN_SPHEREEDGENONEDIANMATIRIAL_H_ */
