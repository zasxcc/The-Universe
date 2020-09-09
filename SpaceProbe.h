#pragma once
#include "CGMath.h"
#include "CGCamera.h"

/* 개발 중. 12월 9일 */

constexpr int IDX_CHARACTER			{ 0 };

constexpr int KIND_NONE				{ -1 };
constexpr int KIND_CHARACTER		{ 0 };
constexpr int KIND_ENEMY			{ 1 };
constexpr float DEFAULT_SIZE		{ 10.f };			/* 주인공 기본 크기 */
constexpr float DEFAULT_ENEMY_SIZE	{ 27.5f };
constexpr float MAX_VELOCITY		{ 0.f };

constexpr int MAX_PROBES			{ 21 };				/* 스페이스 쉽 개수 */

constexpr float DEFAULT_DISTANCE	{ 50.f };
constexpr float MAX_DISTANCE		{ 100.f };

class SpaceProbe {
private:
	const int SLICES{ 10 };
	const int STACKS{ 5 };
	const int MAX_HEALTH{ 10 };
	const int MAX_ENEMY_HEALTH{ 3 };
private:
	myVector4f pos;								/* 우주선 중심부 */
	myVector4f vel;								/* 우주선 속도 */
	myVector4f accel;							/* 우주선 가속도 */
	myVector4f look;							/* 우주선 시선벡터 == 속도 정규화 */
	myVector4f right;							/* 우주선 우측벡터 */
	myVector4f up;								/* 우주선 상향벡터 */
	int health{ 0 };							/* 우주선 생명력 */
	int kinds{ KIND_NONE };
	float pitch{ 0.f };							/* Y 각도 == 상하각도 */
	float yaw{ 0.f };							/* X 각도 == 좌우각도 */
	float roll{ 0.f };							/* Bank각도 == 고개 갸우뚱할 때의 각도 */
	float collideSphereRadius{ 0.f };			/* 충돌박스 반지름 */
	float cameraDistance{ 0.f };				/* 카메라로부터의 이격거리 */
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

	void setCameraDistance(unsigned char input, float eTime);				//	주인공의 카메라부터 이격거리

	void update(float eTime, const Camera& camera);							//	주인공 전용
	void update(float eTime, const SpaceProbe& character);					//	적 전용
	void rotate();

	void sychronizeCamera(const Camera& camera);

	void render();
	void drawEnemyModel();
	void drawCharacterModel();

	void drawCollideSphere();

	void generate();
	void destroy();

	void healthCheck();														//	체력이 0이면 파괴합니다.
	void chaseCharacter(const SpaceProbe& character);						//	주인공을 추적합니다.

};