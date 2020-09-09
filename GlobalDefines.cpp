#include "LibraryHeader.h"
#include "GlobalDefines.h"
#include "CGTime.h"
#include "CGCamera.h"
#include "Planet.h"
#include "Texture.h"
#include "SpaceProbe.h"
#include "Sound.h"
#include "Bullet.h"
#include "Light.h"


Sound g_sound;		//	����� ��� ���� �۷ι� ����
Light g_light;		//	�� ����.
Camera g_camera;	//	ī�޶�
/* �۷ι� Ű �ο︰ */
bool g_keyW					{ false };
bool g_keyA					{ false };
bool g_keyS					{ false };
bool g_keyD					{ false };
bool g_keySpace				{ false };
bool isCheatOn				{ false };
bool isPaused				{ false };
bool isGravityON			{ true };
bool g_keyPlus				{ false };
bool g_keyMinus				{ false };
bool g_isInGravityField		{ false };
int g_prevX = CENTRE_MONITOR_WIDTH;
int g_prevY = CENTRE_MONITOR_HEIGHT;
GLuint g_textureID[MAX_TEXTURE];

/* ���ּ��� */
SpaceProbe spaceProbe[MAX_PROBES];			//	0���� �÷��̾�, �������� ��
float WingAngle = 0;
/* �Ѿ� */
Bullet bullet[MAX_BULLET];
int g_bulletCurrIDX{ 0 };
float g_BLT_passedTime{ 0.f };				//	�Ѿ� ���� ���� ������ �ð�����

Bullet enemybullet[MAX_BULLET];
int g_bulletEnemyCurrIDX{ 0 };
float g_ENEMYBLT_passedTime[MAX_PROBES - 1]{ 0.f };	//	����ü �Ѿ� ���� ���� ������ �ð�����

/* ��ƼŬ */
Particle g_particleDestory[MAX_PARTICLE];	//	SpaceProbe �ı��� ��ƼŬ
Particle g_particleCollide[MAX_PARTICLE];	//	Planet�� Bullet �浹�� ��ƼŬ

///////////////// �༺�� ����
Planet space{ NAME::SPACE };
Planet sun{ NAME::SUN };
Planet mercury{ NAME::MERCURY };
Planet venus{ NAME::VENUS };
Planet moon{ NAME::MOON };
Planet earth{ NAME::EARTH };
Planet earthCloud{ NAME::EARTH_CLOUD };
Planet mars{ NAME::MARS };
Planet jupiter{ NAME::JUPITER };
Planet saturn{ NAME::SATURN };
Planet neptune{ NAME::NEPTUNE };
Planet uranus{ NAME::URANUS };
///////////////////////////

void CB_idle(void)
{
	CB_renderScene();
}

void CB_reshape(int width, int height)
{
	/* key �Է��� ���ؼ� ī�޶� �ٲ� ��쿡 Reshape�� ȣ��������� - ������ȯ�� ���Ӱ� �Ϸ��� */
	g_camera.ModifyProjectionMode(width, height);
}

void CB_renderScene(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	float eTime = calculateElapseTime();
	//printf("%f\n", eTime);	//*********************** TEST
	
	/* ���� ������ ������Ʈ �Լ��� ȣ���մϴ�. �� ó���� ������ �ð� �������� ó���մϴ�. */

	Update(eTime);
	
	/* �� �Ʒ��� ��ü���� �׷��־��ּ���. */

	g_light.setLight();

	drawBullet();
	
	drawParticle();

	drawSpaceship();
	
	/* �༺�� */
	drawWorld();

	glutSwapBuffers();	//	swap function of double buffering 
}

