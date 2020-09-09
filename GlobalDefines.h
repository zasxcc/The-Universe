#pragma once
#include "Planet.h"
#include "SpaceProbe.h"
#include "Bullet.h"
/* 윈도우 수치나, 모든 소스에서 이용될 가능성이 있는 매크로와 함수 정의를 선언합니다. 
	콜백 함수들을 선언하고 정의할 용도입니다. */

/* 매크로 사용 대신에 constexpr 사용을 해보자! */

/* 화면 뷰포트 설정
	화면값을 얻야할 때는 glutGet()을 통해서 얻지 말고
	MY_WINDOW_WIDTH, HEIGHT를 이용해주세요.
	CB_reshape 인자값을 상수값으로 넘겨주고 있습니다. */
constexpr int MY_WINDOW_WIDTH	{ 1300 };
constexpr int MY_WINDOW_HEIGHT	{ 700 };
constexpr int NOT_BE_INITIATED	{ -1 };
const int MAX_MONITOR_WIDTH		{ GetSystemMetrics(SM_CXSCREEN) };
const int MAX_MONITOR_HEIGHT	{ GetSystemMetrics(SM_CYSCREEN) };
const int CENTRE_MONITOR_WIDTH	{ MAX_MONITOR_WIDTH / 2 };
const int CENTRE_MONITOR_HEIGHT	{ MAX_MONITOR_HEIGHT / 2 };

//	여기 사설 함수를 선언합니다.
void Initiate();
void Release();

void Update(float eTime);
void updateWorld(float eTime);
void updateBullet(float eTime);
void updateParticle(float eTime);

void drawBullet();
void drawParticle();
void drawSpaceship();

void drawWorld();
void MakeSpace();
void MakeSun();
void MakeMercury();
void MakeVenus();
void MakeMoon();
void MakeEarth();
void MakeMars();
void MakeJupiter();
void MakeSaturn();
void MakeUranus();
void MakeNeptune();

void ViewCollideBox();
bool GravityCheck(const Planet& p, const SpaceProbe& s);
bool CollideCheck(const Planet& p, const SpaceProbe& s);
bool CollideCheck(const Bullet& b, const Planet& p);
bool CollideCheck(const Bullet& b, const SpaceProbe& s);

void regenerateCharacter(SpaceProbe& main, float eTime);
void spawnEnemy(SpaceProbe * enemyList, float eTime);
void applyGravity(const Planet& p, Camera& s);

void WingRecover(float eTime);
void WingSet(float eTime);

//	여기에 콜백 함수를 선언합니다.
/* CB_ 접두사는 CallBack funtion을 의미합니다. */
void CB_mouseMoveFunc(int button, int state, int x, int y);
void CB_keyDownFunc(unsigned char key, int x, int y);
void CB_keyUpFunc(unsigned char key, int x, int y);
void CB_specialkeyDownFunc(int key, int x, int y);	/* GL에서 Arrow키 하려면 specialkeyfunc에 */
void CB_specialkeyUpFunc(int key, int x, int y);
void CB_idle(void);
void CB_reshape(int width, int height);
void CB_renderScene(void);


void CB_mousePressedMotionFunc(int x, int y);
void CB_mousePassiveMotionFunc(int x, int y);		/* 클릭없이 마우스 이동 갱신 */
/* 
	glutMotionFunc and glutPassiveMotionFunc set the motion and passive motion callback respectively for the current window.
	The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
	The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.

	출처 : https://www.opengl.org/resources/libraries/glut/spec3/node51.html#SECTION00086000000000000000
*/