#pragma once
#include "LibraryHeader.h"
#include "CGMath.h"
#include "SpaceProbe.h"

constexpr int MAX_BULLET{ 50 };
constexpr int MAX_PARTICLE{ 20 };

class Particle {
private:
	/* render ���Ǽ��� ���� Sphere ������� */
	const int SLICES{ 6 };
	const int STACKS{ 6 };
	const float DEFAULT_RADIUS{ 3.f };
public:
	/* ���� �ð� */
	const float LIFETIME_FULL{ 5.f };
private:
	myVector4f pos;									/* �߽���ǥ */
	myVector4f vel;									/* �ӵ� */
	myVector4f color{ 1.f,1.f,1.f };
	bool isAlive{ false };							/* ����ִ°� */
	float lifetime{ LIFETIME_FULL };				/* ���� �ð� */
	float radius{ DEFAULT_RADIUS };					/* ������ */
	float collideSphereRadius{ DEFAULT_RADIUS };	/* �浹�ڽ� ������ */

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

	void generate(const myVector4f& Bulletpos); /* �Ѿ��� ������ ������ ��ġ�� �޾Ƽ� ��ƼŬ�� �����մϴ�. */
	void generate(const SpaceProbe& pos);		/* �༺�� �浹�Ͽ� ���� �� �����մϴ�. */
	void destroy();
};

class Bullet {
private:
	/* render ���Ǽ��� ���� Sphere ������� */
	const int SLICES{ 6 };
	const int STACKS{ 6 };
	const float DEFAULT_RADIUS{ 3.f };
private:
	myVector4f pos;									/* �߽���ǥ */
	myVector4f vel;									/* �ӵ� */
	myVector4f color{ 1.f,1.f,1.f };
	float radius{ DEFAULT_RADIUS };					/* ������ */
	float collideSphereRadius{ DEFAULT_RADIUS };	/* �浹�ڽ� ������ */

public:
	bool CollideSphereFlag{ false };
	bool isAlive{ false };							/* ����ִ°� */

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

	void generate(const SpaceProbe& character); /* ĳ������ ������� �޾Ƽ� �����մϴ�. */
	void destroy();
};
