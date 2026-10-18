/*
 * Vector.h
 *
 *  Created on: 6 Dec 2019
 *      Author: christiaan
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <cmath>

namespace std {
inline double max(double a, double b) {
	return a>b? a : b;
}
inline double min(double a, double b) {
	return a<b? a : b;
}
/*
template <typename T> inline int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
*/
inline double sgn(double v) {
	return v == 0 ? 0 : v>0 ? 1 : -1;
}
}

class Vector3 {
public:
	double x;
	double y;
	double z;
	inline Vector3() : x(0), y(0), z(0) {}
	inline Vector3(double v) : x(v), y(v), z(v) {}
	inline Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
	inline Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z) {}
	inline ~Vector3(){}



	inline Vector3& operator+=(const Vector3& rhs) {
		x+=rhs.x;
		y+=rhs.y;
		z+=rhs.z;
		return *this;
	}
	inline Vector3& operator+=(const double& rhs) {
		x+=rhs;
		y+=rhs;
		z+=rhs;
		return *this;
	}

	inline Vector3& operator-=(const Vector3& rhs) {
		x-=rhs.x;
		y-=rhs.y;
		z-=rhs.z;
		return *this;
	}
	inline Vector3& operator-=(const double& rhs) {
		x-=rhs;
		y-=rhs;
		z-=rhs;
		return *this;
	}

	inline Vector3& operator*=(const Vector3& rhs) {
		x*=rhs.x;
		y*=rhs.y;
		z*=rhs.z;
		return *this;
	}
	inline Vector3& operator*=(const double& rhs) {
		x*=rhs;
		y*=rhs;
		z*=rhs;
		return *this;
	}

	inline Vector3& operator/=(const Vector3& rhs) {
		x/=rhs.x;
		y/=rhs.y;
		z/=rhs.z;
		return *this;
	}
	inline Vector3& operator/=(const double& rhs) {
		x/=rhs;
		y/=rhs;
		z/=rhs;
		return *this;
	}



	inline double dotProduct(const Vector3& vec) const {
		return x*vec.x + y*vec.y + z*vec.z;
	}
	inline Vector3 crossProduct(const Vector3& vec) const {
		Vector3 out (y * vec.z - z * vec.y, x * vec.z - z * vec.x, x * vec.y - y * vec.x);
		return out;
		//return Vector3(y * vec.z - z * vec.y, x * vec.z - z * vec.x, x * vec.y - y * vec.x);
	}
	inline Vector3 abs() const {
		Vector3 out (std::abs(x), std::abs(y), std::abs(z));
		return out;
	}
	inline double lenght() const {
		return std::sqrt(x*x+y*y+z*z);
	}
	inline double max_component() const {
		return std::max(x,std::max(y,z));
	}
	inline double min_component() const {
		return std::min(x,std::min(y,z));
	}
	inline Vector3 max(Vector3 vec) const {
		return Vector3(std::max(x, vec.x), std::max(y, vec.y), std::max(z, vec.z));
	}
	inline Vector3 min(Vector3 vec) const {
		return Vector3(std::min(x, vec.x), std::min(y, vec.y), std::min(z, vec.z));
	}
	inline Vector3 sgn() const {
		return Vector3(std::sgn(x), std::sgn(y), std::sgn(z));
	}
	inline Vector3 sgnInv() const {
		return Vector3(-std::sgn(x), -std::sgn(y), -std::sgn(z));
	}
	inline double angle(const Vector3& vec) {
		double dot = dotProduct(vec);
		dot = ( dot < -1.0 ? -1.0 : ( dot > 1.0 ? 1.0 : dot ) );
		return acos(dot);
	}
	inline double angleCos(const Vector3& vec) {
		double dot = dotProduct(vec);
		dot = ( dot < -1.0 ? -1.0 : ( dot > 1.0 ? 1.0 : dot ) );
		return dot;
	}

	inline Vector3 normalize() {
		return this->operator /=(lenght());
	}
	Vector3 reflectDirection(Vector3 incoming);

	inline bool isEmpty() {
		Vector3 v = this->abs();
		return v.x<0.0001 && v.y<0.0001 && v.z<0.0001;
	}
};

class Vector4 : public Vector3 {
public:
	double m;
	inline Vector4() : Vector3(), m(0) {}
	inline Vector4(double x, double y, double z, double m) : Vector3(x,y,z), m(m) {}
	inline Vector4(Vector3& vec) : Vector3(vec), m(0) {}
	inline ~Vector4(){}

