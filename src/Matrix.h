/*
 * Matrix.h
 *
 *  Created on: 15 Dec 2019
 *      Author: christiaan
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "Vector.h"
#include <cstring>

class Matrix4;
Matrix4 operator*(Matrix4 lhs, const Matrix4& rhs);
Vector3 operator*(Vector3 lhs, const Matrix4& rhs);
static Vector3 singleMult(Vector3 lhs, const Matrix4& rhs);

class Matrix4 {
public:
	double data[4][4];
	//inline Matrix4() : data({{1,0,0,0}}) {}
	inline Matrix4(double data[4][4]) {
		memcpy(this->data, data, sizeof(double)*4*4);
	}
	inline Matrix4(double data[4*4]) {
		memcpy(this->data, data, sizeof(double)*4*4);
	}
	inline Matrix4() {
		for (int i1 = 0;i1<4;i1++)
			for (int i2 = 0;i2<4;i2++)
				data[i1][i2] = i1 == i2 ? 1 : 0;
	}
	inline Matrix4(double scale) {
		for (int i1 = 0;i1<4;i1++)
			for (int i2 = 0;i2<4;i2++)
				data[i1][i2] = 0;
		data[0][0] = scale;
		data[1][1] = scale;
		data[2][2] = scale;
		data[3][3] = 1;
	}
	inline Matrix4(Vector3 scale) {
		for (int i1 = 0;i1<4;i1++)
			for (int i2 = 0;i2<4;i2++)
				data[i1][i2] = 0;
		data[0][0] = scale.x;
		data[1][1] = scale.y;
		data[2][2] = scale.z;
		data[3][3] = 1;
	}
	inline ~Matrix4() {}

	Matrix4& translate(Vector3 ofset) {
		data[0][3] += ofset.x;
		data[1][3] += ofset.y;
		data[2][3] += ofset.z;
		return *this;
	}
	Matrix4& translateSafe(Vector3 ofset) {

		Matrix4 m = Matrix4();
		m.translate(ofset);
		(*this) = (*this) * m;

		//translate(singleMult(ofset, (*this)));
		//translate(ofset * (*this));
		return *this;
	}

	void rotateXY(double angle) {
		double COSV = cos(angle);
		double SINV = cos(angle);
		data[0][0] *= COSV;
		data[0][1] *= -SINV;
		data[1][0] *= SINV;
		data[1][1] *= COSV;
	}

	inline Vector3 asLoc() {
		return Vector3(data[0][3], data[1][3], data[2][3]);
	}
	inline Vector3 asScale() {
		return Vector3(data[0][0], data[1][1], data[2][2]);
	}

	inline bool noRot() {
		for (int i1 = 0;i1<3;i1++)
			for (int i2 = 0;i2<3;i2++)
				if (i1 != i2) {
					double v = data[i1][i2];
					if (std::abs(v) > 0.0001)
						return false;
				}
		return true;
	}
};

inline Vector4 operator*(Vector4 lhs, const Matrix4& rhs) {
	Vector4 out (lhs.x*rhs.data[0][0]+lhs.y*rhs.data[0][1]+lhs.z*rhs.data[0][2]+lhs.m*rhs.data[0][3],
		lhs.x*rhs.data[1][0]+lhs.y*rhs.data[1][1]+lhs.z*rhs.data[1][2]+lhs.m*rhs.data[1][3],
		lhs.x*rhs.data[2][0]+lhs.y*rhs.data[2][1]+lhs.z*rhs.data[2][2]+lhs.m*rhs.data[2][3],
		lhs.x*rhs.data[3][0]+lhs.y*rhs.data[3][1]+lhs.z*rhs.data[3][2]+lhs.m*rhs.data[3][3]);
	return out;
}
inline Vector4 operator*(const Matrix4& lhs, Vector4 rhs) {
	return rhs * lhs;
}

inline Vector3 operator*(Vector3 lhs, const Matrix4& rhs) {
	Vector3 out (lhs.x*rhs.data[0][0]+lhs.y*rhs.data[0][1]+lhs.z*rhs.data[0][2]+rhs.data[0][3],
		lhs.x*rhs.data[1][0]+lhs.y*rhs.data[1][1]+lhs.z*rhs.data[1][2]+rhs.data[1][3],
		lhs.x*rhs.data[2][0]+lhs.y*rhs.data[2][1]+lhs.z*rhs.data[2][2]+rhs.data[2][3]);
	return out;
}
inline Vector3 operator*(const Matrix4& lhs, Vector3 rhs) {
	return rhs * lhs;
}
/*
inline Vector3 operator/(Vector3 lhs, const Matrix4& rhs) {
	Vector3 vec2 = Vector3(lhs.x-rhs.data[0][3], lhs.y-rhs.data[1][3], lhs.z-rhs.data[2][3]);

}
/*
 * ax+by+cz=v
 * dx+ey+fz=w
 * gx+hy+iz=u
 *
 * x=(v-by-cz)/a=(w-ey-fz)/d=(u-hy-iz)/g
 * y=(w-ax-cz)/b=...
 * ...
 *
 * x=(v-(w-ax-cz)-cz)/a=(v-w+ax+cz-cz)/a=v/a-w/a+ax/a+cz/a-cz/a
 * x-x = v/a-w/a+cz/a-cz/a
 */


