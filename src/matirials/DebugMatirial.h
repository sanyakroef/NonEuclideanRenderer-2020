/*
 * DebugMatirial.h
 *
 *  Created on: 10 Dec 2019
 *      Author: christiaan
 */

#ifndef MATIRIALS_DEBUGMATIRIAL_H_
#define MATIRIALS_DEBUGMATIRIAL_H_

#include "../Matirial.h"

class NormMatirial3 : public Matirial3 {
public:
	inline NormMatirial3() {}
	virtual ~NormMatirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		return Color((ray->norm.x+1)/2, (ray->norm.y+1)/2, (ray->norm.z+1)/2);
	}
};

class NormMatirial4 : public Matirial4 {
public:
	inline NormMatirial4() {}
	virtual ~NormMatirial4() {}
	virtual Color doShadeColorRay(Ray4* ray, Ray4* ecadianRay, RenderEnv4* env, RenderStackFrame4* frame) {
		double str = ((ray->norm.m+1)/4+0.5)/2;
		return Color((ray->norm.x+1)*str, (ray->norm.y+1)*str, (ray->norm.z+1)*str);
	}
};



#endif /* MATIRIALS_DEBUGMATIRIAL_H_ */