void Update(float eTime)
{
	/* ī�޶� key input */
	if (spaceProbe[IDX_CHARACTER].isAlive) {
		if (!g_isInGravityField) { g_camera.CameraKeyMoving('0', eTime); }
		if (g_keyW) { g_camera.CameraKeyMoving('w', eTime); }
		if (g_keyA) { g_camera.CameraKeyMoving('a', eTime); }
		if (g_keyS) { g_camera.CameraKeyMoving('s', eTime); }
		if (g_keyD) { g_camera.CameraKeyMoving('d', eTime); }
		if (g_keyPlus) { spaceProbe[IDX_CHARACTER].setCameraDistance('+', eTime); }
		if (g_keyMinus) { spaceProbe[IDX_CHARACTER].setCameraDistance('-', eTime); }
		g_camera.update(eTime);
	}
	CB_reshape(MY_WINDOW_WIDTH, MY_WINDOW_HEIGHT);

	/* ���⿡ ��ü ������Ʈ�� �ۼ��մϴ�. ################# */

	/* �Ѿ� */
	updateBullet(eTime);

	/* ��ƼŬ */
	updateParticle(eTime);

	/* ���ΰ� */
	/* ���� �ִϸ��̼� */
	WingSet(eTime);
	WingRecover(eTime);
	spaceProbe[IDX_CHARACTER].setWingAngle(WingAngle);
	spaceProbe[IDX_CHARACTER].update(eTime, g_camera);
	regenerateCharacter(spaceProbe[IDX_CHARACTER], eTime);
	if (g_keySpace && spaceProbe[IDX_CHARACTER].isAlive) {
		/* 0.3�ʸ��� 1�� �����ϵ��� �� ����. */
		if (g_BLT_passedTime > 0.1f) { /* ��� �ÿ� 0.1���� ������ �ð��� ������. */
			g_BLT_passedTime = 0.f;
			bullet[g_bulletCurrIDX].generate(spaceProbe[IDX_CHARACTER]);
			if (g_bulletCurrIDX == MAX_BULLET - 1) g_bulletCurrIDX = 0;
			g_bulletCurrIDX++;
		}
		else {
			g_BLT_passedTime += eTime;
		}
	}

	/* �� */
	if (!isPaused) {
		for (int i = 1; i < MAX_PROBES; ++i) {
			spawnEnemy(spaceProbe, eTime);
			spaceProbe[i].update(eTime, spaceProbe[IDX_CHARACTER]);
			if (!spaceProbe[i].isChasing && spaceProbe[i].isAlive) {
				if (g_ENEMYBLT_passedTime[i - 1] > 5.f) { /* ��� �ÿ� 5���� ������ �ð��� ������. */
					g_ENEMYBLT_passedTime[i - 1] = 0.f;
					enemybullet[g_bulletEnemyCurrIDX].generate(spaceProbe[i]);
					if (g_bulletEnemyCurrIDX == MAX_BULLET - 1) { g_bulletEnemyCurrIDX = 0; }
					g_bulletEnemyCurrIDX++;
				}
				else {
					g_ENEMYBLT_passedTime[i - 1] += eTime;
				}
			}
		}
	}

	/* �༺ ������Ʈ */
	updateWorld(eTime);
}

