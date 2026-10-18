/*
 * AABBcube.h
 *
 *  Created on: 13 Dec 2019
 *      Author: christiaan
 */

#ifndef RENDRABLES_AABBCUBE_H_
#define RENDRABLES_AABBCUBE_H_

#include "../Vector.h"
#include "../Ray.h"

class AABBcube3 {
public:
	Vector3 min;
	Vector3 max;
	inline AABBcube3() {}
	inline AABBcube3(Vector3 min, Vector3 max) : min(min), max(max) {}
	inline AABBcube3(Vector3 min) : min(min), max(min+1) {}
	inline AABBcube3(Vector3 min, double size) : min(min), max(min+size) {}
	inline ~AABBcube3() {}
	Ray3IntersectReturn intersect(const Ray3* ray) {
		if (ray->loc.x >= min.x && ray->loc.y >= min.y && ray->loc.z >= min.z && ray->loc.x <= max.x && ray->loc.y <= max.y && ray->loc.z <= max.z) {
			//inside
			Vector3 invRayDir = 1/ray->dir;
			Vector3 t0 = (min - ray->loc) * invRayDir;
			Vector3 t1 = (max - ray->loc) * invRayDir;
			Vector3 tmin = t0.min(t1), tmax = t0.max(t1);
			//double best = DBL_MAX;//tmax.min_component()
			/*
			if (tmax.x > 0.00001) best = std::min(best, tmax.x);
			if (tmax.y > 0.00001) best = std::min(best, tmax.y);
			if (tmax.z > 0.00001) best = std::min(best, tmax.z);
			*/
			/*
			best = tmax.x > 0.00001 ? std::min(best, tmax.x) : std::max(best, tmin.x);
			best = tmax.y > 0.00001 ? std::min(best, tmax.y) : std::max(best, tmin.y);
			best = tmax.z > 0.00001 ? std::min(best, tmax.z) : std::max(best, tmin.z);
			*/
			return Ray3IntersectReturn(tmax.min_component(), Vector3(-1,0,0));
		}
		Vector3 invRayDir = 1/ray->dir;
		Vector3 t0 = (min - ray->loc) * invRayDir;
		Vector3 t1 = (max - ray->loc) * invRayDir;
		Vector3 tmin = t0.min(t1), tmax = t0.max(t1);
		if (tmin.max_component() > tmax.min_component()) return Ray3IntersectReturn();
		return Ray3IntersectReturn(tmin.max_component(), Vector3());
	}
	inline Vector3 getNorm(const Ray3* ray) {
		Vector3 intersect = ray->loc+ray->dir*ray->maxDist;
		Vector3 t0 = (min - intersect).abs();
		Vector3 t1 = (max - intersect).abs();

		double best = t0.x;
		Vector3 norm = Vector3(-1, 0, 0);
		if (best > t0.y) {best = t0.y;norm = Vector3(0, -1, 0);}
		if (best > t0.z) {best = t0.z;norm = Vector3(0, 0, -1);}

		if (best > t1.x) {best = t1.x;norm = Vector3(1, 0, 0);}
		if (best > t1.y) {best = t1.y;norm = Vector3(0, 1, 0);}
		if (best > t1.z) {best = t1.z;norm = Vector3(0, 0, 1);}

		if (ray->norm.x == -1)//inside
			norm = 0-norm;

		return norm;
	}
};

/*
 * float max(vec3 v) { return max (max(v.x, v.y), v.z); }
// box.rotation = object-to-world, invRayDir unused if oriented
bool ourIntersectBox(Box box, Ray ray, out float distance, out vec3 normal,
const bool canStartInBox, const in bool oriented, in vec3 _invRayDir) {
ray.origin = ray.origin - box.center;
if (oriented) { ray.dir *= box.rot; ray.origin *= box.rot; }
float winding = canStartInBox && (max(abs(ray.origin) * box.invRadius)
< 1.0) ? -1 : 1;
vec3 sgn = -sign(ray.dir);
// Distance to plane
vec3 d = box.radius * winding * sgn - ray.origin;
if (oriented) d /= ray.dir; else d *= _invRayDir;
# define TEST(U, VW) (d.U >= 0.0) && \
all(lessThan(abs(ray.origin.VW + ray.dir.VW * d.U), box.radius.VW))
bvec3 test = bvec3(TEST(x, yz), TEST(y, zx), TEST(z, xy));
sgn = test.x ? vec3(sgn.x,0,0) : (test.y ? vec3(0,sgn.y,0) :
vec3(0,0,test.z ? sgn.z:0));
# undef TEST
distance = (sgn.x != 0) ? d.x : ((sgn.y != 0) ? d.y : d.z);
normal = oriented ? (box.rot * sgn) : sgn;
return
 */







