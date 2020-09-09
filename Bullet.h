#pragma once
#include "LibraryHeader.h"
#include "CGMath.h"
#include "SpaceProbe.h"

constexpr int MAX_BULLET{ 50 };
constexpr int MAX_PARTICLE{ 20 };

class Particle {
private:
	/* render 편의성을 위한 Sphere 결정요건 */
	const int SLICES{ 6 };
	const int STACKS{ 6 };
	const float DEFAULT_RADIUS{ 3.f };
public:
	/* 생존 시간 */
	const float LIFETIME_FULL{ 5.f };
private:
	myVector4f pos;									/* 중심좌표 */
	myVector4f vel;									/* 속도 */
	myVector4f color{ 1.f,1.f,1.f };
	bool isAlive{ false };							/* 살아있는가 */
	float lifetime{ LIFETIME_FULL };				/* 생존 시간 */
	float radius{ DEFAULT_RADIUS };					/* 반지름 */
	float collideSphereRadius{ DEFAULT_RADIUS };	/* 충돌박스 반지름 */

public:
	bool CollideSphereFlag{ false };

	Particle();
	~Particle();

	myVector4f getPos() const;
	void setPos(const myVector4f& pos);

	myVector4f getVel() const;
	void setVel(const myVector4f& vel);
	void drawCollideSphere();

	void render();
	void update(float eTime);

	void checkLifetime();

	void generate(const myVector4f& Bulletpos); /* 총알이 터지는 시점의 위치를 받아서 파티클을 생성합니다. */
	void generate(const SpaceProbe& pos);		/* 행성에 충돌하여 터질 때 생성합니다. */
	void destroy();
};

class Bullet {
private:
	/* render 편의성을 위한 Sphere 결정요건 */
	const int SLICES{ 6 };
	const int STACKS{ 6 };
	const float DEFAULT_RADIUS{ 3.f };
private:
	myVector4f pos;									/* 중심좌표 */
	myVector4f vel;									/* 속도 */
	myVector4f color{ 1.f,1.f,1.f };
	float radius{ DEFAULT_RADIUS };					/* 반지름 */
	float collideSphereRadius{ DEFAULT_RADIUS };	/* 충돌박스 반지름 */

public:
	bool CollideSphereFlag{ false };
	bool isAlive{ false };							/* 살아있는가 */

	Bullet();
	~Bullet();

	myVector4f getPos() const;
	void setPos(const myVector4f& pos);

	myVector4f getVel() const;
	void setVel(const myVector4f& vel);

	float getRadius() const;

	void drawCollideSphere();
	void makeParticle(Particle* pList);

	void render();
	void update(float eTime);

	void checkWorldOut();

	void generate(const SpaceProbe& character); /* 캐릭터의 재원값을 받아서 생성합니다. */
	void destroy();
};