inline Matrix4 operator*(Matrix4 lhs, const Matrix4& rhs) {
	double data[4][4];
	for (int i1 = 0;i1<4;i1++)
		for (int i2 = 0;i2<4;i2++) {
			double total = 0;
			for (int i3 = 0;i3<4;i3++)
				total += lhs.data[i1][i3] * rhs.data[i3][i2];
			data[i1][i2] = total;
		}
	return Matrix4(data);
}

inline static Vector3 singleMult(Vector3 lhs, const Matrix4& rhs) {
	Vector3 out (lhs.x*rhs.data[0][0]+lhs.y*rhs.data[0][1]+lhs.z*rhs.data[0][2],
		lhs.x*rhs.data[1][0]+lhs.y*rhs.data[1][1]+lhs.z*rhs.data[1][2],
		lhs.x*rhs.data[2][0]+lhs.y*rhs.data[2][1]+lhs.z*rhs.data[2][2]);
	return out;
}

template<unsigned v1, unsigned v2>
static Matrix4 Matrix4ROT(double angle) {
	Matrix4 out;
	double COSV = cos(angle);
	double SINV = sin(angle);
	out.data[v1][v1] = COSV;
	out.data[v1][v2] = -SINV;
	out.data[v2][v1] = SINV;
	out.data[v2][v2] = COSV;
	return out;
}

static inline Matrix4 Matrix4ROTxy(double angle) {return Matrix4ROT<0,1>(angle);}
static inline Matrix4 Matrix4ROTyz(double angle) {return Matrix4ROT<1,2>(angle);}
static inline Matrix4 Matrix4ROTzx(double angle) {return Matrix4ROT<2,0>(angle);}

