/*
 * CustomMatirial.h
 *
 *  Created on: 17 Jan 2020
 *      Author: christiaan
 */

#ifndef MATIRIALS_CUSTOMMATIRIAL_H_
#define MATIRIALS_CUSTOMMATIRIAL_H_

#include "../Matirial.h"
#include <iostream>

class CustomMatirail1_3 : public Matirial3 {
public:
	Matirial3* mat1;
	Matirial3* mat2;
	double scale;
	int* primes;
	int primesCount;
	inline CustomMatirail1_3(Matirial3* mat1, Matirial3* mat2, double scale, int* primes, int primesCount) : mat1(mat1), mat2(mat2), scale(scale), primes(primes), primesCount(primesCount) {}
	virtual ~CustomMatirail1_3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		Vector3 org = ray->loc+ray->dir*ray->maxDist;
		org *= scale;
		org += 0.01;
		int v = ((int)org.x) ^ ((int)org.y) ^ ((int)org.z);
		bool b = false;
		for (int i = 0;i<primesCount;i++)
			b ^= ((v % primes[i]) == 0);

		if (b)
			return mat1->doShadeColorRay(ray, ecadianRay, env, frame);
		else
			return mat2->doShadeColorRay(ray, ecadianRay, env, frame);
	}
};

class CustomMatirail2_3 : public Matirial3 {
public:
	Matirial3* mat1;
	Matirial3* mat2;
	double scale;
	int* primes;
	int primesCount;
	inline CustomMatirail2_3(Matirial3* mat1, Matirial3* mat2, double scale, int* primes, int primesCount) : mat1(mat1), mat2(mat2), scale(scale), primes(primes), primesCount(primesCount) {}
	virtual ~CustomMatirail2_3() {}
	virtual Color doShadeColorRay(Ray3* ray, Ray3* ecadianRay, RenderEnv3* env, RenderStackFrame3* frame) {
		Ray3* rr = ecadianRay != nullptr ? ecadianRay : ray;
		Vector3 org = rr->loc+rr->dir*rr->maxDist;
		org *= scale;
		org += 0.01;
		int v = ((int)org.x) ^ ((int)org.y) ^ ((int)org.z);
		bool b = false;
		for (int i = 0;i<primesCount;i++)
			b ^= ((v % primes[i]) == 0);

		if (b)
			return mat1->doShadeColorRay(ray, ecadianRay, env, frame);
		else
			return mat2->doShadeColorRay(ray, ecadianRay, env, frame);
	}
};



#endif /* MATIRIALS_CUSTOMMATIRIAL_H_ */
