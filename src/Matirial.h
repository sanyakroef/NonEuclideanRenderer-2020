/*
 * Matirial.h
 *
 *  Created on: 10 Dec 2019
 *      Author: christiaan
 */

#ifndef MATIRIAL_H_
#define MATIRIAL_H_

#include "Color.h"

class Ray3;
class Geromitry3;
class RenderEnv3;
class RenderStackFrame3;
class Matirial3 {
public:
	inline Matirial3() {}
	virtual ~Matirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) = 0;
	virtual double doObstructionRay(Ray3* ray, RenderEnv3* env, RenderStackFrame3* frame, Sphere3* dest) {
		return 0;
	}
};


class Ray4;
class Geromitry4;
class RenderEnv4;
class RenderStackFrame4;
class Matirial4 {
public:
	inline Matirial4() {}
	virtual ~Matirial4() {}
	virtual Color doShadeColorRay(Ray4* ray, Ray4* ecadianRay, RenderEnv4* env, RenderStackFrame4* frame) = 0;
	virtual double doObstructionRay(Ray4* ray, RenderEnv4* env, RenderStackFrame4* frame, Sphere4* dest) {
		return 0;
	}
};



#endif /* MATIRIAL_H_ */
