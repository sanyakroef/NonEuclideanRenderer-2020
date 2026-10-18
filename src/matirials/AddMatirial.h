/*
 * AddMatirial.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef MATIRIALS_ADDMATIRIAL_H_
#define MATIRIALS_ADDMATIRIAL_H_

#include "../Matirial.h"

class AddMatirial3 : public Matirial3 {
public:
	Matirial3* mat1;
	Matirial3* mat2;
	inline AddMatirial3(Matirial3* mat1, Matirial3* mat2) : mat1(mat1), mat2(mat2) {}
	virtual ~AddMatirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		Color c = mat1->doShadeColorRay(ray, ecadianRay, env, frame);
		if (c.alpha == 1)
			return c;
		else {
			frame->value *= 1-c.alpha;
			return c.combine(mat2->doShadeColorRay(ray, ecadianRay, env, frame));
		}
	}
};


#endif /* MATIRIALS_ADDMATIRIAL_H_ */
