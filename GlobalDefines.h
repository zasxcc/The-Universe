#pragma once
#include "Planet.h"
#include "SpaceProbe.h"
#include "Bullet.h"
/* ������ ��ġ��, ��� �ҽ����� �̿�� ���ɼ��� �ִ� ��ũ�ο� �Լ� ���Ǹ� �����մϴ�. 
	�ݹ� �Լ����� �����ϰ� ������ �뵵�Դϴ�. */

/* ��ũ�� ��� ��ſ� constexpr ����� �غ���! */

/* ȭ�� ����Ʈ ����
	ȭ�鰪�� ����� ���� glutGet()�� ���ؼ� ���� ����
	MY_WINDOW_WIDTH, HEIGHT�� �̿����ּ���.
	CB_reshape ���ڰ��� ��������� �Ѱ��ְ� �ֽ��ϴ�. */
constexpr int MY_WINDOW_WIDTH	{ 1300 };
constexpr int MY_WINDOW_HEIGHT	{ 700 };
constexpr int NOT_BE_INITIATED	{ -1 };
const int MAX_MONITOR_WIDTH		{ GetSystemMetrics(SM_CXSCREEN) };
const int MAX_MONITOR_HEIGHT	{ GetSystemMetrics(SM_CYSCREEN) };
const int CENTRE_MONITOR_WIDTH	{ MAX_MONITOR_WIDTH / 2 };
const int CENTRE_MONITOR_HEIGHT	{ MAX_MONITOR_HEIGHT / 2 };

//	���� �缳 �Լ��� �����մϴ�.
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

//	���⿡ �ݹ� �Լ��� �����մϴ�.
/* CB_ ���λ�� CallBack funtion�� �ǹ��մϴ�. */
void CB_mouseMoveFunc(int button, int state, int x, int y);
void CB_keyDownFunc(unsigned char key, int x, int y);
void CB_keyUpFunc(unsigned char key, int x, int y);
void CB_specialkeyDownFunc(int key, int x, int y);	/* GL���� ArrowŰ �Ϸ��� specialkeyfunc�� */
void CB_specialkeyUpFunc(int key, int x, int y);
void CB_idle(void);
void CB_reshape(int width, int height);
void CB_renderScene(void);


void CB_mousePressedMotionFunc(int x, int y);
void CB_mousePassiveMotionFunc(int x, int y);		/* Ŭ������ ���콺 �̵� ���� */
/* 
	glutMotionFunc and glutPassiveMotionFunc set the motion and passive motion callback respectively for the current window.
	The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
	The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.

	��ó : https://www.opengl.org/resources/libraries/glut/spec3/node51.html#SECTION00086000000000000000
*/