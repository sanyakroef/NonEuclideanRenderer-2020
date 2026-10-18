/*
 * Geromitry.h
 *
 *  Created on: 6 Dec 2019
 *      Author: christiaan
 */

#ifndef GEROMITRY_H_
#define GEROMITRY_H_

#include <cstdint>

class Triangle3;
class Sphere3;
class AABBcube3;
class VirtualSpace3;

class Triangle4;
class Sphere4;
class AABBcube4;
class VirtualSpace4;

class Geromitry3 {
public:
	enum uint8_t {
		NoneType,
		TriangleType,
		SphereType,
		AABBcubeType,
		VirtualSpaceType,
	} type;
	void* data;
	inline Geromitry3() : type(NoneType), data(nullptr) {}
	inline Geromitry3(Triangle3* triangle) : type(TriangleType), data(triangle) {}
	inline Geromitry3(Sphere3* sphere) : type(SphereType), data(sphere) {}
	inline Geromitry3(AABBcube3* cube) : type(AABBcubeType), data(cube) {}
	inline Geromitry3(VirtualSpace3* vspace) : type(VirtualSpaceType), data(vspace) {}
};

class Geromitry4 {
public:
	enum uint8_t {
		NoneType,
		TriangleType,
		SphereType,
		AABBcubeType,
		VirtualSpaceType,
	} type;
	void* data;
	inline Geromitry4() : type(NoneType), data(nullptr) {}
	inline Geromitry4(Triangle4* triangle) : type(TriangleType), data(triangle) {}
	inline Geromitry4(Sphere4* sphere) : type(SphereType), data(sphere) {}
	inline Geromitry4(AABBcube4* cube) : type(AABBcubeType), data(cube) {}
	inline Geromitry4(VirtualSpace4* vspace) : type(VirtualSpaceType), data(vspace) {}
};

class Matirial3;
class FullGeromitry3 {
public:
	Geromitry3 gerom;
	Matirial3* mat;
	inline FullGeromitry3(const Geromitry3& gerom, Matirial3* mat) : gerom(gerom), mat(mat) {}
	inline FullGeromitry3() : gerom(), mat(nullptr) {}
	inline ~FullGeromitry3() {}
};

class Matirial4;
class FullGeromitry4 {
public:
	Geromitry4 gerom;
	Matirial4* mat;
	inline FullGeromitry4(const Geromitry4& gerom, Matirial4* mat) : gerom(gerom), mat(mat) {}
	inline FullGeromitry4() : gerom(), mat(nullptr) {}
	inline ~FullGeromitry4() {}
};



#endif /* GEROMITRY_H_ */