void updateWorld(float eTime)
{
	/* �༺ ������Ʈ */
	{
		if (!isPaused) { /* �Ͻ����� */
			mercury.update(eTime);
			venus.update(eTime);
			earth.update(eTime);
			mars.update(eTime);
			jupiter.update(eTime);
			saturn.update(eTime);
			uranus.update(eTime);
			neptune.update(eTime);
		}
		/* �ÿ��� ���� ������ ���� ������ �Ͻ������� ���� �ʴ´�. */
		earthCloud.update(eTime);
		moon.update(eTime);
	}
	/* ���ΰ��� �༺ �߷��� �� �浹�˻� */
	{
		if (isGravityON) { /* �߷� ���� ON �϶��� */
			g_isInGravityField = true;
			if (GravityCheck(mercury, spaceProbe[IDX_CHARACTER])) {
				applyGravity(mercury, g_camera);
			}
			else if (GravityCheck(venus, spaceProbe[IDX_CHARACTER])) {
				applyGravity(venus, g_camera);
			}
			else if (GravityCheck(earth, spaceProbe[IDX_CHARACTER])) {
				applyGravity(earth, g_camera);
			}
			else if (GravityCheck(mars, spaceProbe[IDX_CHARACTER])) {
				applyGravity(mars, g_camera);
			}
			else if (GravityCheck(jupiter, spaceProbe[IDX_CHARACTER])) {
				applyGravity(jupiter, g_camera);
			}
			else if (GravityCheck(saturn, spaceProbe[IDX_CHARACTER])) {
				applyGravity(saturn, g_camera);
			}
			else if (GravityCheck(uranus, spaceProbe[IDX_CHARACTER])) {
				applyGravity(uranus, g_camera);
			}
			else if (GravityCheck(neptune, spaceProbe[IDX_CHARACTER])) {
				applyGravity(neptune, g_camera);
			}
			else if (GravityCheck(moon, spaceProbe[IDX_CHARACTER])) {
				applyGravity(moon, g_camera);
			}
			else {
				g_isInGravityField = false;
			}
		}
		else {
			g_isInGravityField = false;
		}
	}
	/* ��� ���ּ��� �༺ �� �����浹 �˻� */
	{
		for (int i = 0; i < MAX_PROBES; ++i) {
			if (isCheatOn && i == 0) { continue; }
			if (CollideCheck(sun, spaceProbe[i]) && spaceProbe[i].isAlive) {
				for (int j = 0; j < MAX_PARTICLE; ++j) {
					g_particleDestory[j].generate(spaceProbe[i]);
				}
				spaceProbe[i].destroy();
			}
			if (CollideCheck(mercury, spaceProbe[i]) && spaceProbe[i].isAlive) {
				for (int j = 0; j < MAX_PARTICLE; ++j) {
					g_particleDestory[j].generate(spaceProbe[i]);
				}
				spaceProbe[i].destroy();
			}
			if (CollideCheck(venus, spaceProbe[i]) && spaceProbe[i].isAlive) {
				for (int j = 0; j < MAX_PARTICLE; ++j) {
					g_particleDestory[j].generate(spaceProbe[i]);
				}
				spaceProbe[i].destroy();
			}
			if (CollideCheck(earth, spaceProbe[i]) && spaceProbe[i].isAlive) {
				for (int j = 0; j < MAX_PARTICLE; ++j) {
					g_particleDestory[j].generate(spaceProbe[i]);
				}
				spaceProbe[i].destroy();
			}
			if (CollideCheck(mars, spaceProbe[i]) && spaceProbe[i].isAlive) {
				for (int j = 0; j < MAX_PARTICLE; ++j) {
					g_particleDestory[j].generate(spaceProbe[i]);
				}
				spaceProbe[i].destroy();
			}
			if (CollideCheck(jupiter, spaceProbe[i]) && spaceProbe[i].isAlive) {
				for (int j = 0; j < MAX_PARTICLE; ++j) {
					g_particleDestory[j].generate(spaceProbe[i]);
				}
				spaceProbe[i].destroy();
			}
			if (CollideCheck(saturn, spaceProbe[i]) && spaceProbe[i].isAlive) {
				for (int j = 0; j < MAX_PARTICLE; ++j) {
					g_particleDestory[j].generate(spaceProbe[i]);
				}
				spaceProbe[i].destroy();
			}
			if (CollideCheck(uranus, spaceProbe[i]) && spaceProbe[i].isAlive) {
				for (int j = 0; j < MAX_PARTICLE; ++j) {
					g_particleDestory[j].generate(spaceProbe[i]);
				}
				spaceProbe[i].destroy();
			}
			if (CollideCheck(neptune, spaceProbe[i]) && spaceProbe[i].isAlive) {
				for (int j = 0; j < MAX_PARTICLE; ++j) {
					g_particleDestory[j].generate(spaceProbe[i]);
				}
				spaceProbe[i].destroy();
			}
			if (CollideCheck(moon, spaceProbe[i]) && spaceProbe[i].isAlive) {
				for (int j = 0; j < MAX_PARTICLE; ++j) {
					g_particleDestory[j].generate(spaceProbe[i]);
				}
				spaceProbe[i].destroy();
			}
		}
	}
	/* �÷��̾� �Ѿ˰� �༺ �� �����浹 �˻� */
	{
		for (int i = 0; i < MAX_BULLET; ++i) {
			if (CollideCheck(bullet[i], sun)) {
				bullet[i].makeParticle(g_particleCollide);
			}
			if (CollideCheck(bullet[i], mercury)) {
				bullet[i].makeParticle(g_particleCollide);
			}
			if (CollideCheck(bullet[i], venus)) {
				bullet[i].makeParticle(g_particleCollide);
			}
			if (CollideCheck(bullet[i], earth)) {
				bullet[i].makeParticle(g_particleCollide);
			}
			if (CollideCheck(bullet[i], mars)) {
				bullet[i].makeParticle(g_particleCollide);
			}
			if (CollideCheck(bullet[i], jupiter)) {
				bullet[i].makeParticle(g_particleCollide);
			}
			if (CollideCheck(bullet[i], saturn)) {
				bullet[i].makeParticle(g_particleCollide);
			}
			if (CollideCheck(bullet[i], uranus)) {
				bullet[i].makeParticle(g_particleCollide);
			}
			if (CollideCheck(bullet[i], neptune)) {
				bullet[i].makeParticle(g_particleCollide);
			}
			if (CollideCheck(bullet[i], moon)) {
				bullet[i].makeParticle(g_particleCollide);
			}
		}
	}
	/* �÷��̾� �Ѿ˰� �� ���ּ����� �����浹 �˻� */
	{
		if (!isPaused){
			for (int i = 0; i < MAX_BULLET; ++i) {
				for (int j = 1; j < MAX_PROBES; ++j) {
					if (CollideCheck(bullet[i], spaceProbe[j])) {
						int localLife = spaceProbe[j].getHealth() - 1;
						if (localLife == 0) {
							for (int k = 0; k < MAX_PARTICLE; ++k) {
								g_particleDestory[k].generate(spaceProbe[j]);
							}
						}
						spaceProbe[j].setHealth(localLife);
						bullet[i].makeParticle(g_particleCollide);
					}
				}
			}
		}
	}
	/* �� �Ѿ˰� �÷��̾��� �浹 �˻� */
	{
		if (isCheatOn) { return; } /* ġƮ��忡���� �������� �ʵ��� �Ѵ�. */
		for (int i = 0; i < MAX_BULLET; ++i) {
			if (CollideCheck(enemybullet[i], spaceProbe[IDX_CHARACTER])) {
				int localLife = spaceProbe[IDX_CHARACTER].getHealth() - 1;
				if (localLife == 0) {
					for (int j = 0; j < MAX_PARTICLE; ++j) {
						g_particleDestory[j].generate(spaceProbe[IDX_CHARACTER]);
					}
				}
				spaceProbe[IDX_CHARACTER].setHealth(localLife);
				enemybullet[i].destroy();
			}
		}
	}
}

