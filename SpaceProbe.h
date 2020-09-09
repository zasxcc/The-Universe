#pragma once
#include "CGMath.h"
#include "CGCamera.h"

/* ���� ��. 12�� 9�� */

constexpr int IDX_CHARACTER			{ 0 };

constexpr int KIND_NONE				{ -1 };
constexpr int KIND_CHARACTER		{ 0 };
constexpr int KIND_ENEMY			{ 1 };
constexpr float DEFAULT_SIZE		{ 10.f };			/* ���ΰ� �⺻ ũ�� */
constexpr float DEFAULT_ENEMY_SIZE	{ 27.5f };
constexpr float MAX_VELOCITY		{ 0.f };

constexpr int MAX_PROBES			{ 21 };				/* �����̽� �� ���� */

constexpr float DEFAULT_DISTANCE	{ 50.f };
constexpr float MAX_DISTANCE		{ 100.f };

class SpaceProbe {
private:
	const int SLICES{ 10 };
	const int STACKS{ 5 };
	const int MAX_HEALTH{ 10 };
	const int MAX_ENEMY_HEALTH{ 3 };
private:
	myVector4f pos;								/* ���ּ� �߽ɺ� */
	myVector4f vel;								/* ���ּ� �ӵ� */
	myVector4f accel;							/* ���ּ� ���ӵ� */
	myVector4f look;							/* ���ּ� �ü����� == �ӵ� ����ȭ */
	myVector4f right;							/* ���ּ� �������� */
	myVector4f up;								/* ���ּ� ���⺤�� */
	int health{ 0 };							/* ���ּ� ����� */
	int kinds{ KIND_NONE };
	float pitch{ 0.f };							/* Y ���� == ���ϰ��� */
	float yaw{ 0.f };							/* X ���� == �¿찢�� */
	float roll{ 0.f };							/* Bank���� == �� ������� ���� ���� */
	float collideSphereRadius{ 0.f };			/* �浹�ڽ� ������ */
	float cameraDistance{ 0.f };				/* ī�޶�κ����� �̰ݰŸ� */
	float WingAngle{ 0.f };

public:
	bool CollideSphereFlag{ false };
	bool isAlive{ false };
	bool isChasing{ true };
	bool isCameraLocked{ false };

	SpaceProbe();
	~SpaceProbe();
	
	void setPos(const myVector4f& pos);
	myVector4f getPos() const;

	void setVel(const myVector4f& vel);
	myVector4f getVel() const;

	void setAccel(const myVector4f& accel);
	myVector4f getAccel() const;

	float getRadius() const;

	void setKinds(int kinds);
	int getKinds() const;

	void setHealth(int health);
	int getHealth() const;

	void setLook(const myVector4f& look);
	myVector4f getLook() const;

	void setWingAngle(float anlge);

	void setCameraDistance(unsigned char input, float eTime);				//	���ΰ��� ī�޶���� �̰ݰŸ�

	void update(float eTime, const Camera& camera);							//	���ΰ� ����
	void update(float eTime, const SpaceProbe& character);					//	�� ����
	void rotate();

	void sychronizeCamera(const Camera& camera);

	void render();
	void drawEnemyModel();
	void drawCharacterModel();

	void drawCollideSphere();

	void generate();
	void destroy();

	void healthCheck();														//	ü���� 0�̸� �ı��մϴ�.
	void chaseCharacter(const SpaceProbe& character);						//	���ΰ��� �����մϴ�.

};