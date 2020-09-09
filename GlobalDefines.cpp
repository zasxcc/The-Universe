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


Sound g_sound;		//	배경음 재생 위한 글로벌 사운드
Light g_light;		//	빛 설정.
Camera g_camera;	//	카메라
/* 글로벌 키 부울린 */
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

/* 우주선들 */
SpaceProbe spaceProbe[MAX_PROBES];			//	0번은 플레이어, 나머지는 적
float WingAngle = 0;
/* 총알 */
Bullet bullet[MAX_BULLET];
int g_bulletCurrIDX{ 0 };
float g_BLT_passedTime{ 0.f };				//	총알 생성 간격 측정용 시간변수

Bullet enemybullet[MAX_BULLET];
int g_bulletEnemyCurrIDX{ 0 };
float g_ENEMYBLT_passedTime[MAX_PROBES - 1]{ 0.f };	//	적개체 총알 생성 간격 측정용 시간변수

/* 파티클 */
Particle g_particleDestory[MAX_PARTICLE];	//	SpaceProbe 파괴시 파티클
Particle g_particleCollide[MAX_PARTICLE];	//	Planet에 Bullet 충돌시 파티클

///////////////// 행성들 선언
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
	/* key 입력을 통해서 카메라를 바꿀 경우에 Reshape을 호출해줘야함 - 투영변환을 새롭게 하려고 */
	g_camera.ModifyProjectionMode(width, height);
}

void CB_renderScene(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	float eTime = calculateElapseTime();
	//printf("%f\n", eTime);	//*********************** TEST
	
	/* 렌더 이전에 업데이트 함수를 호출합니다. 각 처리를 프레임 시간 기준으로 처리합니다. */

	Update(eTime);
	
	/* 이 아래로 객체들을 그려넣어주세요. */

	g_light.setLight();

	drawBullet();
	
	drawParticle();

	drawSpaceship();
	
	/* 행성계 */
	drawWorld();

	glutSwapBuffers();	//	swap function of double buffering 
}

