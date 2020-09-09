#include "SpaceProbe.h"
#include "Texture.h"

/* 
Modeling by P I H 
Copyright PIH, All rights are RESERVED 
*/

SpaceProbe::SpaceProbe()
{
}

SpaceProbe::~SpaceProbe()
{
}

void SpaceProbe::setPos(const myVector4f & pos)
{
	SpaceProbe::pos = pos;
}

myVector4f SpaceProbe::getPos() const
{
	return pos;
}

void SpaceProbe::setVel(const myVector4f & vel)
{
	SpaceProbe::vel = vel;
}

myVector4f SpaceProbe::getVel() const
{
	return vel;
}

void SpaceProbe::setAccel(const myVector4f & accel)
{
	SpaceProbe::accel = accel;
}

myVector4f SpaceProbe::getAccel() const
{
	return accel;
}

float SpaceProbe::getRadius() const
{
	return collideSphereRadius;
}

void SpaceProbe::update(float eTime, const Camera& camera)
{
	healthCheck();
	const float localSpeed{ 1000.f };
	sychronizeCamera(camera);
	rotate();
	vel = look * localSpeed;
}

void SpaceProbe::update(float eTime, const SpaceProbe & character)
{
	healthCheck();
	chaseCharacter(character);
	rotate();
	pos = pos + vel * eTime;
}

void SpaceProbe::rotate()
{
	if (isCameraLocked) { return; }
	float angleX{ acosf(dotproduct(myVector4f(look.x,0.f,look.z), look)) };
	float angleY{ acosf(dotproduct(myVector4f(look.x,0.f,look.z), worldAxisZ)) };
	if (look.y > 0.f) angleX = -angleX;
	if (look.x < 0.f) angleY = -angleY;
	pitch = ToDegree(angleX);
	yaw = ToDegree(angleY);
}

void SpaceProbe::sychronizeCamera(const Camera & camera)
{
	setLook(camera.getLook());
	pos = camera.getPos() + look * cameraDistance;
}

void SpaceProbe::setKinds(int kinds)
{
	SpaceProbe::kinds = kinds;
	generate();
}

int SpaceProbe::getKinds() const
{
	return kinds;
}

void SpaceProbe::setHealth(int health)
{
	SpaceProbe::health = health;
}

int SpaceProbe::getHealth() const
{
	return health;
}

void SpaceProbe::setLook(const myVector4f& look)
{
	SpaceProbe::look = normalize(look);
	right = crossproduct(look, worldAxisY);
	up = crossproduct(right, look);
}

myVector4f SpaceProbe::getLook() const
{
	return look;
}

void SpaceProbe::setCameraDistance(unsigned char input, float eTime)
{
	const float Constant = 10.f; /* 변환 상수 */
	switch (input)
	{
	case '+':
		SpaceProbe::cameraDistance += eTime * Constant;
		if (cameraDistance > MAX_DISTANCE) cameraDistance = MAX_DISTANCE;
		break;
	case '-':
		SpaceProbe::cameraDistance -= eTime * Constant;
		if (cameraDistance < DEFAULT_DISTANCE) cameraDistance = DEFAULT_DISTANCE;
		break;
	}
}

void SpaceProbe::render()
{
	if (!isAlive) return;

	glPushMatrix();
	glDisable(GL_TEXTURE_2D); //	비행기에 텍스쳐 매핑 안되게 텍스쳐 디스에이블
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(pos.x, pos.y, pos.z);
	switch (kinds) {
	case KIND_NONE:
		printf("ERROR[!] No Model is rendered\n");
		break;
	case KIND_CHARACTER:
		glPushMatrix();
		{
			{ glRotatef(yaw, 0.f, 1.f, 0.f); }
			glPushMatrix();
			{
				{ glRotatef(pitch, 1.f, 0.f, 0.f); }
				drawCharacterModel();
				drawCollideSphere();
			}
			glPopMatrix();
		}
		glPopMatrix();
		break;

	case KIND_ENEMY:
		drawEnemyModel();
		drawCollideSphere();
		break;

	default:
		break;
	}
	//drawCollideSphere();
	glEnable(GL_TEXTURE_2D); //		다시 텍스쳐 사용
	glPopMatrix();
}

