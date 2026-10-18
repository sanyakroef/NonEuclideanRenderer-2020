/*
 * DebugMatirial.h
 *
 *  Created on: 10 Dec 2019
 *      Author: christiaan
 */

#ifndef MATIRIALS_IMAGEMATIRIAL_H_
#define MATIRIALS_IMAGEMATIRIAL_H_

#include "../Matirial.h"
#include "../Matrix.h"
#include "../Image.h"
#include <iostream>

class ImageMatirial3 : public Matirial3 {
public:
	void* image;
	Matrix4 mat;
	inline ImageMatirial3(Matrix4 mat, void* image) : mat(mat), image(image) {}
	virtual ~ImageMatirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		Vector3 loc = mat * (ray->loc+ray->dir*ray->maxDist);
		//std::cout <<"image " << image << ' ' << loc.x << ' ' << loc.z << endl;
		charColor color = getPixelDataSafe<charColor>(image, loc.x, loc.z, charColorBLACK);
		return Color(color.red/255.0, color.green/255.0, color.blue/255.0);
	}
};

class ImageMatirial4 : public Matirial4 {
public:
	void* image;
	Matrix5 mat;
	inline ImageMatirial4(Matrix5 mat, void* image) : mat(mat), image(image) {}
	virtual ~ImageMatirial4() {}
	virtual Color doShadeColorRay(Ray4* ray, Ray4* ecadianRay, RenderEnv4* env, RenderStackFrame4* frame) {
		Vector3 loc = mat * (ray->loc+ray->dir*ray->maxDist);
		charColor color = getPixelDataSafe<charColor>(image, loc.x, loc.z, charColorBLACK);

		return Color(color.red/255.0, color.green/255.0, color.blue/255.0);
	}
};



#endif /* MATIRIALS_DEBUGMATIRIAL_H_ */