class AABBcube4 {
public:
	Vector4 min;
	Vector4 max;
	inline AABBcube4() {}
	inline AABBcube4(Vector4 min, Vector4 max) : min(min), max(max) {}
	inline AABBcube4(Vector4 min) : min(min), max(min+1) {}
	inline AABBcube4(Vector4 min, double size) : min(min), max(min+size) {}
	inline ~AABBcube4() {}
	Ray4IntersectReturn intersect(const Ray4* ray) {
		if (ray->loc.x >= min.x && ray->loc.y >= min.y && ray->loc.z >= min.z && ray->loc.m >= min.m && ray->loc.x <= max.x && ray->loc.y <= max.y && ray->loc.z <= max.z && ray->loc.m <= max.m) {
			//inside
			Vector4 invRayDir = 1/ray->dir;
			Vector4 t0 = (min - ray->loc) * invRayDir;
			Vector4 t1 = (max - ray->loc) * invRayDir;
			Vector4 tmin = t0.min(t1), tmax = t0.max(t1);
			//double best = DBL_MAX;//tmax.min_component()
			/*
			if (tmax.x > 0.00001) best = std::min(best, tmax.x);
			if (tmax.y > 0.00001) best = std::min(best, tmax.y);
			if (tmax.z > 0.00001) best = std::min(best, tmax.z);
			*/
			/*
			best = tmax.x > 0.00001 ? std::min(best, tmax.x) : std::max(best, tmin.x);
			best = tmax.y > 0.00001 ? std::min(best, tmax.y) : std::max(best, tmin.y);
			best = tmax.z > 0.00001 ? std::min(best, tmax.z) : std::max(best, tmin.z);
			*/
			return Ray4IntersectReturn(tmax.min_component(), Vector4(-1,0,0,0));
		}
		Vector4 invRayDir = 1/ray->dir;
		Vector4 t0 = (min - ray->loc) * invRayDir;
		Vector4 t1 = (max - ray->loc) * invRayDir;
		Vector4 tmin = t0.min(t1), tmax = t0.max(t1);
		if (tmin.max_component() > tmax.min_component()) return Ray4IntersectReturn();
		return Ray4IntersectReturn(tmin.max_component(), Vector4());
	}
	inline Vector4 getNorm(const Ray4* ray) {
		Vector4 intersect = ray->loc+ray->dir*ray->maxDist;
		Vector4 t0 = (min - intersect).abs();
		Vector4 t1 = (max - intersect).abs();

		double best = t0.x;
		Vector4 norm = Vector4(-1, 0, 0, 0);
		if (best > t0.y) {best = t0.y;norm = Vector4(0, -1, 0, 0);}
		if (best > t0.z) {best = t0.z;norm = Vector4(0, 0, -1, 0);}
		if (best > t0.m) {best = t0.m;norm = Vector4(0, 0, 0, -1);}

		if (best > t1.x) {best = t1.x;norm = Vector4(1, 0, 0, 0);}
		if (best > t1.y) {best = t1.y;norm = Vector4(0, 1, 0, 0);}
		if (best > t1.z) {best = t1.z;norm = Vector4(0, 0, 1, 0);}
		if (best > t1.m) {best = t1.m;norm = Vector4(0, 0, 0, 1);}

		if (ray->norm.x == -1)//inside
			norm = 0-norm;

		return norm;
	}
};



#endif /* RENDRABLES_AABBCUBE_H_ */