void SpaceProbe::drawEnemyModel()
{
	//적 우주선
	glPushMatrix();
	{
		glScalef(1.3f, 1.3f, 1.3f); // 전체 크기 조정
		glPushMatrix(); //우주선 몸뚱이
		{
			glColor3f(0.5f, 0.5f, 0.5f);
			glScalef(2.f, 0.3f, 2.f);
			glutSolidSphere(10.0, 10, 5);
		}
		glPopMatrix();

		glPushMatrix(); //우주선 몸통 실선 1
		{
			glColor3f(1.f, 1.f, 0.3f);
			glRotatef(45.f, 0.f, 1.f, 0.f);
			glTranslatef(10.f, 0.f, 0.f);
			glScalef(3.f, 1.f, 0.2f);
			glutSolidSphere(3.0, 10, 5);
		}
		glPopMatrix();

		glPushMatrix(); //우주선 몸통 실선 2
		{
			glColor3f(1.f, 1.f, 0.3f);
			glRotatef(-45.f, 0.f, 1.f, 0.f);
			glTranslatef(10.f, 0.f, 0.f);
			glScalef(3.f, 1.f, 0.2f);
			glutSolidSphere(3.0, 10, 5);
		}
		glPopMatrix();

		glPushMatrix(); //우주선 몸통 실선 3
		{
			glColor3f(1.f, 1.f, 0.3f);
			glRotatef(-135.f, 0.f, 1.f, 0.f);
			glTranslatef(10.f, 0.f, 0.f);
			glScalef(3.f, 1.f, 0.2f);
			glutSolidSphere(3.0, 10, 5);
		}
		glPopMatrix();

		glPushMatrix(); //우주선 몸통 실선 4
		{
			glColor3f(1.f, 1.f, 0.3f);
			glRotatef(135.f, 0.f, 1.f, 0.f);
			glTranslatef(10.f, 0.f, 0.f);
			glScalef(3.f, 1.f, 0.2f);
			glutSolidSphere(3.0, 10, 5);
		}
		glPopMatrix();


		glPushMatrix(); //우주선 대가리
		{
			glColor3f(0.7f, 0.7f, 0.7f);
			glTranslatef(0.f, 3.f, 0.f);
			glScalef(1.3f, 1.f, 1.3f);
			glutSolidSphere(5.0, 10, 10);
		}
		glPopMatrix();

		glPushMatrix(); //우주선 아래 빨간 구
		{
			glColor3f(0.7f, 0.2f, 0.2f);
			glTranslatef(0.f, -3.f, 0.f);
			glScalef(1.3f, 1.f, 1.3f);
			glutSolidSphere(2.0, 10, 10);
		}
		glPopMatrix();

		glPushMatrix(); //우주선 다리 1
		{
			glColor3f(0.7f, 0.7f, 0.7f);
			glTranslatef(0.f, -3.f, -12.f);
			glRotatef(30.f, 1.f, 0.f, 0.f);
			glScalef(0.2f, 1.f, 0.2f);
			glutSolidCube(5.0);
		}
		glPopMatrix();

		glPushMatrix(); //우주선 다리 2
		{
			glColor3f(0.7f, 0.7f, 0.7f);
			glTranslatef(0.f, -3.f, 12.f);
			glRotatef(-30.f, 1.f, 0.f, 0.f);
			glScalef(0.2f, 1.f, 0.2f);
			glutSolidCube(5.0);
		}
		glPopMatrix();

		glPushMatrix(); //우주선 다리 3
		{
			glColor3f(0.7f, 0.7f, 0.7f);
			glTranslatef(-12.f, -3.f, 0.f);
			glRotatef(-30.f, 0.f, 0.f, 1.f);
			glScalef(0.2f, 1.f, 0.2f);
			glutSolidCube(5.0);
		}
		glPopMatrix();

		glPushMatrix(); //우주선 다리 4
		{
			glColor3f(0.7f, 0.7f, 0.7f);
			glTranslatef(12.f, -3.f, 0.f);
			glRotatef(30.f, 0.f, 0.f, 1.f);
			glScalef(0.2f, 1.f, 0.2f);
			glutSolidCube(5.0);
		}
		glPopMatrix();

	}
	glPopMatrix();
}

