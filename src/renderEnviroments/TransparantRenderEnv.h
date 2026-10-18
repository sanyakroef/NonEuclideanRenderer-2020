/*
 * DebugRenderEnv.h
 *
 *  Created on: 11 Dec 2019
 *      Author: christiaan
 */

#ifndef RENDERENVIROMENTS_TRANSPARANTRENDERENV_H_
#define RENDERENVIROMENTS_TRANSPARANTRENDERENV_H_

#include "../RenderEnv.h"

class TransparantRenderEnv3 : public RenderEnvBasic3 {
public:
	inline TransparantRenderEnv3(RayVerivirer3 alowRay) : RenderEnvBasic3(alowRay) {}
	virtual ~TransparantRenderEnv3() {}
	virtual Color doRayColor(Ray3* ray, Ray3* ecadianRay, RenderStackFrame3* frame) {
		if (alowRay(frame)) {
			flushEnv(ray);
			RenderStackFrame3 cframe = frame->addRay(ray, ColorRayType);
			if (ray->maxDist == DBL_MAX)
				return Color(0,0,0,0);

			ray->finish();
			if (ecadianRay == nullptr) {
				ray->norm = ray->norm.normalize();
				return ray->geromitry.mat->doShadeColorRay(ray, nullptr, this, &cframe);
			} else {
				Ray3 ecadianRay2 (*ecadianRay);
				ecadianRay2.maxDist+=ray->maxDist;
				ray->norm = ray->norm.normalize();
				return ray->geromitry.mat->doShadeColorRay(ray, &ecadianRay2, this, &cframe);
			}
		} else
			return Color(0,0,0,0);
	}
};


#endif /* RENDERENVIROMENTS_DEBUGRENDERENV_H_ */
