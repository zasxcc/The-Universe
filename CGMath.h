#pragma once
/* �ۼ��� - 2013182024 KPU SMS, helped by LSK

last write date : 2018. 12. 6

myVector4f �� myMatrix44f �� �ε��Ҽ��� �񱳿���, �ﰢ�Լ� Ȱ���� ������ֽ��ϴ�.
*/
#include <gl/freeglut.h> /* LibraryHeader ���� ������. */
#include <math.h>
#include <float.h>

#define PI_d				3.1415926535
#define PI					3.1415926535f
#define HALF_PI_d			PI_d / 2.0	/* PI / 2, double */
#define HALF_PI				PI / 2.f	/* PI / 2, float */

constexpr GLfloat FACTOR_DEGREE_f{ 180.f / PI };
constexpr GLfloat FACTOR_RADIAN_f{ PI / 180.f };

constexpr float ToRadian(float degree) { return degree * FACTOR_RADIAN_f; }
constexpr float ToDegree(float radian) { return radian * FACTOR_DEGREE_f; }

bool isZero(float number);				/* �ε��Ҽ��� ���񱳿��� */
bool isEqual(float lhs, float rhs);		/* �ε��Ҽ��� �񱳿��� */

class myVector4f {
public:
	union
	{	/* ���� ����ü, ������ ���� �پ缺 ����� ���ؼ�, ����� - �̸����� ����ü ���� ���� ��� */
		GLfloat m[4];					//	float �迭����
		struct { GLfloat x, y, z, w; };	//	�Ϲ� ��������
		struct { GLfloat r, g, b, a; };	//	rgba ��������
	};

	myVector4f() : x{ 0.f }, y{ 0.f }, z{ 0.f }, w{ 1.f } {}
	myVector4f(double x, double y, double z);
	myVector4f(int x, int y, int z);
	myVector4f(float x, float y, float z);
	myVector4f(const myVector4f& other);
	~myVector4f();

	/* ������ �����ε� */
		/* ��Į��� */
	myVector4f operator*(float scalar) const;
	myVector4f operator*(int scalar) const;
	myVector4f operator*(double scalar) const;
	myVector4f operator/(float scalar) const;
	myVector4f operator/(int scalar) const;
	myVector4f operator/(double scalar) const;

	/* ���� ���� ���� */
	myVector4f operator-() const;

	/* ���� ���� */
	myVector4f operator+(const myVector4f& rvalue) const;

	/* ���� ���� */
	myVector4f operator-(const myVector4f& rvalue) const;

	/* ���� ���� */
	myVector4f& operator=(const myVector4f& lvalue);

	/* �ݺ����� ���� */
	myVector4f& operator+=(const myVector4f& lvalue);
};

myVector4f operator*(int scalar, const myVector4f& rhsVector);
myVector4f operator*(float scalar, const myVector4f& rhsVector);
myVector4f operator*(double scalar, const myVector4f& rhsVector);
bool operator==(const myVector4f& lhsVector, const myVector4f& rhsVector);			/* �� ������ */

float dotproduct(const myVector4f& lhsVector, const myVector4f& rhsVector);			/* ���� */
myVector4f crossproduct(const myVector4f& lhsVector, const myVector4f& rhsVector);	/* ���� */
myVector4f normalize(const myVector4f& lvalue);										/* ����ȭ */
float getNorm(const myVector4f& vector);
float getDistance(const myVector4f& vector1, const myVector4f& vector2);
float getRoughMagnitude(const myVector4f & vector1, const myVector4f & vector2);	/* sqrtf ������ ��� �����̿���, ��Ʈ���� ���� �������¸� �����մϴ�. */

const myVector4f zeroVector;					/* ������ */
const myVector4f worldAxisY{ 0.f,1.f,0.f };		/* ���� y�� ���⺤�� */
const myVector4f worldAxisZ{ 0.f,0.f,1.f };		/* ���� z�� ���⺤�� */
const myVector4f worldAxisX{ 1.f,0.f,0.f };		/* ���� x�� ���⺤�� */

class myMatrix44f {
	/* 4x4, ��� ���Ҵ� float ���� Matrix */
public:
	union 
	{
		myVector4f row[4];
		struct {
			GLfloat 
				m11, m12, m13, m14,
				m21, m22, m23, m24,
				m31, m32, m33, m34,
				m41, m42, m43, m44;
		};
	};
	
	myMatrix44f(); /* �⺻ ������, ������� "E" �� �ʱ�ȭ */
	~myMatrix44f();
	myMatrix44f(const myMatrix44f& other);
	myMatrix44f& operator=(const myMatrix44f& other);

	/* ��� �⺻ ��ȯ */
	static myMatrix44f Rotate(myVector4f axis, float degree);	/* ȸ�� */
	/* static ������ myMatrix44f ���ӽ����̽��� �����Լ�ó�� ����ϰ� �Ϸ��� �ǵ� */
	/* ������ �࿡ ���� ȸ����ȯ ����Դϴ�. ���� URL
	http://nobilitycat.tistory.com/entry/%EC%9E%84%EC%9D%98%EC%9D%98-%EC%B6%95-%ED%9A%8C%EC%A0%84-Axis-Angle-Rotation
	*/

	myMatrix44f operator*(const myMatrix44f& other) const;		/* ��İ� */
	myVector4f operator*(const myVector4f& ColumnVector) const;	/* ��� * �����Ͱ� */
};