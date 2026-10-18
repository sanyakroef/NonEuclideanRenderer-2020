/*
 * SphereEdgeNonedianMatirial.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef MATIRIALS_NONEDIAN_SPHEREEDGENONEDIANMATIRIAL_H_
#define MATIRIALS_NONEDIAN_SPHEREEDGENONEDIANMATIRIAL_H_

//SphereEdgeNonedianMatirial
#include "../../Matirial.h"
#include "../../RenderEnv.h"

class SphereEdgeNonedianMatirial3 : public Matirial3 {
public:
	inline SphereEdgeNonedianMatirial3() {}
	virtual ~SphereEdgeNonedianMatirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		if (ray->geromitry.gerom.type == Geromitry3::SphereType) {
			Vector3 locSpr = ((Sphere3*)ray->geromitry.gerom.data)->loc;
			Vector3 loc = (locSpr*2) - (ray->loc+ray->dir*ray->maxDist);
			Ray3 newRay (loc+ray->dir*0.000001, ray->dir);

			if (ecadianRay == nullptr)
				ecadianRay = ray;

			return env->doRayColor(&newRay, ecadianRay, frame);
		} else if (ray->geromitry.gerom.type == Geromitry3::AABBcubeType) {
			Vector3 locSpr2 = (((AABBcube3*)ray->geromitry.gerom.data)->min+((AABBcube3*)ray->geromitry.gerom.data)->max);
			Vector3 loc = locSpr2 - (ray->loc+ray->dir*ray->maxDist);
			Ray3 newRay (loc+ray->dir*0.000001, ray->dir);

			if (ecadianRay == nullptr)
				ecadianRay = ray;

			return env->doRayColor(&newRay, ecadianRay, frame);
		} else
			return Color(0,0,0,0);
	}
	virtual double doObstructionRay(Ray3* ray, RenderEnv3* env, RenderStackFrame3* frame, Sphere3* dest) {
		if (ray->geromitry.gerom.type == Geromitry3::SphereType) {
			Vector3 locSpr = ((Sphere3*)ray->geromitry.gerom.data)->loc;
			Vector3 loc = (locSpr*2) - (ray->loc+ray->dir*ray->maxDist);
			Ray3 newRay (loc+ray->dir*0.000001, ray->dir);

			double ocl = env->doShadowRay(&newRay, frame, dest);
			ray->maxDist += newRay.maxDist;
			return ocl;
		} else if (ray->geromitry.gerom.type == Geromitry3::AABBcubeType) {
			Vector3 locSpr2 = (((AABBcube3*)ray->geromitry.gerom.data)->min+((AABBcube3*)ray->geromitry.gerom.data)->max);
			Vector3 loc = locSpr2 - (ray->loc+ray->dir*ray->maxDist);
			Ray3 newRay (loc+ray->dir*0.000001, ray->dir);

			double ocl = env->doShadowRay(&newRay, frame, dest);
			ray->maxDist += newRay.maxDist;
			return ocl;
		} else
			return 0;
	}
};


#endif /* MATIRIALS_NONEDIAN_SPHEREEDGENONEDIANMATIRIAL_H_ */
