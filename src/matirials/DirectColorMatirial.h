/*
 * DirectColorMatirial.h
 *
 *  Created on: 10 Dec 2019
 *      Author: christiaan
 */

#ifndef MATIRIALS_DIRECTCOLORMATIRIAL_H_
#define MATIRIALS_DIRECTCOLORMATIRIAL_H_

#include "../Matirial.h"

class DirectColorMatirial3 : public Matirial3 {
public:
	Color color;
	inline DirectColorMatirial3(Color color) : color(color) {}
	virtual ~DirectColorMatirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		return color;
	}
};

class DirectNormColorMatirial3 : public Matirial3 {
public:
	Color color;
	inline DirectNormColorMatirial3(Color color) : color(color) {}
	virtual ~DirectNormColorMatirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		double str = 0.5 + (ray->norm.x+1)/4 - (ray->norm.y+1)/8 - (ray->norm.z+1)/16;
		//return Color(str,str,str,str).combine(color);
		return Color(color.red*str, color.green*str, color.blue*str, color.alpha);
	}
};

class DirectNormColorMatirial4 : public Matirial4 {
public:
	Color color;
	inline DirectNormColorMatirial4(Color color) : color(color) {}
	virtual ~DirectNormColorMatirial4() {}
	virtual Color doShadeColorRay(Ray4* ray, Ray4* ecadianRay, RenderEnv4* env, RenderStackFrame4* frame) {
		double str = 0.5 + (ray->norm.x+1)/4 - (ray->norm.y+1)/8 - (ray->norm.z+1)/16 - (ray->norm.m+1)/32;
		//return Color(str,str,str,str).combine(color);
		return Color(color.red*str, color.green*str, color.blue*str, color.alpha);
	}
};



#endif /* MATIRIALS_DIRECTCOLORMATIRIAL_H_ */
