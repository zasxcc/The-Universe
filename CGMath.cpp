#include "CGMath.h"

myVector4f::myVector4f(double x, double y, double z)
{
	myVector4f::x = static_cast<GLfloat>(x);
	myVector4f::y = static_cast<GLfloat>(y);
	myVector4f::z = static_cast<GLfloat>(z);
	myVector4f::w = 1.f;
}

myVector4f::myVector4f(int x, int y, int z)
{
	myVector4f::x = static_cast<GLfloat>(x);
	myVector4f::y = static_cast<GLfloat>(y);
	myVector4f::z = static_cast<GLfloat>(z);
	myVector4f::w = 1.f;
}

myVector4f::myVector4f(float x, float y, float z)
{
	myVector4f::x = x;
	myVector4f::y = y;
	myVector4f::z = z;
	myVector4f::w = 1.f;
}

myVector4f::myVector4f(const myVector4f & other)
{
	/* 깊은복사 */
	memcpy_s(this, sizeof(myVector4f), &other, sizeof(myVector4f));
}

myVector4f::~myVector4f()
{
}

myVector4f myVector4f::operator*(float scalar) const
{
	return myVector4f(scalar * x, scalar * y, scalar * z);
}

myVector4f myVector4f::operator*(int scalar) const
{
	return myVector4f(static_cast<float>(scalar) * x, static_cast<float>(scalar) * y, static_cast<float>(scalar) * z);
}

myVector4f myVector4f::operator*(double scalar) const
{
	return myVector4f(static_cast<float>(scalar) * x, static_cast<float>(scalar) * y, static_cast<float>(scalar) * z);
}

myVector4f myVector4f::operator/(float scalar) const
{
	return myVector4f(x / scalar, y / scalar, z / scalar);
}

myVector4f myVector4f::operator/(int scalar) const
{
	return myVector4f(x / static_cast<float>(scalar), y / static_cast<float>(scalar), z / static_cast<float>(scalar));
}

myVector4f myVector4f::operator/(double scalar) const
{
	return myVector4f(x / static_cast<float>(scalar), y / static_cast<float>(scalar), z / static_cast<float>(scalar));
}

myVector4f myVector4f::operator-() const
{
	return myVector4f(-x, -y, -z);
}

myVector4f myVector4f::operator+(const myVector4f & rvalue) const
{
	return myVector4f(x + rvalue.x, y + rvalue.y, z + rvalue.z);
}

myVector4f myVector4f::operator-(const myVector4f & rvalue) const
{
	return myVector4f(x - rvalue.x, y - rvalue.y, z - rvalue.z);
}

myVector4f& myVector4f::operator=(const myVector4f & lvalue)
{
	/* 깊은 복사 */
	memcpy_s(this, sizeof(myVector4f), &lvalue, sizeof(myVector4f));
	return *this;
}

myVector4f & myVector4f::operator+=(const myVector4f & lvalue)
{
	*this = lvalue + *this;
	return *this;
}

bool isZero(float number)
{
	return (-FLT_EPSILON < number && number < FLT_EPSILON);
}

bool isEqual(float lhs, float rhs)
{
	return (isZero(lhs - rhs));
}

myVector4f operator*(int scalar, const myVector4f & rhsVector)
{
	return myVector4f(static_cast<float>(scalar) * rhsVector.x, static_cast<float>(scalar) * rhsVector.y, static_cast<float>(scalar) * rhsVector.z);
}

myVector4f operator*(float scalar, const myVector4f & rhsVector)
{
	return myVector4f(scalar * rhsVector.x, scalar * rhsVector.y, scalar * rhsVector.z);
}

myVector4f operator*(double scalar, const myVector4f & rhsVector)
{
	return myVector4f(static_cast<float>(scalar) * rhsVector.x, static_cast<float>(scalar) * rhsVector.y, static_cast<float>(scalar) * rhsVector.z);
}

bool operator==(const myVector4f & lhsVector, const myVector4f & rhsVector)
{
	if (isEqual(lhsVector.x, rhsVector.x)
		&&
		isEqual(lhsVector.y, rhsVector.y)
		&&
		isEqual(lhsVector.z, rhsVector.z)
		&&
		isEqual(lhsVector.w, rhsVector.w)) {
		return true;
	}
	return false;
}

float dotproduct(const myVector4f & lhsVector, const myVector4f & rhsVector)
{
	return lhsVector.x * rhsVector.x + lhsVector.y * rhsVector.y + lhsVector.z * rhsVector.z;
}

myVector4f crossproduct(const myVector4f & lhsVector, const myVector4f & rhsVector)
{
	return myVector4f(
		lhsVector.y * rhsVector.z - lhsVector.z * rhsVector.y,
		lhsVector.z * rhsVector.x - lhsVector.x * rhsVector.z,
		lhsVector.x * rhsVector.y - lhsVector.y * rhsVector.x);
}

myVector4f normalize(const myVector4f & lvalue)
{
	GLfloat norm = getNorm(lvalue);
	return myVector4f(lvalue.x / norm, lvalue.y / norm, lvalue.z / norm);
}

float getNorm(const myVector4f & vector)
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

float getDistance(const myVector4f & vector1, const myVector4f & vector2)
{
	return sqrtf(
		(vector2.x - vector1.x) * (vector2.x - vector1.x) +
		(vector2.y - vector1.y) * (vector2.y - vector1.y) +
		(vector2.z - vector1.z) * (vector2.z - vector1.z)
	);
}

float getRoughMagnitude(const myVector4f & vector1, const myVector4f & vector2)
{
	return
		(vector2.x - vector1.x) * (vector2.x - vector1.x) +
		(vector2.y - vector1.y) * (vector2.y - vector1.y) +
		(vector2.z - vector1.z) * (vector2.z - vector1.z);
}

