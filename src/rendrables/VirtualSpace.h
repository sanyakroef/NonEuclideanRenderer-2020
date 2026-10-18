/*
 * VirtualSpace.h
 *
 *  Created on: 7 Feb 2020
 *      Author: christiaan
 */

#ifndef RENDRABLES_VIRTUALSPACE_H_
#define RENDRABLES_VIRTUALSPACE_H_

#include "../Vector.h"
#include "../Matrix.h"
#include "../Ray.h"
#include "../Geromitry.h"

class VirtualSpace3 {
public:
	Matrix4 mat;
	Matrix4 matI;
	Geromitry3 gero;
	inline VirtualSpace3(Geromitry3 gero, Matrix4 mat, Matrix4 matI) {this->gero = gero;this->mat = mat;this->matI = matI;}
};

class VirtualSpace4 {
public:
	Matrix5 mat;
	Matrix5 matI;
	Geromitry4 gero;
	inline VirtualSpace4(Geromitry4 gero, Matrix5 mat, Matrix5 matI) {this->gero = gero;this->mat = mat;this->matI = matI;}
};


#endif /* RENDRABLES_VIRTUALSPACE_H_ */
