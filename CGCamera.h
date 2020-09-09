#pragma once
/* 작성자 - 2013182024 KPU SMS, helped by LSK

last write date : 2018. 12. 8

카메라 이동 완성.

항공기 기본 3축 참고 꺼무위키 https://namu.wiki/w/%ED%95%AD%EA%B3%B5%EA%B8%B0%EC%9D%98%20%EA%B8%B0%EB%B3%B8%203%EC%B6%95
*/

#include <gl/freeglut.h> /* LibraryHeader 없이 쓰려고 */
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
	ProjectionOption mode;	/* 카메라 설정 모드 */
	myVector4f pos;			/* 카메라 위치 */
	myVector4f vel;			/* 카메라 이동속도 */
	myVector4f accel;		/* 카메라 가속도 */
	myVector4f look;		/* 카메라의 시선벡터 */
	myVector4f up;			/* 카메라의 상향벡터 */
	myVector4f right;		/* 카메라의 우측벡터 */
	float yaw;				/* 좌, 우  */
	float roll;				/* 이동 방향 축에 대한 회전, 쉽게 말해 고개 갸우뚱, Bank 한다고도 표현합니다. */
	float pitch;			/* 상, 하 (고도조절 할 때) */
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
	/*  바라보는 점벡터 at을 통해 look 벡터를 정규화하여 구하고
		월드 y 축과 외적하여 right를 정렬, 이후 look과 right로 외적을 통해 up벡터 정렬 */
	void Rotate(float pitch, float yaw, float roll);		/* 카메라 객체의 회전 */

	void setProjectionMode(const ProjectionOption& input);
	void ModifyProjectionMode(int w, int h);
	void ModePerspective(int w, int h);
	void ModeOrtho(int w, int h);

	void update(float eTime);

	/* 카메라 io 처리 */
	void CameraKeyMoving(unsigned char key, float eTime);

	/* 
	개발 요구사항 - 자유시점 이동, 마우스 움직임에 따른 카메라 x, y축 회전 - <완료> 
	추가 개발사항 - 오브젝트에 AT이 고정되고, 360도 구 좌표에서 쿼터뷰하는 방식 - <다른 방식으로 구현>
	>> 카메라가 포지션을 가지고, 카메라 회전에 의해 오브젝트가 움직여진다. <완료>
	*/
};

