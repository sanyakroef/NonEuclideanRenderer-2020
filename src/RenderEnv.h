/*
 * RenderEnv.h
 *
 *  Created on: 11 Dec 2019
 *      Author: christiaan
 */

#ifndef RENDERENV_H_
#define RENDERENV_H_

#include "Color.h"
#include "Ray.h"
#include "Geromitry.h"
#include <vector>

enum RayType : uint8_t {
	HitRayType,
	ColorRayType,
	ShadowRayType
};

class RenderStackFrame3 {
public:
	double depf;
	unsigned rayCount;
	unsigned specificRayCount;
	unsigned noEndian;
	unsigned allRayCount;
	double value;
	RayType type;
	inline RenderStackFrame3(RayType type) : depf(0), rayCount(0), specificRayCount(0), noEndian(0), allRayCount(0), value(1), type(type) {}
	inline RenderStackFrame3(double depf, unsigned rayCount, unsigned specificRayCount, RayType type, unsigned noEndian, unsigned allRayCount, double value) : depf(depf), rayCount(rayCount), specificRayCount(specificRayCount), noEndian(noEndian), allRayCount(allRayCount), value(value), type(type) {}
	inline ~RenderStackFrame3() {}
	inline RenderStackFrame3 addRay(const Ray3* org) const {
		return RenderStackFrame3(depf+org->maxDist, rayCount+1, specificRayCount+1, type, 0, allRayCount+1, value);
	}
	inline RenderStackFrame3 addRay(const Ray3* org, RayType type) const {
		if (this->type == type)
			return RenderStackFrame3(depf+org->maxDist, rayCount+1, specificRayCount+1, type, 0, allRayCount+1, value);
		else
			return RenderStackFrame3(depf+org->maxDist, rayCount+1, 0, type, 0, allRayCount+1, value);
	}
	inline RenderStackFrame3 addRay(const Ray3* org, double value) const {
		return RenderStackFrame3(depf+org->maxDist, rayCount+1, specificRayCount+1, type, 0, allRayCount+1, this->value*value);
	}
	inline RenderStackFrame3 addRay(const Ray3* org, RayType type, double value) const {
		if (this->type == type)
			return RenderStackFrame3(depf+org->maxDist, rayCount+1, specificRayCount+1, type, 0, allRayCount+1, this->value*value);
		else
			return RenderStackFrame3(depf+org->maxDist, rayCount+1, 0, type, 0, allRayCount+1, this->value*value);
	}
	inline RenderStackFrame3 addNonEdian(double depf) {
		return RenderStackFrame3(this->depf+depf, rayCount, specificRayCount, type, noEndian+1, allRayCount+1, value);
	}
	inline RenderStackFrame3 addNonEdian(double depf, double value) {
		return RenderStackFrame3(this->depf+depf, rayCount, specificRayCount, type, noEndian+1, allRayCount+1, this->value*value);
	}
};
typedef bool (*RayVerivirer3)(RenderStackFrame3*);
class RenderEnv3 {
public:
	RayVerivirer3 alowRay;
	inline RenderEnv3(RayVerivirer3 alowRay) : alowRay(alowRay) {}
	virtual ~RenderEnv3() {}
	virtual void flushEnv(Ray3* ray) = 0;
	virtual Color doRayColor(Ray3* ray, Ray3* ecadianRay, RenderStackFrame3* frame) = 0;
	inline Color doRayColor(Vector3 loc, Vector3 dir, Ray3* ecadianRay, RenderStackFrame3* frame) {
		Ray3 ray (loc, dir);
		return doRayColor(&ray, ecadianRay, frame);
	}
	inline Color doRayColor(Ray3* ray, RenderStackFrame3* frame) {
		return doRayColor(ray, nullptr, frame);
	}
	inline Color doRayColor(Vector3 loc, Vector3 dir, RenderStackFrame3* frame) {
		Ray3 ray (loc, dir);
		return doRayColor(&ray, nullptr, frame);
	}
	//obstruction %
	virtual double doShadowRay(Ray3* ray, RenderStackFrame3* frame, Sphere3* dest) = 0;
};

class RenderEnvBasic3 : public RenderEnv3 {
public:
	std::vector<FullGeromitry3> geromitry;
	inline RenderEnvBasic3(RayVerivirer3 alowRay) : RenderEnv3(alowRay) {}
	virtual ~RenderEnvBasic3() {}
	virtual void flushEnv(Ray3* ray) {
		ray->parseGeromitry(geromitry.data(), geromitry.size());
	}
	inline void addGeromitry(FullGeromitry3 ger) {geromitry.push_back(ger);}
	inline void addGeromitry(FullGeromitry3* data, unsigned size) {
		for (unsigned i = 0;i<size;i++)
			addGeromitry(data[i]);
	}
	inline void addGeromitry(Triangle3* tri, Matirial3* mat) {geromitry.push_back(FullGeromitry3(Geromitry3(tri), mat));}
	inline void addGeromitry(Triangle3* data, Matirial3* mat, unsigned size) {
		for (unsigned i = 0;i<size;i++)
			addGeromitry(data+i, mat);
	}
	virtual double doShadowRay(Ray3* ray, RenderStackFrame3* frame, Sphere3* dest) {
		if (alowRay(frame)) {
			flushEnv(ray);
			auto ret = dest->intersect(ray);
			if (ret.dist <= ray->maxDist && ret.dist > 0.00001) {
				ray->maxDist = ret.dist;
				return 1;
			}
			else if (ray->maxDist == DBL_MAX)
				return 0;
			else {
				ray->finish();
				ray->norm = ray->norm.normalize();
				RenderStackFrame3 cframe = frame->addRay(ray, ShadowRayType);
				return ray->geromitry.mat->doObstructionRay(ray, this, &cframe, dest);
			}
		} else
			return 0;
	}
};










