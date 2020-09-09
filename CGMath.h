#pragma once
/* 작성자 - 2013182024 KPU SMS, helped by LSK

last write date : 2018. 12. 6

myVector4f 와 myMatrix44f 및 부동소수점 비교연산, 삼각함수 활용이 담겨져있습니다.
*/
#include <gl/freeglut.h> /* LibraryHeader 없이 쓰려고. */
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

bool isZero(float number);				/* 부동소수점 영비교연산 */
bool isEqual(float lhs, float rhs);		/* 부동소수점 비교연산 */

class myVector4f {
public:
	union
	{	/* 공용 구조체, 데이터 접근 다양성 향상을 위해서, 경고문구 - 이름없는 구조체 선언에 대한 경고 */
		GLfloat m[4];					//	float 배열형태
		struct { GLfloat x, y, z, w; };	//	일반 벡터형태
		struct { GLfloat r, g, b, a; };	//	rgba 벡터형태
	};

	myVector4f() : x{ 0.f }, y{ 0.f }, z{ 0.f }, w{ 1.f } {}
	myVector4f(double x, double y, double z);
	myVector4f(int x, int y, int z);
	myVector4f(float x, float y, float z);
	myVector4f(const myVector4f& other);
	~myVector4f();

	/* 연산자 오버로딩 */
		/* 스칼라곱 */
	myVector4f operator*(float scalar) const;
	myVector4f operator*(int scalar) const;
	myVector4f operator*(double scalar) const;
	myVector4f operator/(float scalar) const;
	myVector4f operator/(int scalar) const;
	myVector4f operator/(double scalar) const;

	/* 벡터 방향 역전 */
	myVector4f operator-() const;

	/* 벡터 덧셈 */
	myVector4f operator+(const myVector4f& rvalue) const;

	/* 벡터 뺄셈 */
	myVector4f operator-(const myVector4f& rvalue) const;

	/* 대입 연산 */
	myVector4f& operator=(const myVector4f& lvalue);

	/* 반복대입 연산 */
	myVector4f& operator+=(const myVector4f& lvalue);
};

myVector4f operator*(int scalar, const myVector4f& rhsVector);
myVector4f operator*(float scalar, const myVector4f& rhsVector);
myVector4f operator*(double scalar, const myVector4f& rhsVector);
bool operator==(const myVector4f& lhsVector, const myVector4f& rhsVector);			/* 비교 연산자 */

float dotproduct(const myVector4f& lhsVector, const myVector4f& rhsVector);			/* 내적 */
myVector4f crossproduct(const myVector4f& lhsVector, const myVector4f& rhsVector);	/* 외적 */
myVector4f normalize(const myVector4f& lvalue);										/* 정규화 */
float getNorm(const myVector4f& vector);
float getDistance(const myVector4f& vector1, const myVector4f& vector2);
float getRoughMagnitude(const myVector4f & vector1, const myVector4f & vector2);	/* sqrtf 연산이 비싼 연산이여서, 루트적용 안한 제곱상태를 리턴합니다. */

const myVector4f zeroVector;					/* 영벡터 */
const myVector4f worldAxisY{ 0.f,1.f,0.f };		/* 월드 y축 방향벡터 */
const myVector4f worldAxisZ{ 0.f,0.f,1.f };		/* 월드 z축 방향벡터 */
const myVector4f worldAxisX{ 1.f,0.f,0.f };		/* 월드 x축 방향벡터 */

class myMatrix44f {
	/* 4x4, 모든 원소는 float 값인 Matrix */
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
	
	myMatrix44f(); /* 기본 생성자, 단위행렬 "E" 로 초기화 */
	~myMatrix44f();
	myMatrix44f(const myMatrix44f& other);
	myMatrix44f& operator=(const myMatrix44f& other);

	/* 행렬 기본 변환 */
	static myMatrix44f Rotate(myVector4f axis, float degree);	/* 회전 */
	/* static 선언은 myMatrix44f 네임스페이스의 전역함수처럼 사용하게 하려는 의도 */
	/* 임의의 축에 대한 회전변환 행렬입니다. 참고 URL
	http://nobilitycat.tistory.com/entry/%EC%9E%84%EC%9D%98%EC%9D%98-%EC%B6%95-%ED%9A%8C%EC%A0%84-Axis-Angle-Rotation
	*/

	myMatrix44f operator*(const myMatrix44f& other) const;		/* 행렬곱 */
	myVector4f operator*(const myVector4f& ColumnVector) const;	/* 행렬 * 열벡터곱 */
};