static Matrix4 inverse(Matrix4 m) {
	double A2323 = m.data[2][2] * m.data[3][3] - m.data[2][3] * m.data[3][2];
	double A1323 = m.data[2][1] * m.data[3][3] - m.data[2][3] * m.data[3][1];
	double A1223 = m.data[2][1] * m.data[3][2] - m.data[2][2] * m.data[3][1];
	double A0323 = m.data[2][0] * m.data[3][3] - m.data[2][3] * m.data[3][0];
	double A0223 = m.data[2][0] * m.data[3][2] - m.data[2][2] * m.data[3][0];
	double A0123 = m.data[2][0] * m.data[3][1] - m.data[2][1] * m.data[3][0];
	double A2313 = m.data[1][2] * m.data[3][3] - m.data[1][3] * m.data[3][2];
	double A1313 = m.data[1][1] * m.data[3][3] - m.data[1][3] * m.data[3][1];
	double A1213 = m.data[1][1] * m.data[3][2] - m.data[1][2] * m.data[3][1];
	double A2312 = m.data[1][2] * m.data[2][3] - m.data[1][3] * m.data[2][2];
	double A1312 = m.data[1][1] * m.data[2][3] - m.data[1][3] * m.data[2][1];
	double A1212 = m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1];
	double A0313 = m.data[1][0] * m.data[3][3] - m.data[1][3] * m.data[3][0];
	double A0213 = m.data[1][0] * m.data[3][2] - m.data[1][2] * m.data[3][0];
	double A0312 = m.data[1][0] * m.data[2][3] - m.data[1][3] * m.data[2][0];
	double A0212 = m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0];
	double A0113 = m.data[1][0] * m.data[3][1] - m.data[1][1] * m.data[3][0];
	double A0112 = m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0];

	double det = m.data[0][0] * ( m.data[1][1] * A2323 - m.data[1][2] * A1323 + m.data[1][3] * A1223 )
	    - m.data[0][1] * ( m.data[1][0] * A2323 - m.data[1][2] * A0323 + m.data[1][3] * A0223 )
	    + m.data[0][2] * ( m.data[1][0] * A1323 - m.data[1][1] * A0323 + m.data[1][2] * A0123 )
	    - m.data[0][3] * ( m.data[1][0] * A1223 - m.data[1][1] * A0223 + m.data[1][2] * A0123 ) ;
	det = 1 / det;

	double arr[4*4] = {
			   det *   ( m.data[1][1] * A2323 - m.data[1][2] * A1323 + m.data[1][3] * A1223 ),
			   det * - ( m.data[0][1] * A2323 - m.data[0][2] * A1323 + m.data[0][3] * A1223 ),
			   det *   ( m.data[0][1] * A2313 - m.data[0][2] * A1313 + m.data[0][3] * A1213 ),
			   det * - ( m.data[0][1] * A2312 - m.data[0][2] * A1312 + m.data[0][3] * A1212 ),
			   det * - ( m.data[1][0] * A2323 - m.data[1][2] * A0323 + m.data[1][3] * A0223 ),
			   det *   ( m.data[0][0] * A2323 - m.data[0][2] * A0323 + m.data[0][3] * A0223 ),
			   det * - ( m.data[0][0] * A2313 - m.data[0][2] * A0313 + m.data[0][3] * A0213 ),
			   det *   ( m.data[0][0] * A2312 - m.data[0][2] * A0312 + m.data[0][3] * A0212 ),
			   det *   ( m.data[1][0] * A1323 - m.data[1][1] * A0323 + m.data[1][3] * A0123 ),
			   det * - ( m.data[0][0] * A1323 - m.data[0][1] * A0323 + m.data[0][3] * A0123 ),
			   det *   ( m.data[0][0] * A1313 - m.data[0][1] * A0313 + m.data[0][3] * A0113 ),
			   det * - ( m.data[0][0] * A1312 - m.data[0][1] * A0312 + m.data[0][3] * A0112 ),
			   det * - ( m.data[1][0] * A1223 - m.data[1][1] * A0223 + m.data[1][2] * A0123 ),
			   det *   ( m.data[0][0] * A1223 - m.data[0][1] * A0223 + m.data[0][2] * A0123 ),
			   det * - ( m.data[0][0] * A1213 - m.data[0][1] * A0213 + m.data[0][2] * A0113 ),
			   det *   ( m.data[0][0] * A1212 - m.data[0][1] * A0212 + m.data[0][2] * A0112 ),
			};
	return Matrix4(arr);
}


class Matrix5 {
public:
	double data[5][5];
	//inline Matrix4() : data({{1,0,0,0}}) {}
	inline Matrix5(double data[5][5]) {
		//this->data = data;
		memcpy(this->data, data, sizeof(double)*5*5);
	}
	inline Matrix5() {
		for (int i1 = 0;i1<5;i1++)
			for (int i2 = 0;i2<5;i2++)
				data[i1][i2] = i1 == i2 ? 1 : 0;
	}
	inline Matrix5(Vector4 scale) {
		for (int i1 = 0;i1<5;i1++)
			for (int i2 = 0;i2<5;i2++)
				data[i1][i2] = 0;
		data[0][0] = scale.x;
		data[1][1] = scale.y;
		data[2][2] = scale.z;
		data[3][3] = scale.m;
		data[4][4] = 1;
	}
	inline ~Matrix5() {}

	inline void translate(Vector4 ofset) {
		data[0][4] += ofset.x;
		data[1][4] += ofset.y;
		data[2][4] += ofset.z;
		data[3][4] += ofset.m;
	}

	inline Vector4 asLoc() {
		return Vector4(data[0][4], data[1][4], data[2][4], data[3][4]);
	}
};

