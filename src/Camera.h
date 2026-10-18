/*
 * Camera.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "noise/Random.h"
#include "Matrix.h"

class RenderEnv3;
class Color;
class Camera3 {
public:
	inline Camera3() {}
	virtual ~Camera3() {}
	virtual void setSize(unsigned width, unsigned height) = 0;
	virtual void setMatrix(Matrix4 matrix) = 0;
	virtual Color renderPixel(unsigned x, unsigned y, RenderEnv3* env, Noise::Random& r) = 0;
};

class RenderEnv4;
class Camera4 {
public:
	inline Camera4() {}
	virtual ~Camera4() {}
	virtual void setSize(unsigned width, unsigned height) = 0;
	virtual void setMatrix(Matrix5 matrix) = 0;
	virtual Color renderPixel(unsigned x, unsigned y, RenderEnv4* env, Noise::Random& r) = 0;
};

#endif /* CAMERA_H_ */
