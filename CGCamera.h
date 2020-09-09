#pragma once
/* �ۼ��� - 2013182024 KPU SMS, helped by LSK

last write date : 2018. 12. 8

ī�޶� �̵� �ϼ�.

�װ��� �⺻ 3�� ���� ������Ű https://namu.wiki/w/%ED%95%AD%EA%B3%B5%EA%B8%B0%EC%9D%98%20%EA%B8%B0%EB%B3%B8%203%EC%B6%95
*/

#include <gl/freeglut.h> /* LibraryHeader ���� ������ */
#include "CGMath.h"

/* View Frustum, */
constexpr double frustum_fovy{ 60.f };
constexpr double frustum_Zfar{ 60000.f }; 
constexpr double frustum_Znear{ 1.f };

constexpr float DEFAULT_CAMERA_SPEED{ 500.f };
constexpr float CHEAT_CAMERA_SPEED{ 1500.f };

class Camera {
public:
	enum ProjectionOption : int
	{
		Orthographic,
		Perspective
	};
private:
	ProjectionOption mode;	/* ī�޶� ���� ��� */
	myVector4f pos;			/* ī�޶� ��ġ */
	myVector4f vel;			/* ī�޶� �̵��ӵ� */
	myVector4f accel;		/* ī�޶� ���ӵ� */
	myVector4f look;		/* ī�޶��� �ü����� */
	myVector4f up;			/* ī�޶��� ���⺤�� */
	myVector4f right;		/* ī�޶��� �������� */
	float yaw;				/* ��, ��  */
	float roll;				/* �̵� ���� �࿡ ���� ȸ��, ���� ���� �� �����, Bank �Ѵٰ� ǥ���մϴ�. */
	float pitch;			/* ��, �� (������ �� ��) */
	float speed{ DEFAULT_CAMERA_SPEED };

public:
	Camera();
	~Camera();

	myVector4f getPos() const;
	void setPos(float x, float y, float z);
	void setPos(const myVector4f& pos);

	myVector4f getLook() const;
	void setAccel(const myVector4f& accel);

	void LookAt(const myVector4f& AT);
	/*  �ٶ󺸴� ������ at�� ���� look ���͸� ����ȭ�Ͽ� ���ϰ�
		���� y ��� �����Ͽ� right�� ����, ���� look�� right�� ������ ���� up���� ���� */
	void Rotate(float pitch, float yaw, float roll);		/* ī�޶� ��ü�� ȸ�� */

	void setProjectionMode(const ProjectionOption& input);
	void ModifyProjectionMode(int w, int h);
	void ModePerspective(int w, int h);
	void ModeOrtho(int w, int h);

	void update(float eTime);

	/* ī�޶� io ó�� */
	void CameraKeyMoving(unsigned char key, float eTime);

	/* 
	���� �䱸���� - �������� �̵�, ���콺 �����ӿ� ���� ī�޶� x, y�� ȸ�� - <�Ϸ�> 
	�߰� ���߻��� - ������Ʈ�� AT�� �����ǰ�, 360�� �� ��ǥ���� ���ͺ��ϴ� ��� - <�ٸ� ������� ����>
	>> ī�޶� �������� ������, ī�޶� ȸ���� ���� ������Ʈ�� ����������. <�Ϸ�>
	*/
};

