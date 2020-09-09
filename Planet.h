#pragma once
#include "CGMath.h"

/* 케플러 운동법칙
	1.	행성은 태양을 한 초점으로 하는 타원궤도를 그리면서 공전한다. (타원궤도 법칙)
	2.	행성과 태양을 연결하는 가상적인 선분이 같은 시간 동안 쓸고 지나가는 면적은 항상 같다.
	3.	행성의 공전주기의 제곱은 궤도의 긴반지름의 세제곱에 비례한다.
*/
/* 일반상대성 이론처럼 시공간 곡률이 밀도와 질량에 따라 결정되는 것이 아니라
	고전역학에서의 가상의 중력장 모델 사용하자. 
	*/

enum NAME {
	NONE,
	SPACE,
	SUN,
	MERCURY,
	VENUS,
	EARTH,
	EARTH_CLOUD,
	MOON,
	MARS,
	JUPITER,
	SATURN,
	URANUS,
	NEPTUNE
};

class Planet {
private:
	/* collide Sphere slice & stack */
	const int SLICES{ 20 };
	const int STACKS{ 20 };
	/* 행성은 자전하여도 '구'라고 가정한다. */
private:
	myVector4f centre;	/* pos, 중점 */
	myVector4f vel;
	myVector4f accel;
	myVector4f look;	/* 무조건 태양을 바라본다. 태양의 경우 look은 World Z축임. */
	myVector4f up;		
	myVector4f right;	/* 운동 방향 */
	float speed_move{ 300.f };
	float speed_rotate{ 0.f };
	float mass{ 0.f };
	float radius{ 0.f };
	float gravityFieldRadius{ 0.f };	/* 구 중력장의 반지름 */
	float collideSphereRadius{ 0.f };	/* 충돌박스 반지름 */
	float angle_rotationAxis{ 0.f };	//	자전축 기울기
	float angle_rotate{ 0.f };			//	자전각
	int kinds{ NAME::NONE };

public:
	bool CollideSphereFlag{ false };

	Planet(int name);
	~Planet();

	void setCentre(const myVector4f pos);
	myVector4f getCentre() const;

	void setVel(const myVector4f vel);
	myVector4f getVel() const;

	void setAccel(const myVector4f accel);
	myVector4f getAccel() const;

	int getKinds() const;

	float getRadius() const;
	float getGavityRadius() const;

	void drawCollideSphere();

	void render(void);
	void update(float eTime);
	void rotateFaceEarth();			/* 달 전용 함수 */
	void MakeSphere(float posX, float posY, float posZ, float radius, int n); // n은 원의 둥글기 (클수록 둥글둥글)

	void drawRing();
};
