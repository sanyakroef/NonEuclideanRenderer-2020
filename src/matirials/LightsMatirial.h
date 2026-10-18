/*
 * AddMatirial.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef MATIRIALS_LIGHTSMATIRIAL_H_
#define MATIRIALS_LIGHTSMATIRIAL_H_

#include "../Matirial.h"
#include "../rendrables/Sphere.h"

class Light3 {
public:
	Sphere3 source;
	double strenght;
	inline Light3(Sphere3 source, double strenght) : source(source), strenght(strenght) {}
	inline ~Light3() {}
};

class LightsMatirialDirect3 : public Matirial3 {
public:
	Matirial3* mat;
	Light3* lights;
	unsigned lightsCount;
	inline LightsMatirialDirect3(Matirial3* mat, Light3* lights, unsigned lightsCount) : mat(mat), lights(lights), lightsCount(lightsCount) {}
	virtual ~LightsMatirialDirect3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		Vector3 org = ray->loc+ray->dir*ray->maxDist;
		Ray3* ecadianRay2 = ecadianRay == nullptr ? ray : ecadianRay;
		Vector3 org2 = ecadianRay2->loc+ecadianRay2->dir*ecadianRay2->maxDist;
		double light;
		if (lightsCount == 1) {
			Ray3 shadowRay (org, (lights->source.loc-org2).normalize());
			light = env->doShadowRay(&shadowRay, frame, &(lights->source));
		} else {
			//todo
			light = 0.5;
		}

		if (light < 0.00001)
			return Color(0);

		Color c = mat->doShadeColorRay(ray, ecadianRay, env, frame);
		return Color(c.red*light, c.green*light, c.blue*light, c.alpha);
	}
};

class LightsMatirial3 : public Matirial3 {
public:
	Matirial3* mat;
	Light3* lights;
	unsigned lightsCount;
	inline LightsMatirial3(Matirial3* mat, Light3* lights, unsigned lightsCount) : mat(mat), lights(lights), lightsCount(lightsCount) {}
	virtual ~LightsMatirial3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		Vector3 org = ray->loc+ray->dir*ray->maxDist;
		Ray3* ecadianRay2 = ecadianRay == nullptr ? ray : ecadianRay;
		Vector3 org2 = ecadianRay2->loc+ecadianRay2->dir*ecadianRay2->maxDist;
		double light;
		if (lightsCount == 1) {
			Ray3 shadowRay (org, (lights->source.loc-org2).normalize());
			light = env->doShadowRay(&shadowRay, frame, &(lights->source));
			if (light < 0.00001)
				return Color(0);

			light *= lights->strenght / (shadowRay.maxDist*shadowRay.maxDist);
			//light *= lights->strenght * shadowRay.dir.angleCos(0 - ray->dir) / (shadowRay.maxDist*shadowRay.maxDist);
		} else {
			//todo
			light = 0.5;
		}

		Color c = mat->doShadeColorRay(ray, ecadianRay, env, frame);
		return Color(c.red*light, c.green*light, c.blue*light, c.alpha);
	}
};

class LightsMatirialBounded3 : public Matirial3 {
public:
	Matirial3* mat;
	Light3* lights;
	unsigned lightsCount;
	inline LightsMatirialBounded3(Matirial3* mat, Light3* lights, unsigned lightsCount) : mat(mat), lights(lights), lightsCount(lightsCount) {}
	virtual ~LightsMatirialBounded3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		Vector3 org = ray->loc+ray->dir*ray->maxDist;
		Ray3* ecadianRay2 = ecadianRay == nullptr ? ray : ecadianRay;
		Vector3 org2 = ecadianRay2->loc+ecadianRay2->dir*ecadianRay2->maxDist;
		double light;
		if (lightsCount == 1) {
			Ray3 shadowRay (org, (lights->source.loc-org2).normalize());
			light = env->doShadowRay(&shadowRay, frame, &(lights->source));
			if (light < 0.00001)
				return Color(0);

			light *= 1 / ((shadowRay.maxDist*shadowRay.maxDist) / lights->strenght + 1);
			//light *= shadowRay.dir.angleCos(0 - ray->dir) / ((shadowRay.maxDist*shadowRay.maxDist) / lights->strenght + 1);
		} else {
			//todo
			light = 0.5;
		}

		Color c = mat->doShadeColorRay(ray, ecadianRay, env, frame);
		return Color(c.red*light, c.green*light, c.blue*light, c.alpha);
	}
};

class LightsMatirialGlobal3 : public Matirial3 {
public:
	Matirial3* mat;
	Vector3 angle;
	inline LightsMatirialGlobal3(Matirial3* mat, Vector3 angle) : mat(mat), angle(angle) {}
	virtual ~LightsMatirialGlobal3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		double light = ray->dir.angleCos(angle);

		Color c = mat->doShadeColorRay(ray, ecadianRay, env, frame);
		return Color(c.red*light, c.green*light, c.blue*light, c.alpha);
	}
};


#endif /* MATIRIALS_ADDMATIRIAL_H_ */