class RenderStackFrame4 {
public:
	double depf;
	unsigned rayCount;
	unsigned specificRayCount;
	unsigned noEndian;
	unsigned allRayCount;
	double value;
	RayType type;
	inline RenderStackFrame4(RayType type) : depf(0), rayCount(0), specificRayCount(0), noEndian(0), allRayCount(0), value(1), type(type) {}
	inline RenderStackFrame4(double depf, unsigned rayCount, unsigned specificRayCount, RayType type, unsigned noEndian, unsigned allRayCount, double value) : depf(depf), rayCount(rayCount), specificRayCount(specificRayCount), noEndian(noEndian), allRayCount(allRayCount), value(value), type(type) {}
	inline ~RenderStackFrame4() {}
	inline RenderStackFrame4 addRay(const Ray4* org) const {
		return RenderStackFrame4(depf+org->maxDist, rayCount+1, specificRayCount+1, type, 0, allRayCount+1, value);
	}
	inline RenderStackFrame4 addRay(const Ray4* org, RayType type) const {
		if (this->type == type)
			return RenderStackFrame4(depf+org->maxDist, rayCount+1, specificRayCount+1, type, 0, allRayCount+1, value);
		else
			return RenderStackFrame4(depf+org->maxDist, rayCount+1, 0, type, 0, allRayCount+1, value);
	}
	inline RenderStackFrame4 addRay(const Ray4* org, double value) const {
		return RenderStackFrame4(depf+org->maxDist, rayCount+1, specificRayCount+1, type, 0, allRayCount+1, this->value*value);
	}
	inline RenderStackFrame4 addRay(const Ray4* org, RayType type, double value) const {
		if (this->type == type)
			return RenderStackFrame4(depf+org->maxDist, rayCount+1, specificRayCount+1, type, 0, allRayCount+1, this->value*value);
		else
			return RenderStackFrame4(depf+org->maxDist, rayCount+1, 0, type, 0, allRayCount+1, this->value*value);
	}
	inline RenderStackFrame4 addNonEdian(double depf) {
		return RenderStackFrame4(this->depf+depf, rayCount, specificRayCount, type, noEndian+1, allRayCount+1, value);
	}
	inline RenderStackFrame4 addNonEdian(double depf, double value) {
		return RenderStackFrame4(this->depf+depf, rayCount, specificRayCount, type, noEndian+1, allRayCount+1, this->value*value);
	}
};
typedef bool (*RayVerivirer4)(RenderStackFrame4*);
class RenderEnv4 {
public:
	RayVerivirer4 alowRay;
	inline RenderEnv4(RayVerivirer4 alowRay) : alowRay(alowRay) {}
	virtual ~RenderEnv4() {}
	virtual void flushEnv(Ray4* ray) = 0;
	virtual Color doRayColor(Ray4* ray, Ray4* ecadianRay, RenderStackFrame4* frame) = 0;
	inline Color doRayColor(Vector4 loc, Vector4 dir, Ray4* ecadianRay, RenderStackFrame4* frame) {
		Ray4 ray (loc, dir);
		return doRayColor(&ray, ecadianRay, frame);
	}
	inline Color doRayColor(Ray4* ray, RenderStackFrame4* frame) {
		return doRayColor(ray, nullptr, frame);
	}
	inline Color doRayColor(Vector4 loc, Vector4 dir, RenderStackFrame4* frame) {
		Ray4 ray (loc, dir);
		return doRayColor(&ray, nullptr, frame);
	}
	//obstruction %
	virtual double doShadowRay(Ray4* ray, RenderStackFrame4* frame, Sphere4* dest) = 0;
};

class RenderEnvBasic4 : public RenderEnv4 {
public:
	std::vector<FullGeromitry4> geromitry;
	inline RenderEnvBasic4(RayVerivirer4 alowRay) : RenderEnv4(alowRay) {}
	virtual ~RenderEnvBasic4() {}
	virtual void flushEnv(Ray4* ray) {
		ray->parseGeromitry(geromitry.data(), geromitry.size());
	}
	inline void addGeromitry(FullGeromitry4 ger) {geromitry.push_back(ger);}
	inline void addGeromitry(FullGeromitry4* data, unsigned size) {
		for (unsigned i = 0;i<size;i++)
			addGeromitry(data[i]);
	}
	inline void addGeromitry(Triangle4* tri, Matirial4* mat) {geromitry.push_back(FullGeromitry4(Geromitry4(tri), mat));}
	inline void addGeromitry(Triangle4* data, Matirial4* mat, unsigned size) {
		for (unsigned i = 0;i<size;i++)
			addGeromitry(data+i, mat);
	}
	virtual double doShadowRay(Ray4* ray, RenderStackFrame4* frame, Sphere4* dest) {
		if (alowRay(frame)) {
			flushEnv(ray);
			auto ret = dest->intersect(ray);
			if (ret.dist <= ray->maxDist && ret.dist > 0.00001) {
				ray->maxDist = ret.dist;
				return 1;
			}
			else if (ray->maxDist == DBL_MAX)
				return 0;
			else {
				ray->finish();
				ray->norm = ray->norm.normalize();
				RenderStackFrame4 cframe = frame->addRay(ray, ShadowRayType);
				return ray->geromitry.mat->doObstructionRay(ray, this, &cframe, dest);
			}
		} else
			return 0;
	}
};



#endif /* RENDERENV_H_ */
