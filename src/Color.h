/*
 * Color.h
 *
 *  Created on: 10 Dec 2019
 *      Author: christiaan
 */

#ifndef COLOR_H_
#define COLOR_H_

#include "Vector.h"

//pre multyplied with alpha
class Color {
public:
	double red;
	double green;
	double blue;
	double alpha;
	inline Color(double red, double green, double blue, double alpha) : red(red), green(green), blue(blue), alpha(alpha) {}
	inline Color(double red, double green, double blue) : red(red), green(green), blue(blue), alpha(1) {}
	inline Color(double gray) : red(gray), green(gray), blue(gray), alpha(1) {}
	inline ~Color() {}
	inline Color combine(Color c) {
		double Ialpha = 1-alpha;
		return Color(c.red*Ialpha + red,
				c.green*Ialpha + green,
				c.blue*Ialpha + blue,
				1 - (1 - c.alpha) * Ialpha);
	}
};
inline Color LiniarColor(double red, double green, double blue, double alpha) {
	return Color(red*alpha, green*alpha, blue*alpha, alpha);
}
inline Color LiniarColor(Vector4 v) {
	return Color(v.x*v.m, v.y*v.m, v.z*v.m, v.m);
}
inline Vector4 LiniarColor(Color c) {
	if (c.alpha == 0)
		return Vector4();
	else
		return Vector4(c.red/c.alpha, c.green/c.alpha, c.blue/c.alpha, c.alpha);
}


#endif /* COLOR_H_ */