myMatrix44f::myMatrix44f()
	: 
	m11{ 1.f }, m12{ 0.f }, m13{ 0.f }, m14{ 0.f },
	m21{ 0.f }, m22{ 1.f }, m23{ 0.f }, m24{ 0.f },
	m31{ 0.f }, m32{ 0.f }, m33{ 1.f }, m34{ 0.f },
	m41{ 0.f }, m42{ 0.f }, m43{ 0.f }, m44{ 1.f }
{
}

myMatrix44f::~myMatrix44f()
{
}

myMatrix44f::myMatrix44f(const myMatrix44f & other)
{
	/* 깊은 복사 */
	memcpy_s(this, sizeof(myMatrix44f), &other, sizeof(myMatrix44f));
}

myMatrix44f & myMatrix44f::operator=(const myMatrix44f & other)
{
	/* 깊은 복사 */
	memcpy_s(this, sizeof(myMatrix44f), &other, sizeof(myMatrix44f));
	return *this;
}

myMatrix44f myMatrix44f::Rotate(myVector4f axis, float degree)
{
	float radian = ToRadian(degree);
	float c = cosf(radian);
	float s = sinf(radian);
	float t = 1 - c;

	/* 공식유도와 정보출처는 헤더의 URL 참고 */

	myMatrix44f tmpMatrix;
	tmpMatrix.m11 = t * axis.x * axis.x + c;
	tmpMatrix.m12 = t * axis.x * axis.y - s * axis.z;
	tmpMatrix.m13 = t * axis.x * axis.z + s * axis.y;

	tmpMatrix.m21 = t * axis.y * axis.x + s * axis.z;
	tmpMatrix.m22 = t * axis.y * axis.y + c;
	tmpMatrix.m23 = t * axis.y * axis.z - s * axis.x;
	
	tmpMatrix.m31 = t * axis.z * axis.x - s * axis.y;
	tmpMatrix.m32 = t * axis.z * axis.y + s * axis.x;
	tmpMatrix.m33 = t * axis.z * axis.z + c;

	return tmpMatrix;
}

myMatrix44f myMatrix44f::operator*(const myMatrix44f & other) const
{
	myMatrix44f tmpMatrix;
	tmpMatrix.m11 = this->m11 * other.m11 + this->m12 * other.m21 + this->m13 * other.m31 + this->m14 * other.m41;
	tmpMatrix.m12 = this->m11 * other.m12 + this->m12 * other.m22 + this->m13 * other.m32 + this->m14 * other.m42;
	tmpMatrix.m13 = this->m11 * other.m13 + this->m12 * other.m23 + this->m13 * other.m33 + this->m14 * other.m43;
	tmpMatrix.m14 = this->m11 * other.m14 + this->m12 * other.m24 + this->m13 * other.m34 + this->m14 * other.m44;

	tmpMatrix.m21 = this->m21 * other.m11 + this->m22 * other.m21 + this->m23 * other.m31 + this->m24 * other.m41;
	tmpMatrix.m22 = this->m21 * other.m12 + this->m22 * other.m22 + this->m23 * other.m32 + this->m24 * other.m42;
	tmpMatrix.m23 = this->m21 * other.m13 + this->m22 * other.m23 + this->m23 * other.m33 + this->m24 * other.m43;
	tmpMatrix.m24 = this->m21 * other.m14 + this->m22 * other.m24 + this->m23 * other.m34 + this->m24 * other.m44;

	tmpMatrix.m31 = this->m31 * other.m11 + this->m32 * other.m21 + this->m33 * other.m31 + this->m34 * other.m41;
	tmpMatrix.m32 = this->m31 * other.m12 + this->m32 * other.m22 + this->m33 * other.m32 + this->m34 * other.m42;
	tmpMatrix.m33 = this->m31 * other.m13 + this->m32 * other.m23 + this->m33 * other.m33 + this->m34 * other.m43;
	tmpMatrix.m34 = this->m31 * other.m14 + this->m32 * other.m24 + this->m33 * other.m34 + this->m34 * other.m44;

	tmpMatrix.m41 = this->m41 * other.m11 + this->m42 * other.m21 + this->m43 * other.m31 + this->m44 * other.m41;
	tmpMatrix.m42 = this->m41 * other.m12 + this->m42 * other.m22 + this->m43 * other.m32 + this->m44 * other.m42;
	tmpMatrix.m43 = this->m41 * other.m13 + this->m42 * other.m23 + this->m43 * other.m33 + this->m44 * other.m43;
	tmpMatrix.m44 = this->m41 * other.m14 + this->m42 * other.m24 + this->m43 * other.m34 + this->m44 * other.m44;

	return tmpMatrix;
}

myVector4f myMatrix44f::operator*(const myVector4f & ColumnVector) const
{
	myVector4f tmpVec;
	tmpVec.x = this->m11 * ColumnVector.x + this->m12 * ColumnVector.y + this->m13 * ColumnVector.z + this->m14 * ColumnVector.w;
	tmpVec.y = this->m21 * ColumnVector.x + this->m22 * ColumnVector.y + this->m23 * ColumnVector.z + this->m24 * ColumnVector.w;
	tmpVec.z = this->m31 * ColumnVector.x + this->m32 * ColumnVector.y + this->m33 * ColumnVector.z + this->m34 * ColumnVector.w;
	tmpVec.w = this->m41 * ColumnVector.x + this->m42 * ColumnVector.y + this->m43 * ColumnVector.z + this->m44 * ColumnVector.w;
	return tmpVec;
}