void SpaceProbe::drawCharacterModel()
{
	glPushMatrix(); //플레이어 우주선
	{
		glPushMatrix(); //플레이어 우주선
		{
			glScalef(0.4f, 0.4f, 0.4f); //전체 크기 조정

			glPushMatrix(); //우주선 몸뚱이
			{
				glColor3f(1.f, 1.f, 1.f);

				glutSolidCylinder(5.0, 20.0, 20, 20);
				glColor3f(0.f, 0.f, 0.f);
				glutWireCylinder(5.2, 19.9, 10, 5);
			}
			glPopMatrix();

			glPushMatrix(); //우주선 중간 대가리
			{
				glColor3f(1.f, 1.f, 1.f);

				glTranslatef(0.f, 0.f, 20.f);
				glutSolidCone(5.0, 7.0, 20, 20);
			}
			glPopMatrix();

			glPushMatrix(); //우주선 중간 대가리 날개 1
			{
				glColor3f(1.f, 1.f, 1.f);

				glTranslatef(0.f, 7.f, 19.f);
				glRotatef(-60.f, 1.f, 0.f, 0.f);
				glRotatef(WingAngle / 2.3, -1.f, 0.f, 0.f);
				glScalef(0.8f, 2.f, 0.2f);
				glutSolidCube(5.0);
				glColor3f(0.f, 0.f, 0.f);
				glutWireCube(5.0);
			}
			glPopMatrix();

			glPushMatrix(); //우주선 중간 대가리 날개 2
			{
				glColor3f(1.f, 1.f, 1.f);

				glTranslatef(0.f, -7.f, 19.f);
				glRotatef(60.f, 1.f, 0.f, 0.f);
				glRotatef(WingAngle / 2.3, 1.f, 0.f, 0.f);
				glScalef(0.8f, 2.f, 0.2f);
				glutSolidCube(5.0);
				glColor3f(0.f, 0.f, 0.f);
				glutWireCube(5.0);
			}
			glPopMatrix();

			glPushMatrix(); //우주선 중간 대가리 날개 3
			{
				glColor3f(1.f, 1.f, 1.f);

				glTranslatef(-7.f, 0.f, 19.f);
				glRotatef(-60.f, 0.f, 1.f, 0.f);
				glRotatef(WingAngle / 2.3, 0.f, -1.f, 0.f);
				glScalef(2.f, 0.8f, 0.2f);
				glutSolidCube(5.0);
				glColor3f(0.f, 0.f, 0.f);
				glutWireCube(5.0);
			}
			glPopMatrix();

			glPushMatrix(); //우주선 중간 대가리 날개 4
			{
				glColor3f(1.f, 1.f, 1.f);

				glTranslatef(7.f, 0.f, 19.f);
				glRotatef(60.f, 0.f, 1.f, 0.f);
				glRotatef(WingAngle / 2.3, 0.f, 1.f, 0.f);
				glScalef(2.f, 0.8f, 0.2f);
				glutSolidCube(5.0);
				glColor3f(0.f, 0.f, 0.f);
				glutWireCube(5.0);
			}
			glPopMatrix();

			glPushMatrix(); //우주선 맨 앞 대가리
			{
				glColor3f(1.f, 1.f, 1.f);

				glTranslatef(0.f, 0.f, 24);
				glutSolidCylinder(2.0, 3.0, 20, 20);
				glColor3f(0.f, 0.f, 0.f);
				glutWireCylinder(2.0, 3.0, 5, 5);
			}
			glPopMatrix();

			glPushMatrix(); //우주선 뒷 날개 1
			{
				glColor3f(1.f, 1.f, 1.f);

				glTranslatef(-11.f, 12.f, 1.f);
				glRotatef(45.f, 0.f, 0.f, 1.f);
				glRotatef(WingAngle, -1.f, 0.f, 0.f);
				glScalef(1.3f, 3.3f, 0.2f);
				glutSolidCube(5.0);
				glColor3f(0.f, 0.f, 0.f);

				glutWireCube(5.0);
			}
			glPopMatrix();

			glPushMatrix(); //우주선 뒷 날개 2
			{
				glColor3f(1.f, 1.f, 1.f);

				glTranslatef(11.f, 12.f, 1.f);
				glRotatef(-45.f, 0.f, 0.f, 1.f);
				glRotatef(WingAngle, -1.f, 0.f, 0.f);
				glScalef(1.3f, 3.3f, 0.2f);
				glutSolidCube(5.0);
				glColor3f(0.f, 0.f, 0.f);
				glutWireCube(5.0);
			}
			glPopMatrix();

			glPushMatrix(); //우주선 뒷 날개 3
			{
				glColor3f(1, 1, 1);

				glTranslatef(-11.f, -12.f, 1.f);
				glRotatef(135.f, 0.f, 0.f, 1.f);
				glRotatef(WingAngle, -1.f, 0.f, 0.f);
				glScalef(1.3f, 3.3f, 0.2f);
				glutSolidCube(5.0);
				glColor3f(0.f, 0.f, 0.f);
				glutWireCube(5.0);
			}
			glPopMatrix();

			glPushMatrix(); //우주선 뒷 날개 4
			{
				glColor3f(1.f, 1.f, 1.f);

				glTranslatef(11.f, -12.f, 1.f);
				glRotatef(-135.f, 0.f, 0.f, 1.f);
				glRotatef(WingAngle, -1.f, 0.f, 0.f);
				glScalef(1.3f, 3.3f, 0.2f);
				glutSolidCube(5.0);
				glColor3f(0.f, 0.f, 0.f);
				glutWireCube(5.0);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void SpaceProbe::generate()
{
	switch (kinds)
	{
	case KIND_CHARACTER:
		collideSphereRadius = DEFAULT_SIZE;
		cameraDistance = DEFAULT_DISTANCE;
		health = MAX_HEALTH;
		isAlive = true;
		break;
	case KIND_ENEMY:
		collideSphereRadius = DEFAULT_ENEMY_SIZE;
		health = MAX_ENEMY_HEALTH;
		isAlive = true;
		break;
	default:
		printf("ERROR[!] SpaceProbe setting error\n");
		break;
	}
}

void SpaceProbe::destroy()
{
	isAlive = false;
	vel = zeroVector;
	accel = zeroVector;
}

void SpaceProbe::healthCheck()
{
	if (health <= 0) {
		destroy();
	}
}

void SpaceProbe::chaseCharacter(const SpaceProbe & character)
{
	float LeastLength = 500.f;
	float Distance = getDistance(character.getPos(), pos);
	if (LeastLength < Distance || isEqual(LeastLength, Distance)) {
		const float localSpeed{ 1000.f };
		look = normalize(character.getPos() - pos);
		right = crossproduct(look, worldAxisY);
		up = crossproduct(right, look);
		vel = look * localSpeed;
		isChasing = true;
	}
	else {
		isChasing = false;
		vel = zeroVector;
	}
}

void SpaceProbe::drawCollideSphere()
{
	if (CollideSphereFlag) {
		glColor3f(1.f, 0.f, 0.f);
		glutWireSphere(static_cast<double>(collideSphereRadius), SpaceProbe::SLICES, SpaceProbe::STACKS);
		glColor3f(1.f, 1.f, 1.f);
	}
}

void SpaceProbe::setWingAngle(float angle) {
	WingAngle = angle;
}