void updateBullet(float eTime)
{
	for (int i = 0; i < MAX_BULLET; ++i) {
		bullet[i].update(eTime);
		enemybullet[i].update(eTime);
	}
}

void updateParticle(float eTime)
{
	for (int i = 0; i < MAX_PARTICLE; ++i) {
		g_particleCollide[i].update(eTime);
		g_particleDestory[i].update(eTime);
	}
}

void Initiate()
{
	/* ���⿡ �ʱ�ȭ ������ �ۼ��մϴ�. */
	InitiateTexture(g_textureID);

	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<float> urdDirection(-1.f, 1.f); //	���ּ� ��ġ���� ������
	std::uniform_int_distribution<int> uidDistance(27000, 30000);

	/* ���ΰ� �����˵� �� ���� �Ÿ� ��ġ�� ���� ���� */
	{
		myVector4f direction{ urdDirection(dre), 0.f, urdDirection(dre) };
		spaceProbe[IDX_CHARACTER].setKinds(KIND_CHARACTER);
		g_camera.setPos(normalize(direction) * 8500.f);
		printf("The Character is deployed\n");
	}

	/* �� ���ּ� �ʱ� ��ġ ���� */
	for (int i = 1; i < MAX_PROBES; ++i)
	{
		myVector4f direction{ urdDirection(dre), urdDirection(dre), urdDirection(dre) };
		int distance = uidDistance(dre);
		spaceProbe[i].setKinds(KIND_ENEMY);
		spaceProbe[i].setPos(normalize(direction) * distance);
	}
	printf("All Enemy is Generated\n");

	/* ���� �������� ���带 �����մϴ�. */
	g_sound.playSound_BGM();
}

void drawBullet()
{
	for (int i = 0; i < MAX_BULLET; ++i) {
		bullet[i].render();
		enemybullet[i].render();
	}
}

void drawParticle()
{
	for (int i = 0; i < MAX_PARTICLE; ++i) {
		g_particleCollide[i].render();
		g_particleDestory[i].render();
	}
}

void drawSpaceship()
{
	/* �÷��̾� ���ּ� */
	glPushMatrix();
	{
		spaceProbe[0].render();
	}
	glPopMatrix();

	/* �� ���ּ� */
	glPushMatrix();
	{
		for (int i = 1; i < MAX_PROBES; ++i) {
			
			spaceProbe[i].render();
		}
	}
	glPopMatrix();
}

