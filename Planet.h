#pragma once
#include "CGMath.h"

/* ���÷� ���Ģ
	1.	�༺�� �¾��� �� �������� �ϴ� Ÿ���˵��� �׸��鼭 �����Ѵ�. (Ÿ���˵� ��Ģ)
	2.	�༺�� �¾��� �����ϴ� �������� ������ ���� �ð� ���� ���� �������� ������ �׻� ����.
	3.	�༺�� �����ֱ��� ������ �˵��� ��������� �������� ����Ѵ�.
*/
/* �Ϲݻ�뼺 �̷�ó�� �ð��� ����� �е��� ������ ���� �����Ǵ� ���� �ƴ϶�
	�������п����� ������ �߷��� �� �������. 
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
	/* �༺�� �����Ͽ��� '��'��� �����Ѵ�. */
private:
	myVector4f centre;	/* pos, ���� */
	myVector4f vel;
	myVector4f accel;
	myVector4f look;	/* ������ �¾��� �ٶ󺻴�. �¾��� ��� look�� World Z����. */
	myVector4f up;		
	myVector4f right;	/* � ���� */
	float speed_move{ 300.f };
	float speed_rotate{ 0.f };
	float mass{ 0.f };
	float radius{ 0.f };
	float gravityFieldRadius{ 0.f };	/* �� �߷����� ������ */
	float collideSphereRadius{ 0.f };	/* �浹�ڽ� ������ */
	float angle_rotationAxis{ 0.f };	//	������ ����
	float angle_rotate{ 0.f };			//	������
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
	void rotateFaceEarth();			/* �� ���� �Լ� */
	void MakeSphere(float posX, float posY, float posZ, float radius, int n); // n�� ���� �ձ۱� (Ŭ���� �ձ۵ձ�)

	void drawRing();
};
