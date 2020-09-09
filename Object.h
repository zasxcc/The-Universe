#pragma once
#include "CGMath.h"
#include "GlobalDefines.h"
#include "ProjectDefines.h"

/* for ���� 1 */
constexpr int NONE{ 0 };
constexpr int RECTANGLE{ 1 };
constexpr int DIAMOND{ 2 };
constexpr int TRIANGLE{ 3 };
constexpr int CIRCLE{ 4 };
constexpr int STAR{ 5 };

constexpr int STATIC{ 0 };
constexpr int DYNAMIC{ 1 };

constexpr float rectEdgeHalf{ 15.0f };	/* rectangle's Edge's half, �ﰢ���� ���� �����簢�� */
constexpr float rectEdge{ 2.f * rectEdgeHalf };	/* rectangle's Edge */

//constexpr

class Object {
private:
	myVector4f pos;
	myVector4f vel;
	myVector4f accel;
	myVector4f color;
	GLfloat mass{ 0.f };		/* ���, ������ ���� ������ ���� �ٽ� ��Ȱ���Ѿ��� */
	GLfloat angle{ 0.f };		/* z�� ȸ��, degree */
	int shape{ NONE };			/* ������ ��� */
	int state{ STATIC };		/* ���� - STATIC(����), DYNAMIC(�߷� ����)*/
	myVector4f destination;
	GLfloat size{ 1.f };
public:

	bool isAlive{ true };		/* Render ���� */
	bool isClicked{ false };	/* MouseClick, Ŭ�� �� ���콺�� ������*/

	void getPos(myVector4f* pos);
	void setPos(const myVector4f& pos);

	void getVel(myVector4f* vel);
	void setVel(const myVector4f& vel);

	void getAccel(myVector4f* accel);
	void setAccel(const myVector4f& accel);

	void getColor(myVector4f* rgba);
	void setColor(const myVector4f& rgba);

	void getMass(GLfloat* mass);
	void setMass(GLfloat mass);

	GLfloat getAngle();
	void setAngle(GLfloat degree);

	int getShape();
	void setShape(int shape);

	int getState();
	void setState(int state);

	bool getLifeSign();
	void setLifeSign(bool truefalse);

	bool getMouseClicked();
	void setMouseClicked(bool truefalse);

	void setDestination(const myVector4f& dest);

	void setSize(GLfloat size);
	GLfloat getSize();

	void initiate();
	void update(float eTime);
	//void addForce(const myVector4f& force, float eTime); /* �ϴ� ��� */
	void drawShape();
	void render();
	bool collideCheck();
};