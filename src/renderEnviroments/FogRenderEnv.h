/*
 * DebugRenderEnv.h
 *
 *  Created on: 11 Dec 2019
 *      Author: christiaan
 */

#ifndef RENDERENVIROMENTS_FOGRENDERENV_H_
#define RENDERENVIROMENTS_FOGRENDERENV_H_

#include "../RenderEnv.h"
#include <cmath>

//baced on depf (like in the game)
class FogRenderEnv3 : public RenderEnvBasic3 {
public:
	Color FogColor;
	double FogStrenght;
	inline FogRenderEnv3(RayVerivirer3 alowRay, Color FogColor, double FogStrenght) : RenderEnvBasic3(alowRay), FogColor(FogColor), FogStrenght(-FogStrenght) {}
	virtual ~FogRenderEnv3() {}
	virtual Color doRayColor(Ray3* ray, Ray3* ecadianRay, RenderStackFrame3* frame) {
		if (alowRay(frame)) {
			flushEnv(ray);
			if (ray->maxDist == DBL_MAX)
				return FogColor;

			double fog = pow(M_E, ray->maxDist*FogStrenght);
			ray->finish();
			if (ecadianRay == nullptr) {
				RenderStackFrame3 cframe = frame->addRay(ray, ColorRayType, fog);
				fog = 1-fog;
				ray->norm = ray->norm.normalize();
				Color c = ray->geromitry.mat->doShadeColorRay(ray, nullptr, this, &cframe);
				return Color(fog*FogColor.red, fog*FogColor.green, fog*FogColor.blue, fog).combine(c);
			} else {
				RenderStackFrame3 cframe = frame->addNonEdian(ray->maxDist, fog);
				fog = 1-fog;
				Ray3 ecadianRay2 (*ecadianRay);
				ecadianRay2.maxDist+=ray->maxDist;
				ray->norm = ray->norm.normalize();
				Color c = ray->geromitry.mat->doShadeColorRay(ray, &ecadianRay2, this, &cframe);
				return Color(fog*FogColor.red, fog*FogColor.green, fog*FogColor.blue, fog).combine(c);
			}
		} else
			return FogColor;
	}
};


#endif /* RENDERENVIROMENTS_DEBUGRENDERENV_H_ */
