/*
 * PerspectiveCamera.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef CAMERAS_PERSPECTIVECAMERANOISE_H_
#define CAMERAS_PERSPECTIVECAMERANOISE_H_

#include "../Camera.h"
#include "../Color.h"
#include "../RenderEnv.h"

class PerspectiveCameraNoise3 : public Camera3 {
public:
	Vector3 org;
	double ofsetX;
	double scaleX;
	double ofsetY;
	double scaleY;
	inline PerspectiveCameraNoise3() : ofsetX(1), scaleX(1), ofsetY(1), scaleY(1) {}
	virtual ~PerspectiveCameraNoise3() {}
	virtual void setSize(unsigned width, unsigned height) {
		ofsetX = width/2.0;
		scaleX = 1/ofsetX;
		ofsetX = -ofsetX;

		ofsetY = height/-2.0;
		scaleY = -scaleX;
	}
	virtual Color renderPixel(unsigned x, unsigned y, RenderEnv3* env, Noise::Random& r) {
		Ray3 ray (org, Vector3((x+ofsetX+(r.nextInt()/2.0/INT_MAX))*scaleX, 1, (y+ofsetY+(r.nextInt()/2.0/INT_MAX))*scaleY).normalize());
		RenderStackFrame3 frame (ColorRayType);
		return env->doRayColor(&ray, &frame);
	}
};

class PerspectiveCameraNoise4 : public Camera4 {
public:
	Vector3 org;
	double ofsetX;
	double scaleX;
	double ofsetY;
	double scaleY;
	inline PerspectiveCameraNoise4() : ofsetX(1), scaleX(1), ofsetY(1), scaleY(1) {}
	virtual ~PerspectiveCameraNoise4() {}
	virtual void setSize(unsigned width, unsigned height) {
		ofsetX = width/2.0;
		scaleX = 1/ofsetX;
		ofsetX = -ofsetX;

		ofsetY = height/-2.0;
		scaleY = -scaleX;
	}
	virtual Color renderPixel(unsigned x, unsigned y, RenderEnv4* env, Noise::Random& r) {
		Ray4 ray (org, Vector4((x+ofsetX+(r.nextInt()/2.0/INT_MAX))*scaleX, 1, (y+ofsetY+(r.nextInt()/2.0/INT_MAX))*scaleY, 0).normalize());
		RenderStackFrame4 frame (ColorRayType);
		return env->doRayColor(&ray, &frame);
	}
};


#endif /* CAMERAS_PERSPECTIVECAMERA_H_ */