void drawWorld()
{
	glEnable(GL_TEXTURE_2D);
	MakeSpace();
	MakeSun();
	MakeMercury();
	MakeVenus();
	MakeEarth();
	MakeMars();
	MakeJupiter();
	MakeSaturn();
	MakeUranus();
	MakeNeptune();
	glDisable(GL_TEXTURE_2D);
}

void Release()
{
	return;
}

void CB_mousePassiveMotionFunc(int x, int y)
{
	//	Ŀ���� ����ϴ�.
	ShowCursor(FALSE);
	/* setCursorPos() �Լ����� ����ȭ���� LTRB ��ǥ��� �Ķ���� x,y �� �Ⱦ��� GetCursorPos�� ��� */

	float dxConstant = 0.5f; /* ������ ȸ�����, ȸ���� ����, 1�� �ٻ��� ���� �� ���� ���ư� */
	float dyConstant = 0.2f; /* ������ ȸ�����, ȸ���� ����, 1�� �ٻ��� ���� �� ���� ���ư� */

	POINT mouse;
	GetCursorPos(&mouse);
	int dx = g_prevX - mouse.x, dy = g_prevY - mouse.y;
	g_prevX = CENTRE_MONITOR_WIDTH, g_prevY = CENTRE_MONITOR_HEIGHT;

	if (dx == 0 && dy == 0) {
		/* ���ٸ�, SetCursorPos() �Լ��� ��ȣ������ �ʱ� ���ؼ� return. SetCursorPos �Լ��� 
		���콺 �̺�Ʈ�� �߻����� ������ ť�� �� �Լ��� ��� �ݺ��Ǵ� �һ�簡 �߻��߾���. */
		return;
	}
	else {
		g_camera.Rotate(static_cast<float>(dy) * dyConstant, static_cast<float>(dx) * dxConstant, 0.f); /* rolling�� �����ϴ�. */
		CB_renderScene();
		SetCursorPos(CENTRE_MONITOR_WIDTH, CENTRE_MONITOR_HEIGHT);
		/* ȭ�� ȸ������ ������ ��, ���콺 �������� �������Ѵ�. */
	}
}

void CB_keyDownFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
		/* ���α׷� Ŀ�ǵ� */
		/* Arrow Button�� Special Key�� �ؾ��մϴ�. */
	case VK_ESCAPE:	//	ESC
		PostQuitMessage(0);
	case VK_RETURN:	//	Enter
		break;
	case VK_BACK:	//	Back space
		break;
	case VK_SPACE:	//	Space bar - �Ѿ� �߻�
		g_keySpace = true;
		break;

	case 'w':
	case 'W':
		g_keyW = true;
		break;

	case 'a':
	case 'A':
		g_keyA = true;
		break;

	case 's':
	case 'S':
		g_keyS = true;
		break;

	case 'd':
	case 'D':
		g_keyD = true;
		break;

	case 'p':	/* �ִϸ��̼� �Ͻ����� */
	case 'P':
		isPaused = !isPaused;
		break;

	case '8':	/* �߷��� �������� �ʽ��ϴ�. */
		isGravityON = !isGravityON;
		break;

	case '9' :	/* ġƮ��� �ӵ��� �����մϴ�. */
		isCheatOn = !isCheatOn;
		if (isCheatOn) {
			g_camera.CameraKeyMoving('c', 0.f);
		}
		else {
			g_camera.CameraKeyMoving('n', 0.f);
		}
		break;

	case '0':	/* �浹�ڽ��� �����ݴϴ�. */
		ViewCollideBox();
		break;

	case 'l':	/* ī�޶� Locking */
	case 'L':	
		spaceProbe[IDX_CHARACTER].isCameraLocked = !spaceProbe[IDX_CHARACTER].isCameraLocked;
		break;

	case '+':	/* ī�޶� �� ��,�ƿ� */
		g_keyPlus = true;
		break;
	case '-':
		g_keyMinus = true;
		break;

	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void CB_keyUpFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
	case 'W':
		g_keyW = false;
		break;

	case 'a':
	case 'A':
		g_keyA = false;
		break;

	case 's':
	case 'S':
		g_keyS = false;
		break;

	case 'd':
	case 'D':
		g_keyD = false;
		break;

	case '+':
		g_keyPlus = false;
		break;
	case '-':
		g_keyMinus = false;
		break;

	case VK_SPACE:
		g_keySpace = false;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void CB_mouseMoveFunc(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		switch (state)
		{
		case GLUT_DOWN:	/* ���콺 ���� ���� */
			break;
		case GLUT_UP:	/* ���콺 ���� ���� */
			break;
		default:
			printf("MouseMove:LB Nothing\n");
			break;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		switch (state)
		{
		case GLUT_DOWN:	/* ���콺 ������ ���� */
			break;
		case GLUT_UP:	/* ���콺 ������ ���� */
			break;
		default:
			printf("MouseMove:RB Nothing\n");
			break;
		}
		break;

	default:
		printf("MouseMove:Nothing\n");
		break;
	}
	CB_renderScene();
}

