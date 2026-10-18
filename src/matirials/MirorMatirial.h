/*
 * DirectColorMatirial.h
 *
 *  Created on: 10 Dec 2019
 *      Author: christiaan
 */

#ifndef MATIRIALS_MIRORMATIRIAL_H_
#define MATIRIALS_MIRORMATIRIAL_H_

#include "../Matirial.h"
#include "../RenderEnv.h"

class MirorMatirial3 : public Matirial3 {
public:
	inline MirorMatirial3() {}
	virtual ~MirorMatirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		Ray3 newRay (ray->loc+ray->dir*ray->maxDist, ray->norm.reflectDirection(ray->dir));
		return env->doRayColor(&newRay, frame);
	}
};



#endif /* MATIRIALS_DIRECTCOLORMATIRIAL_H_ */