inline Vector5 operator*(Vector5 lhs, const Matrix5& rhs) {
	Vector5 out (lhs.x*rhs.data[0][0]+lhs.y*rhs.data[0][1]+lhs.z*rhs.data[0][2]+lhs.m*rhs.data[0][3]+lhs.w*rhs.data[0][4],
		lhs.x*rhs.data[1][0]+lhs.y*rhs.data[1][1]+lhs.z*rhs.data[1][2]+lhs.m*rhs.data[1][3]+lhs.w*rhs.data[1][4],
		lhs.x*rhs.data[2][0]+lhs.y*rhs.data[2][1]+lhs.z*rhs.data[2][2]+lhs.m*rhs.data[2][3]+lhs.w*rhs.data[2][4],
		lhs.x*rhs.data[3][0]+lhs.y*rhs.data[3][1]+lhs.z*rhs.data[3][2]+lhs.m*rhs.data[3][3]+lhs.w*rhs.data[3][4],
		lhs.x*rhs.data[4][0]+lhs.y*rhs.data[4][1]+lhs.z*rhs.data[4][2]+lhs.m*rhs.data[4][3]+lhs.w*rhs.data[4][4]);
	return out;
}
inline Vector5 operator*(const Matrix5& lhs, Vector5 rhs) {
	return rhs * lhs;
}

inline Vector4 operator*(Vector4 lhs, const Matrix5& rhs) {
	Vector4 out (lhs.x*rhs.data[0][0]+lhs.y*rhs.data[0][1]+lhs.z*rhs.data[0][2]+lhs.m*rhs.data[0][3]+rhs.data[0][4],
		lhs.x*rhs.data[1][0]+lhs.y*rhs.data[1][1]+lhs.z*rhs.data[1][2]+lhs.m*rhs.data[1][3]+rhs.data[1][4],
		lhs.x*rhs.data[2][0]+lhs.y*rhs.data[2][1]+lhs.z*rhs.data[2][2]+lhs.m*rhs.data[2][3]+rhs.data[2][4],
		lhs.x*rhs.data[3][0]+lhs.y*rhs.data[3][1]+lhs.z*rhs.data[3][2]+lhs.m*rhs.data[3][3]+rhs.data[3][4]);
	return out;
}
inline Vector4 operator*(const Matrix5& lhs, Vector4 rhs) {
	return rhs * lhs;
}

inline Matrix5 operator*(Matrix5 lhs, const Matrix5& rhs) {
	double data[5][5];
	for (int i1 = 0;i1<5;i1++)
		for (int i2 = 0;i2<5;i2++) {
			double total = 0;
			for (int i3 = 0;i3<5;i3++)
				total += lhs.data[i1][i3] * rhs.data[i3][i2];
			data[i1][i2] = total;
		}
	return Matrix5(data);
}

inline static Vector4 singleMult(Vector4 lhs, const Matrix5& rhs) {
	Vector4 out (lhs.x*rhs.data[0][0]+lhs.y*rhs.data[0][1]+lhs.z*rhs.data[0][2]+lhs.m*rhs.data[0][3],
			lhs.x*rhs.data[1][0]+lhs.y*rhs.data[1][1]+lhs.z*rhs.data[1][2]+lhs.m*rhs.data[1][3],
			lhs.x*rhs.data[2][0]+lhs.y*rhs.data[2][1]+lhs.z*rhs.data[2][2]+lhs.m*rhs.data[2][3],
			lhs.x*rhs.data[3][0]+lhs.y*rhs.data[3][1]+lhs.z*rhs.data[3][2]+lhs.m*rhs.data[3][3]);
	return out;
}

template<unsigned v1, unsigned v2>
static Matrix5 Matrix5ROT(double angle) {
	Matrix5 out;
	double COSV = cos(angle);
	double SINV = sin(angle);
	out.data[v1][v1] = COSV;
	out.data[v1][v2] = -SINV;
	out.data[v2][v1] = SINV;
	out.data[v2][v2] = COSV;
	return out;
}

static inline Matrix5 Matrix5ROTxy(double angle) {return Matrix5ROT<0,1>(angle);}
static inline Matrix5 Matrix5ROTyz(double angle) {return Matrix5ROT<1,2>(angle);}
static inline Matrix5 Matrix5ROTzx(double angle) {return Matrix5ROT<2,0>(angle);}
static inline Matrix5 Matrix5ROTzm(double angle) {return Matrix5ROT<2,3>(angle);}
static inline Matrix5 Matrix5ROTmx(double angle) {return Matrix5ROT<3,0>(angle);}






#endif /* MATRIX_H_ */
