/*
 * Ray.cpp
 *
 *  Created on: 6 Dec 2019
 *      Author: christiaan
 */

#include "Ray.h"
#include "Geromitry.h"
#include "rendrables/Triangle.h"
#include "rendrables/Sphere.h"
#include "rendrables/AABBcube.h"
#include "rendrables/VirtualSpace.h"
#include <iostream>

Ray3IntersectReturn parseSingleGeromitry3S(Geromitry3 data, Ray3* ray) {
	switch (data.type) {
	case Geromitry3::TriangleType: {
		return ((Triangle3*)data.data)->intersect(ray);
	}
	case Geromitry3::SphereType: {
		return ((Sphere3*)data.data)->intersect(ray);
	}
	case Geromitry3::AABBcubeType: {
		return ((AABBcube3*)data.data)->intersect(ray);
	}
	case Geromitry3::VirtualSpaceType: {
		VirtualSpace3* ptr = ((VirtualSpace3*)data.data);
		Ray3 newRay (ray->loc * ptr->matI, singleMult(ray->dir, ptr->matI));
		Ray3IntersectReturn ret = parseSingleGeromitry3S(ptr->gero, &newRay);
		newRay.maxDist = ret.dist;
		newRay.geromitry = FullGeromitry3(ptr->gero, nullptr);
		newRay.norm = ret.norm;
		newRay.finish();
		return Ray3IntersectReturn(ret.dist, singleMult(newRay.norm, ptr->mat));
	}
	default:{
		std::cerr << data.type << std::endl;
		return Ray3IntersectReturn();
	}
	}
}

void parseSingleGeromitry3(FullGeromitry3 data, Ray3* ray) {
	Ray3IntersectReturn ret = parseSingleGeromitry3S(data.gerom, ray);
	//std::cout << ret.dist << std::endl;
	if (ret.dist > 0.00001 && ret.dist < ray->maxDist) {//0.00001
		ray->maxDist = ret.dist;
		ray->geromitry = data;
		ray->norm = ret.norm;
	}
}

void Ray3::finish() {
	switch (geromitry.gerom.type) {
	case Geromitry3::AABBcubeType: {
		norm = ((AABBcube3*)geromitry.gerom.data)->getNorm(this);
		break;
	}
	}
}

void Ray3::parseGeromitry(Geromitry3* data, unsigned size, Matirial3* mat) {
	for (unsigned i = 0;i<size;i++)
		parseSingleGeromitry3(FullGeromitry3(data[i], mat), this);
}

void Ray3::parseGeromitry(FullGeromitry3* data, unsigned size) {
	for (unsigned i = 0;i<size;i++)
		parseSingleGeromitry3(data[i], this);
	//if (maxDist != DBL_MAX && maxDist > 30)
	/*
	std::cout << "- " << dir.x << ' ' << dir.y << ' ' << dir.z << std::endl;
	Vector3 locTo = loc+dir*maxDist;
	std::cout << "- " << loc.x << ' ' << loc.y << ' ' << loc.z << std::endl;
	std::cout << "- " << locTo.x << ' ' << locTo.y << ' ' << locTo.z << std::endl;
	std::cout << "- " << norm.x << ' ' << norm.y << ' ' << norm.z << std::endl;
		std::cout << "> " << maxDist << '\n' << std::endl;
		*/
}

void Ray3::parseGeromitry(Triangle3* data, unsigned size, Matirial3* mat) {
	for (unsigned i = 0;i<size;i++) {
		Ray3IntersectReturn ret = data[i].intersect(this);
		if (ret.dist > 0.00001 && ret.dist < maxDist) {
			maxDist = ret.dist;
			geromitry = FullGeromitry3(Geromitry3(data+i), mat);
			norm = ret.norm;
		}
	}
}
/*
Color Ray3::finish(Ray3* ecadianRay) {
	if (maxDist == DBL_MAX)
		return Color(0,0,0,0);

	norm = norm.normalize();
	return geromitry.mat->doShadeColorRay(this, ecadianRay);
}
*/










Ray4IntersectReturn parseSingleGeromitry4S(Geromitry4 data, Ray4* ray) {
	switch (data.type) {
	case Geromitry4::TriangleType: {
		return ((Triangle4*)data.data)->intersect(ray);
	}
	case Geromitry4::SphereType: {
		return ((Sphere4*)data.data)->intersect(ray);
	}
	case Geromitry4::AABBcubeType: {
		return ((AABBcube4*)data.data)->intersect(ray);
	}
	case Geromitry4::VirtualSpaceType: {
		VirtualSpace4* ptr = ((VirtualSpace4*)data.data);
		Ray4 newRay (ray->loc * ptr->matI, singleMult(ray->dir, ptr->matI));
		Ray4IntersectReturn ret = parseSingleGeromitry4S(ptr->gero, &newRay);
		newRay.maxDist = ret.dist;
		newRay.geromitry = FullGeromitry4(ptr->gero, nullptr);
		newRay.norm = ret.norm;
		newRay.finish();
		return Ray4IntersectReturn(ret.dist, singleMult(newRay.norm, ptr->mat));
	}
	default:{
		std::cerr << data.type << std::endl;
		return Ray4IntersectReturn();
	}
	}
}

void parseSingleGeromitry4(FullGeromitry4 data, Ray4* ray) {
	Ray4IntersectReturn ret = parseSingleGeromitry4S(data.gerom, ray);
	//std::cout << ret.dist << std::endl;
	if (ret.dist > 0.00001 && ret.dist < ray->maxDist) {//0.00001
		ray->maxDist = ret.dist;
		ray->geromitry = data;
		ray->norm = ret.norm;
	}
}

void Ray4::finish() {
	switch (geromitry.gerom.type) {
	case Geromitry4::AABBcubeType: {
		norm = ((AABBcube4*)geromitry.gerom.data)->getNorm(this);
		break;
	}
	}
}

void Ray4::parseGeromitry(Geromitry4* data, unsigned size, Matirial4* mat) {
	for (unsigned i = 0;i<size;i++)
		parseSingleGeromitry4(FullGeromitry4(data[i], mat), this);
}

void Ray4::parseGeromitry(FullGeromitry4* data, unsigned size) {
	for (unsigned i = 0;i<size;i++)
		parseSingleGeromitry4(data[i], this);
}