void MakeSpace()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[10]);
	glPushMatrix();
	{
		space.render();
	}
	glPopMatrix();
}

void MakeSun()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[1]);
	glPushMatrix();
	{
		sun.render();
	}
	glPopMatrix();
}

void MakeMercury()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[2]);
	glPushMatrix();
	{
		mercury.render();
	}
	glPopMatrix();
}

void MakeVenus()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[3]);
	glPushMatrix();
	{
		venus.render();
	}
	glPopMatrix();
}

void MakeEarth()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[0]);
	glPushMatrix();
	{
		earth.render();
		{
			myVector4f earthPos = earth.getCentre();
			glTranslatef(earthPos.x, earthPos.y, earthPos.z);
			glBindTexture(GL_TEXTURE_2D, g_textureID[11]);
			glPushMatrix();
			{
				earthCloud.render();
			}
			glPopMatrix();
			MakeMoon();
		}
	}
	glPopMatrix();
}

void MakeMoon()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[9]);
	glPushMatrix();
	{
		moon.render();
	}
	glPopMatrix();
}

void MakeMars()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[4]);
	glPushMatrix();
	{
		mars.render();
	}
	glPopMatrix();
}

void MakeJupiter()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[5]);
	glPushMatrix();
	{
		jupiter.render();
	}
	glPopMatrix();
}

void MakeSaturn()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[6]);
	glPushMatrix();
	{
		saturn.render();
	}
	glPopMatrix();
}

void MakeUranus()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[7]);
	glPushMatrix();
	{
		uranus.render();
	}
	glPopMatrix();
}

void MakeNeptune()
{
	glBindTexture(GL_TEXTURE_2D, g_textureID[8]);
	glPushMatrix();
	{
		neptune.render();
	}
	glPopMatrix();
}

void ViewCollideBox()
{
	/* ��� ��ü���� ����� ���Ǿ� ���� �÷��׸� Ȱ�� / ��Ȱ��ȭ ��ŵ�ϴ�. */
	for (int i = 0; i < MAX_BULLET; ++i) {
		/* �Ѿ� */
		bullet[i].CollideSphereFlag = !bullet[i].CollideSphereFlag;
	}
	{
		/* �༺ */
		sun.CollideSphereFlag = !sun.CollideSphereFlag;
		mercury.CollideSphereFlag = !mercury.CollideSphereFlag;
		venus.CollideSphereFlag = !venus.CollideSphereFlag;
		earth.CollideSphereFlag = !earth.CollideSphereFlag;
		moon.CollideSphereFlag = !moon.CollideSphereFlag;
		mars.CollideSphereFlag = !mars.CollideSphereFlag;
		jupiter.CollideSphereFlag = !jupiter.CollideSphereFlag;
		saturn.CollideSphereFlag = !saturn.CollideSphereFlag;
		uranus.CollideSphereFlag = !uranus.CollideSphereFlag;
		neptune.CollideSphereFlag = !neptune.CollideSphereFlag;
	}
	{
		/* ���ּ��� */
		spaceProbe[0].CollideSphereFlag = !spaceProbe[0].CollideSphereFlag;
		for (int i = 1; i < MAX_PROBES; ++i) {
			spaceProbe[i].CollideSphereFlag = !spaceProbe[i].CollideSphereFlag;
		}
	}
}

bool GravityCheck(const Planet& p, const SpaceProbe& s)	//	�߷��� �ȿ� �ִ°�?
{
	float SumRadius = p.getGavityRadius() + s.getRadius();
	float Distance = getDistance(p.getCentre(), s.getPos());
	if (p.getKinds() == NAME::MOON) {
		Distance = getDistance(s.getPos(), p.getCentre() + earth.getCentre());
	}
	if (Distance < SumRadius || isEqual(Distance, SumRadius)) {
		return true;
	}
	return false;
}