	inline Vector4& operator+=(const Vector4& rhs) {
		x+=rhs.x;
		y+=rhs.y;
		z+=rhs.z;
		m+=rhs.m;
		return *this;
	}
	inline Vector4& operator+=(const double& rhs) {
		x+=rhs;
		y+=rhs;
		z+=rhs;
		m+=rhs;
		return *this;
	}
	inline Vector4& operator-=(const Vector4& rhs) {
		x-=rhs.x;
		y-=rhs.y;
		z-=rhs.z;
		m-=rhs.m;
		return *this;
	}
	inline Vector4& operator-=(const double& rhs) {
		x-=rhs;
		y-=rhs;
		z-=rhs;
		m-=rhs;
		return *this;
	}
	inline Vector4& operator*=(const Vector4& rhs) {
		x*=rhs.x;
		y*=rhs.y;
		z*=rhs.z;
		m*=rhs.m;
		return *this;
	}
	inline Vector4& operator*=(const double& rhs) {
		x*=rhs;
		y*=rhs;
		z*=rhs;
		m*=rhs;
		return *this;
	}
	inline Vector4& operator/=(const Vector4& rhs) {
		x/=rhs.x;
		y/=rhs.y;
		z/=rhs.z;
		m/=rhs.m;
		return *this;
	}
	inline Vector4& operator/=(const double& rhs) {
		x/=rhs;
		y/=rhs;
		z/=rhs;
		m/=rhs;
		return *this;
	}

	inline double dotProduct(Vector4 vec) const {
		return x*vec.x + y*vec.y + z*vec.z + m*vec.m;
	}
	inline Vector4 crossProduct(Vector4 vec1, Vector4 vec2) const {
		double A, B, C, D, E, F, outX, outY, outZ, outM;       // Intermediate Values

		// Calculate intermediate values.

		A = (vec1.x * vec2.y) - (vec1.y * vec2.x);
		B = (vec1.x * vec2.z) - (vec1.z * vec2.x);
		C = (vec1.x * vec2.m) - (vec1.m * vec2.x);
		D = (vec1.y * vec2.z) - (vec1.z * vec2.y);
		E = (vec1.y * vec2.m) - (vec1.m * vec2.y);
		F = (vec1.z * vec2.m) - (vec1.m * vec2.z);

		// Calculate the result-vector components.

		outX =   (y * F) - (z * E) + (m * D);
		outY = - (x * F) + (z * C) - (m * B);
		outZ =   (x * E) - (y * C) + (z * A);
		outM = - (x * D) + (y * B) - (z * A);

		return Vector4(outX, outY, outZ, outM);
	}
	inline Vector4 abs() const {
		Vector4 out (std::abs(x), std::abs(y), std::abs(z), std::abs(m));
		return out;
	}
	inline double lenght() const {
		return std::sqrt(x*x+y*y+z*z+m*m);
	}
	inline double max_component() const {
		return std::max(std::max(x, y), std::max(z,m));
	}
	inline double min_component() const {
		return std::min(std::min(x, y), std::min(z,m));
	}
	inline Vector4 max(Vector4 vec) const {
		return Vector4(std::max(x, vec.x), std::max(y, vec.y), std::max(z, vec.z), std::max(m, vec.m));
	}
	inline Vector4 min(Vector4 vec) const {
		return Vector4(std::min(x, vec.x), std::min(y, vec.y), std::min(z, vec.z), std::min(m, vec.m));
	}
	inline Vector4 sgn() const {
		return Vector4(std::sgn(x), std::sgn(y), std::sgn(z), std::sgn(m));
	}
	inline Vector4 sgnInv() const {
		return Vector4(-std::sgn(x), -std::sgn(y), -std::sgn(z), -std::sgn(m));
	}

	inline Vector4 normalize() {
		return this->operator /=(lenght());
	}
	Vector4 reflectDirection(Vector4 incoming);
};

class Vector5 : public Vector4 {
public:
	double w;
	inline Vector5() : Vector4(), w(0) {}
	inline Vector5(double x, double y, double z, double m, double w) : Vector4(x,y,z,m), w(w) {}
	inline Vector5(Vector4& vec) : Vector4(vec), w(0) {}
	inline ~Vector5(){}
};



