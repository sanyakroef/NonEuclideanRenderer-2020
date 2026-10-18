/*
 * Triangle.h
 *
 *  Created on: 6 Dec 2019
 *      Author: christiaan
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../Vector.h"
#include "../Ray.h"

class Triangle3 {
private:
	Vector3 norm;
public:
	Vector3 loc[3];
	inline Triangle3(Vector3 loc1, Vector3 loc2, Vector3 loc3) {
		loc[0] = loc1;
		loc[1] = loc2;
		loc[2] = loc3;
	}
	inline Triangle3(){}
	inline ~Triangle3(){}
	Ray3IntersectReturn intersect(const Ray3* ray);
	inline void setup() {
		Vector3 v0v1 = loc[1] - loc[0];
		Vector3 v0v2 = loc[2] - loc[0];
		norm = v0v1.crossProduct(v0v2);
	}
};

class Triangle4 {
private:
	Vector4 norm;
public:
	Vector4 loc[4];
	inline Triangle4(Vector4 loc1, Vector4 loc2, Vector4 loc3, Vector4 loc4) {
		loc[0] = loc1;
		loc[1] = loc2;
		loc[2] = loc3;
		loc[3] = loc4;
	}
	inline Triangle4(){}
	inline ~Triangle4(){}
	Ray4IntersectReturn intersect(const Ray4* ray);
	inline void setup() {
		Vector4 v0v1 = loc[1] - loc[0];
		Vector4 v0v2 = loc[2] - loc[0];
		Vector4 v0v3 = loc[3] - loc[0];
		norm = v0v1.crossProduct(v0v2, v0v3);
	}
};



#endif /* TRIANGLE_H_ */

/*
 * todo
 * rotate cros 4d
 * shader (class)
 *
 */