bool CollideCheck(const Planet& p, const SpaceProbe& s)			// �༺ ���ּ� �浹
{
	if (!s.isAlive) { return false; }
	float SumRadius = p.getRadius() + s.getRadius();
	float Distance = getDistance(p.getCentre(), s.getPos());
	if (p.getKinds() == NAME::MOON) {
		Distance = getDistance(s.getPos(), p.getCentre() + earth.getCentre());
	}

	if (Distance < SumRadius || isEqual(Distance, SumRadius)) {
		return true;
	}
	return false;
}

bool CollideCheck(const Bullet& b, const Planet& p)					// �Ѿ� �༺ �浹
{
	if (!b.isAlive) { return false; }
	float SumRadius = b.getRadius() + p.getRadius();
	float Distance = getDistance(b.getPos(), p.getCentre());
	if (p.getKinds() == NAME::MOON) {
		Distance = getDistance(b.getPos(), p.getCentre() + earth.getCentre());
	}

	if (Distance < SumRadius || isEqual(Distance, SumRadius)) {
		return true;
	}
	return false;
}

bool CollideCheck(const Bullet& b, const SpaceProbe& s)			// �Ѿ� ���ּ� �浹
{
	float SumRadius = b.getRadius() + s.getRadius();
	float Distance = getDistance(b.getPos(), s.getPos());
	if (Distance < SumRadius || isEqual(Distance, SumRadius)) {
		return true;
	}
	return false;
}

void regenerateCharacter(SpaceProbe & main, float eTime)
{
	static float passedTime{ 0.f };
	if (!main.isAlive) {
		if (passedTime > 5.f) {
			passedTime = 0.f;
			std::random_device rd;
			std::default_random_engine dre(rd());
			std::uniform_real_distribution<float> urdDirection(-1.f, 1.f);
			myVector4f direction{ urdDirection(dre), urdDirection(dre), urdDirection(dre) };
			main.generate();
			g_camera.setPos(earth.getCentre() + normalize(direction) * earth.getGavityRadius());
		}
		else {
			passedTime += eTime;
		}
	}
}

void spawnEnemy(SpaceProbe * enemyList, float eTime)
{
	for (int i = 1; i < MAX_PROBES; ++i) {
		if (!enemyList[i].isAlive) {
			std::random_device rd;
			std::default_random_engine dre(rd());
			std::uniform_real_distribution<float> urdDirection(-1.f, 1.f); //	���ּ� ��ġ���� ������
			std::uniform_int_distribution<int> uidDistance(27000, 30000);
			myVector4f direction{ urdDirection(dre), urdDirection(dre), urdDirection(dre) };
			int distance = uidDistance(dre);
			spaceProbe[i].setKinds(KIND_ENEMY);
			spaceProbe[i].setPos(normalize(direction) * distance);
			spaceProbe[i].generate();
		}
	}
}

void applyGravity(const Planet & p, Camera & main)
{
	myVector4f pos = main.getPos();
	myVector4f look = normalize(p.getCentre() - pos) * p.getGavityRadius() / 10.f;
	if (p.getKinds() == NAME::MOON) {
		look = normalize(p.getCentre() + earth.getCentre() - pos) * p.getGavityRadius() / 10.f;
	}
	main.setAccel(look);
}

void WingRecover(float eTime)
{
	if (g_keyW || g_keyS) { return; }
	float direction{ 0.f - WingAngle };
	float recoverVel{ 25.f };
	if (!isZero(direction)) {
		if (direction > 0.f) {
			WingAngle = WingAngle + recoverVel * eTime;
		}
		else if (direction < 0.f) {
			WingAngle = WingAngle - recoverVel * eTime;
		}
	}
}

void WingSet(float eTime)
{
	if (g_keyW) {
		if (WingAngle < 50.f || isEqual(WingAngle, 50.f)) {
			WingAngle += 100.f * eTime;
		}
		else {
			WingAngle = 50.f;
		}
	}
	else if (g_keyS) {
		if (WingAngle > -50.f || isEqual(WingAngle, -50.f)) {
			WingAngle -= 100.f * eTime;
		}
		else {
			WingAngle = -50.f;
		}
	}
}
