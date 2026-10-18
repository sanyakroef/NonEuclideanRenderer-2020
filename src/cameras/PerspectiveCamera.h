/*
 * PerspectiveCamera.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef CAMERAS_PERSPECTIVECAMERA_H_
#define CAMERAS_PERSPECTIVECAMERA_H_

#include "../Camera.h"
#include "../Color.h"
#include "../RenderEnv.h"
#include <iostream>

class PerspectiveCamera3 : public Camera3 {
public:
	Matrix4 matrix;
	double ofsetX;
	double scaleX;
	double ofsetY;
	double scaleY;

	double Xof;
	double Yof;
	inline PerspectiveCamera3() : ofsetX(1), scaleX(1), ofsetY(1), scaleY(1), Xof(0), Yof(0) {}
	virtual ~PerspectiveCamera3() {}
	virtual void setSize(unsigned width, unsigned height) {
		ofsetX = width/2.0;
		scaleX = 1/ofsetX;
		ofsetX = -ofsetX+Xof;

		ofsetY = height/-2.0+Yof;
		scaleY = -scaleX;
	}
	virtual void setMatrix(Matrix4 matrix) {
		this->matrix = matrix;
	}
	virtual Color renderPixel(unsigned x, unsigned y, RenderEnv3* env, Noise::Random& r) {
		Ray3 ray (matrix.asLoc()
				, singleMult(  Vector3((x+ofsetX)*scaleX, 1, (y+ofsetY)*scaleY).normalize(),   matrix));
		RenderStackFrame3 frame (ColorRayType);
		//std::cout << x << ' ' << y << std::endl;
		return env->doRayColor(&ray, &frame);
	}
};

class PerspectiveCamera4 : public Camera4 {
public:
	Matrix5 matrix;
	double ofsetX;
	double scaleX;
	double ofsetY;
	double scaleY;

	double Xof;
	double Yof;
	inline PerspectiveCamera4() : ofsetX(1), scaleX(1), ofsetY(1), scaleY(1), Xof(0), Yof(0) {}
	virtual ~PerspectiveCamera4() {}
	virtual void setSize(unsigned width, unsigned height) {
		ofsetX = width/2.0;
		scaleX = 1/ofsetX;
		ofsetX = -ofsetX+Xof;

		ofsetY = height/-2.0+Yof;
		scaleY = -scaleX;
	}
	virtual void setMatrix(Matrix5 matrix) {
			this->matrix = matrix;
		}
	virtual Color renderPixel(unsigned x, unsigned y, RenderEnv4* env, Noise::Random& r) {
		Ray4 ray (matrix.asLoc()
				, singleMult(  Vector4((x+ofsetX)*scaleX, 1, (y+ofsetY)*scaleY, 0).normalize(),   matrix));
		RenderStackFrame4 frame (ColorRayType);
		return env->doRayColor(&ray, &frame);
	}
};


#endif /* CAMERAS_PERSPECTIVECAMERA_H_ */