void Update(float eTime)
{
	/* 카메라 key input */
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

	/* 여기에 객체 업데이트를 작성합니다. ################# */

	/* 총알 */
	updateBullet(eTime);

	/* 파티클 */
	updateParticle(eTime);

	/* 주인공 */
	/* 날개 애니메이션 */
	WingSet(eTime);
	WingRecover(eTime);
	spaceProbe[IDX_CHARACTER].setWingAngle(WingAngle);
	spaceProbe[IDX_CHARACTER].update(eTime, g_camera);
	regenerateCharacter(spaceProbe[IDX_CHARACTER], eTime);
	if (g_keySpace && spaceProbe[IDX_CHARACTER].isAlive) {
		/* 0.3초마다 1개 생성하도록 할 예정. */
		if (g_BLT_passedTime > 0.1f) { /* 사격 시에 0.1초의 딜레이 시간을 가진다. */
			g_BLT_passedTime = 0.f;
			bullet[g_bulletCurrIDX].generate(spaceProbe[IDX_CHARACTER]);
			if (g_bulletCurrIDX == MAX_BULLET - 1) g_bulletCurrIDX = 0;
			g_bulletCurrIDX++;
		}
		else {
			g_BLT_passedTime += eTime;
		}
	}

	/* 적 */
	if (!isPaused) {
		for (int i = 1; i < MAX_PROBES; ++i) {
			spawnEnemy(spaceProbe, eTime);
			spaceProbe[i].update(eTime, spaceProbe[IDX_CHARACTER]);
			if (!spaceProbe[i].isChasing && spaceProbe[i].isAlive) {
				if (g_ENEMYBLT_passedTime[i - 1] > 5.f) { /* 사격 시에 5초의 딜레이 시간을 가진다. */
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

	/* 행성 업데이트 */
	updateWorld(eTime);
}

void updateWorld(float eTime)
{
	/* 행성 업데이트 */
	{
		if (!isPaused) { /* 일시정지 */
			mercury.update(eTime);
			venus.update(eTime);
			earth.update(eTime);
			mars.update(eTime);
			jupiter.update(eTime);
			saturn.update(eTime);
			uranus.update(eTime);
			neptune.update(eTime);
		}
		/* 시연을 위해 구름과 달의 공전은 일시정지를 걸지 않는다. */
		earthCloud.update(eTime);
		moon.update(eTime);
	}
	/* 주인공과 행성 중력장 간 충돌검사 */
	{
		if (isGravityON) { /* 중력 연산 ON 일때만 */
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
	/* 모든 우주선과 행성 간 직접충돌 검사 */
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
	/* 플레이어 총알과 행성 간 직접충돌 검사 */
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
	/* 플레이어 총알과 적 우주선과의 직접충돌 검사 */
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
	/* 적 총알과 플레이어의 충돌 검사 */
	{
		if (isCheatOn) { return; } /* 치트모드에서는 연산하지 않도록 한다. */
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
	/* 여기에 초기화 구문을 작성합니다. */
	InitiateTexture(g_textureID);

	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<float> urdDirection(-1.f, 1.f); //	우주선 위치벡터 생성용
	std::uniform_int_distribution<int> uidDistance(27000, 30000);

	/* 주인공 지구궤도 상 일정 거리 위치에 랜덤 생성 */
	{
		myVector4f direction{ urdDirection(dre), 0.f, urdDirection(dre) };
		spaceProbe[IDX_CHARACTER].setKinds(KIND_CHARACTER);
		g_camera.setPos(normalize(direction) * 8500.f);
		printf("The Character is deployed\n");
	}

	/* 적 우주선 초기 위치 생성 */
	for (int i = 1; i < MAX_PROBES; ++i)
	{
		myVector4f direction{ urdDirection(dre), urdDirection(dre), urdDirection(dre) };
		int distance = uidDistance(dre);
		spaceProbe[i].setKinds(KIND_ENEMY);
		spaceProbe[i].setPos(normalize(direction) * distance);
	}
	printf("All Enemy is Generated\n");

	/* 제일 마지막에 사운드를 실행합니다. */
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
	/* 플레이어 우주선 */
	glPushMatrix();
	{
		spaceProbe[0].render();
	}
	glPopMatrix();

	/* 적 우주선 */
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
	//	커서를 숨깁니다.
	ShowCursor(FALSE);
	/* setCursorPos() 함수값은 바탕화면의 LTRB 좌표계라서 파라미터 x,y 를 안쓰고 GetCursorPos를 사용 */

	float dxConstant = 0.5f; /* 가로축 회전상수, 회전의 정도, 1에 근사할 수록 더 많이 돌아감 */
	float dyConstant = 0.2f; /* 세로축 회전상수, 회전의 정도, 1에 근사할 수록 더 많이 돌아감 */

	POINT mouse;
	GetCursorPos(&mouse);
	int dx = g_prevX - mouse.x, dy = g_prevY - mouse.y;
	g_prevX = CENTRE_MONITOR_WIDTH, g_prevY = CENTRE_MONITOR_HEIGHT;

	if (dx == 0 && dy == 0) {
		/* 같다면, SetCursorPos() 함수를 재호출하지 않기 위해서 return. SetCursorPos 함수도 
		마우스 이벤트를 발생시켜 윈도우 큐에 들어가 함수가 계속 반복되는 불상사가 발생했었음. */
		return;
	}
	else {
		g_camera.Rotate(static_cast<float>(dy) * dyConstant, static_cast<float>(dx) * dxConstant, 0.f); /* rolling은 없습니다. */
		CB_renderScene();
		SetCursorPos(CENTRE_MONITOR_WIDTH, CENTRE_MONITOR_HEIGHT);
		/* 화면 회전까지 적용한 후, 마우스 포지션을 재정렬한다. */
	}
}

void CB_keyDownFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
		/* 프로그램 커맨드 */
		/* Arrow Button은 Special Key로 해야합니다. */
	case VK_ESCAPE:	//	ESC
		PostQuitMessage(0);
	case VK_RETURN:	//	Enter
		break;
	case VK_BACK:	//	Back space
		break;
	case VK_SPACE:	//	Space bar - 총알 발사
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

	case 'p':	/* 애니메이션 일시정지 */
	case 'P':
		isPaused = !isPaused;
		break;

	case '8':	/* 중력을 적용하지 않습니다. */
		isGravityON = !isGravityON;
		break;

	case '9' :	/* 치트모드 속도를 적용합니다. */
		isCheatOn = !isCheatOn;
		if (isCheatOn) {
			g_camera.CameraKeyMoving('c', 0.f);
		}
		else {
			g_camera.CameraKeyMoving('n', 0.f);
		}
		break;

	case '0':	/* 충돌박스를 보여줍니다. */
		ViewCollideBox();
		break;

	case 'l':	/* 카메라 Locking */
	case 'L':	
		spaceProbe[IDX_CHARACTER].isCameraLocked = !spaceProbe[IDX_CHARACTER].isCameraLocked;
		break;

	case '+':	/* 카메라 줌 인,아웃 */
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
		case GLUT_DOWN:	/* 마우스 왼쪽 눌림 */
			break;
		case GLUT_UP:	/* 마우스 왼쪽 떼임 */
			break;
		default:
			printf("MouseMove:LB Nothing\n");
			break;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		switch (state)
		{
		case GLUT_DOWN:	/* 마우스 오른쪽 눌림 */
			break;
		case GLUT_UP:	/* 마우스 오른쪽 떼임 */
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
	/* 모든 객체들의 공통된 스피어 렌더 플래그를 활성 / 비활성화 시킵니다. */
	for (int i = 0; i < MAX_BULLET; ++i) {
		/* 총알 */
		bullet[i].CollideSphereFlag = !bullet[i].CollideSphereFlag;
	}
	{
		/* 행성 */
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
		/* 우주선들 */
		spaceProbe[0].CollideSphereFlag = !spaceProbe[0].CollideSphereFlag;
		for (int i = 1; i < MAX_PROBES; ++i) {
			spaceProbe[i].CollideSphereFlag = !spaceProbe[i].CollideSphereFlag;
		}
	}
}

bool GravityCheck(const Planet& p, const SpaceProbe& s)	//	중력장 안에 있는가?
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

bool CollideCheck(const Planet& p, const SpaceProbe& s)			// 행성 우주선 충돌
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

bool CollideCheck(const Bullet& b, const Planet& p)					// 총알 행성 충돌
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

bool CollideCheck(const Bullet& b, const SpaceProbe& s)			// 총알 우주선 충돌
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
			std::uniform_real_distribution<float> urdDirection(-1.f, 1.f); //	우주선 위치벡터 생성용
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
