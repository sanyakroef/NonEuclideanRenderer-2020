/*
 * DebugRenderEnv.h
 *
 *  Created on: 11 Dec 2019
 *      Author: christiaan
 */

#ifndef RENDERENVIROMENTS_DEBUGRENDERENV_H_
#define RENDERENVIROMENTS_DEBUGRENDERENV_H_

#include "../RenderEnv.h"

inline Color getDebugColor(const Ray3* ray) {
	return Color((ray->dir.x+1)/2, (ray->dir.y+1)/2, (ray->dir.z+1)/2);
	//return Color((cos(ray->dir.z*50)+1)/2);
	//return Color((cos(ray->dir.z*50+cos(ray->dir.x*40)+cos(ray->dir.y*40))+1)/2);
/*
	double gray = (cos(ray->dir.z*50+cos(ray->dir.x*40)+cos(ray->dir.y*40))+1);
	return Color((ray->dir.x+1+gray)/4, (ray->dir.y+1+gray)/4, (ray->dir.z+1+gray)/4);
	*/
}

class DebugRenderEnv3 : public RenderEnvBasic3 {
public:
	inline DebugRenderEnv3(RayVerivirer3 alowRay) : RenderEnvBasic3(alowRay) {}
	virtual ~DebugRenderEnv3() {}
	virtual Color doRayColor(Ray3* ray, Ray3* ecadianRay, RenderStackFrame3* frame) {
		if (alowRay(frame)) {
			flushEnv(ray);
			if (ray->maxDist == DBL_MAX)
				return getDebugColor(ray);

			ray->finish();
			if (ecadianRay == nullptr) {
				RenderStackFrame3 cframe = frame->addRay(ray, ColorRayType);
				ray->norm = ray->norm.normalize();
				return ray->geromitry.mat->doShadeColorRay(ray, nullptr, this, &cframe);
			} else {
				RenderStackFrame3 cframe = frame->addNonEdian(ray->maxDist);
				Ray3 ecadianRay2 (*ecadianRay);
				ecadianRay2.maxDist+=ray->maxDist;
				ray->norm = ray->norm.normalize();
				return ray->geromitry.mat->doShadeColorRay(ray, &ecadianRay2, this, &cframe);
			}
		} else
			return getDebugColor(ray);
	}
};

inline Color getDebugColor(const Ray4* ray) {
	double s = (ray->dir.m+1)/2;
	return Color((ray->dir.x+1)/2*s, (ray->dir.y+1)/2*s, (ray->dir.z+1)/2*s);
}

class DebugRenderEnv4 : public RenderEnvBasic4 {
public:
	inline DebugRenderEnv4(RayVerivirer4 alowRay) : RenderEnvBasic4(alowRay) {}
	virtual ~DebugRenderEnv4() {}
	virtual Color doRayColor(Ray4* ray, Ray4* ecadianRay, RenderStackFrame4* frame) {
		if (alowRay(frame)) {
			flushEnv(ray);
			if (ray->maxDist == DBL_MAX)
				return getDebugColor(ray);

			ray->finish();
			if (ecadianRay == nullptr) {
				RenderStackFrame4 cframe = frame->addRay(ray, ColorRayType);
				ray->norm = ray->norm.normalize();
				return ray->geromitry.mat->doShadeColorRay(ray, nullptr, this, &cframe);
			} else {
				RenderStackFrame4 cframe = frame->addNonEdian(ray->maxDist);
				Ray4 ecadianRay2 (*ecadianRay);
				ecadianRay2.maxDist+=ray->maxDist;
				ray->norm = ray->norm.normalize();
				return ray->geromitry.mat->doShadeColorRay(ray, &ecadianRay2, this, &cframe);
			}
		} else
			return getDebugColor(ray);
	}
};


#endif /* RENDERENVIROMENTS_DEBUGRENDERENV_H_ */