inline Vector3 operator+(Vector3 lhs, const Vector3& rhs) {
	Vector3 out (lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z);
	return out;
}
inline Vector3 operator+(Vector3 lhs, const double& rhs) {
	Vector3 out (lhs.x+rhs, lhs.y+rhs, lhs.z+rhs);
	return out;
}
inline Vector3 operator+(double lhs, const Vector3& rhs) {
	Vector3 out (lhs+rhs.x, lhs+rhs.y, lhs+rhs.z);
	return out;
}

inline Vector3 operator-(Vector3 lhs, const Vector3& rhs) {
	Vector3 out (lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z);
	return out;
}
inline Vector3 operator-(Vector3 lhs, const double& rhs) {
	Vector3 out (lhs.x-rhs, lhs.y-rhs, lhs.z-rhs);
	return out;
}
inline Vector3 operator-(double lhs, const Vector3& rhs) {
	Vector3 out (lhs-rhs.x, lhs-rhs.y, lhs-rhs.z);
	return out;
}

inline Vector3 operator*(Vector3 lhs, const Vector3& rhs) {
	Vector3 out (lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z);
	return out;
}
inline Vector3 operator*(Vector3 lhs, const double& rhs) {
	Vector3 out (lhs.x*rhs, lhs.y*rhs, lhs.z*rhs);
	return out;
}
inline Vector3 operator*(double lhs, const Vector3& rhs) {
	Vector3 out (lhs*rhs.x, lhs*rhs.y, lhs*rhs.z);
	return out;
}

inline Vector3 operator/(Vector3 lhs, const Vector3& rhs) {
	Vector3 out (lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z);
	return out;
}
inline Vector3 operator/(Vector3 lhs, const double& rhs) {
	Vector3 out (lhs.x/rhs, lhs.y/rhs, lhs.z/rhs);
	return out;
}
inline Vector3 operator/(double lhs, const Vector3& rhs) {
	Vector3 out (lhs/rhs.x, lhs/rhs.y, lhs/rhs.z);
	return out;
}
inline Vector3 Vector3::reflectDirection(Vector3 incoming) {
	return incoming - dotProduct(incoming)*2 * (*this);
}




inline Vector4 operator+(Vector4 lhs, const Vector4& rhs) {
	Vector4 out (lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z, lhs.m+rhs.m);
	return out;
}
inline Vector4 operator+(Vector4 lhs, const double& rhs) {
	Vector4 out (lhs.x+rhs, lhs.y+rhs, lhs.z+rhs, lhs.m+rhs);
	return out;
}
inline Vector4 operator+(double lhs, const Vector4& rhs) {
	Vector4 out (lhs+rhs.x, lhs+rhs.y, lhs+rhs.z, lhs+rhs.m);
	return out;
}

inline Vector4 operator-(Vector4 lhs, const Vector4& rhs) {
	Vector4 out (lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z, lhs.m-rhs.m);
	return out;
}
inline Vector4 operator-(Vector4 lhs, const double& rhs) {
	Vector4 out (lhs.x-rhs, lhs.y-rhs, lhs.z-rhs, lhs.m-rhs);
	return out;
}
inline Vector4 operator-(double lhs, const Vector4& rhs) {
	Vector4 out (lhs-rhs.x, lhs-rhs.y, lhs-rhs.z, lhs-rhs.m);
	return out;
}

inline Vector4 operator*(Vector4 lhs, const Vector4& rhs) {
	Vector4 out (lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z, lhs.m*rhs.m);
	return out;
}
inline Vector4 operator*(Vector4 lhs, const double& rhs) {
	Vector4 out (lhs.x*rhs, lhs.y*rhs, lhs.z*rhs, lhs.m*rhs);
	return out;
}
inline Vector4 operator*(double lhs, const Vector4& rhs) {
	Vector4 out (lhs*rhs.x, lhs*rhs.y, lhs*rhs.z, lhs*rhs.m);
	return out;
}

inline Vector4 operator/(Vector4 lhs, const Vector4& rhs) {
	Vector4 out (lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z, lhs.m/rhs.m);
	return out;
}
inline Vector4 operator/(Vector4 lhs, const double& rhs) {
	Vector4 out (lhs.x/rhs, lhs.y/rhs, lhs.z/rhs, lhs.m/rhs);
	return out;
}
inline Vector4 operator/(double lhs, const Vector4& rhs) {
	Vector4 out (lhs/rhs.x, lhs/rhs.y, lhs/rhs.z, lhs/rhs.m);
	return out;
}
inline Vector4 Vector4::reflectDirection(Vector4 incoming) {
	return incoming - dotProduct(incoming)*2 * (*this);
}


#endif /* VECTOR_H_ */
