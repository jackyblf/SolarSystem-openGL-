#ifndef __BMATH_H
#define __BMATH_H
#include "math.h"

#define EPSILON 1e-6f
#define PI 3.14159265358979323846f
#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)

struct vec3 {
	
	inline vec3() : x(0), y(0), z(0) { }
	inline vec3(float x,float y,float z) : x(x), y(y), z(z) { }
	inline vec3(const float *v) : x(v[0]), y(v[1]), z(v[2]) { }
	inline vec3(const vec3 &v) : x(v.x), y(v.y), z(v.z) { }
	
	inline int operator==(const vec3 &v) { return (fabs(x - v.x) < EPSILON && fabs(y - v.y) < EPSILON && fabs(z - v.z) < EPSILON); }
	inline int operator!=(const vec3 &v) { return !(*this == v); }
	
	inline const vec3 operator*(float f) const { return vec3(x * f,y * f,z * f); }
	inline const vec3 operator/(float f) const { return vec3(x / f,y / f,z / f); }
	inline const vec3 operator+(const vec3 &v) const { return vec3(x + v.x,y + v.y,z + v.z); }
	inline const vec3 operator-() const { return vec3(-x,-y,-z); }
	inline const vec3 operator-(const vec3 &v) const { return vec3(x - v.x,y - v.y,z - v.z); }
	
	inline vec3 &operator*=(float f) { return *this = *this * f; }
	inline vec3 &operator/=(float f) { return *this = *this / f; }
	inline vec3 &operator+=(const vec3 &v) { return *this = *this + v; }
	inline vec3 &operator-=(const vec3 &v) { return *this = *this - v; }
	
	inline float operator*(const vec3 &v) const { return x * v.x + y * v.y + z * v.z; }
	
	
	inline operator float*() { return (float*)&x; }
	inline operator const float*() const { return (float*)&x; }
	
	inline float &operator[](int i) { return ((float*)&x)[i]; }
	inline const float operator[](int i) const { return ((float*)&x)[i]; }
	
	inline float length() const { return sqrt(x * x + y * y + z * z); }
	inline float normalize() {
		float inv,length = sqrt(x * x + y * y + z * z);
		if(length < EPSILON) return 0.0;
		inv = 1.0f / length;
		x *= inv;
		y *= inv;
		z *= inv;
		return length;
	}
	inline void cross(const vec3 &v1,const vec3 &v2) {
		x = v1.y * v2.z - v1.z * v2.y;
		y = v1.z * v2.x - v1.x * v2.z;
		z = v1.x * v2.y - v1.y * v2.x;
	}
	
	union {
		struct {
			float x,y,z;
		};
		float v[3];
	};
};

inline vec3 cross(const vec3 &v1,const vec3 &v2) {
	vec3 ret;
	ret.x = v1.y * v2.z - v1.z * v2.y;
	ret.y = v1.z * v2.x - v1.x * v2.z;
	ret.z = v1.x * v2.y - v1.y * v2.x;
	return ret;
}
#endif