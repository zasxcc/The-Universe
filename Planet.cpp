#include "LibraryHeader.h"
#include "Planet.h"
#include "Texture.h"

/* ������ �������� �ȹٷ� ���ִ�.(=������ �������� õ���� ������ ���Ͽ� �����̴�.)
��ó https://m.blog.naver.com/iq_up/100172130277
*/

const myVector4f SpaceColor{ 0.4f,0.4f,0.4f}; /* Space���� �����ų ��ü ��ǻ�� �Ӽ��Դϴ�. */
float angle_rotateRing{ 0.f };

Planet::Planet(int kind)
{
	kinds = kind;
	/* ������ �˵��� ��ġ�ϰ�, �˵��󿡼� ���� ������ �մϴ�. */
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<float> urdDirection(-1.f, 1.f);

	switch (kinds)
	{
	case NAME::NONE:
		printf("ERROR[!] Planet::NONE type initiation \n");
		break;

	case NAME::SUN:
		radius = 1100.f;
		centre = myVector4f(0.f, 0.f, 0.f);		//	���� �Ÿ�
		look = worldAxisZ;
		up = worldAxisY;
		right = worldAxisX;
		collideSphereRadius = radius;
		gravityFieldRadius = 29999.f;
		angle_rotationAxis = 0.f;
		speed_rotate = 0.0f;
		printf("Planet::SUN type initiation \n");
		break;

	case NAME::MERCURY:
		radius = 51.f;
		centre = normalize(myVector4f(urdDirection(dre), 0.f, urdDirection(dre))) * 3500.f;
		look = normalize(zeroVector - centre);
		right = crossproduct(look, worldAxisY);	/* �̹� ����ȭ �Ǿ����� */
		up = crossproduct(right, look);			/* ���������� ����ȭ �Ǿ����� */
		collideSphereRadius = radius;
		gravityFieldRadius = 8.f * radius;
		angle_rotationAxis = 0.03f;
		speed_rotate = 0.3f;
		printf("Planet::MERCURY type initiation \n");
		break;

	case NAME::VENUS:
		radius = 153.f;
		centre = normalize(myVector4f(urdDirection(dre), 0.f, urdDirection(dre))) * 5200.f;
		look = normalize(zeroVector - centre);
		right = crossproduct(look, worldAxisY);	/* �̹� ����ȭ �Ǿ����� */
		up = crossproduct(right, look);			/* ���������� ����ȭ �Ǿ����� */
		collideSphereRadius = radius;
		gravityFieldRadius = 8.f * radius;
		angle_rotationAxis = 177.4f;
		speed_rotate = 0.7f;
		printf("Planet::VENUS type initiation \n");
		break;
		
	case NAME::EARTH:
		radius = 170.f;
		centre = normalize(myVector4f(urdDirection(dre), 0.f, urdDirection(dre))) * 7200.f;
		look = normalize(zeroVector - centre);
		right = crossproduct(look, worldAxisY);	/* �̹� ����ȭ �Ǿ����� */
		up = crossproduct(right, look);			/* ���������� ����ȭ �Ǿ����� */
		collideSphereRadius = radius;
		gravityFieldRadius = 8.f * radius;
		angle_rotationAxis = 23.44f;
		speed_rotate = 1.f;
		printf("Planet::EARTH type initiation \n");
		break;

	case NAME::EARTH_CLOUD:
		radius = 171.f;
		centre = myVector4f(0.f, 0.f, 0.f);		/* ������ ��ġ�� ���� ��. */
		look = normalize(zeroVector - centre);
		right = crossproduct(look, worldAxisY);	/* �̹� ����ȭ �Ǿ����� */
		up = crossproduct(right, look);			/* ���������� ����ȭ �Ǿ����� */
		angle_rotationAxis = 23.44f;
		speed_rotate = 3.7f;
		printf("Planet::EARTH CLOUD type initiation \n");
		break;

	case NAME::MARS:
		radius = 155.f;
		centre = normalize(myVector4f(urdDirection(dre), 0.f, urdDirection(dre))) * 9700.f;
		look = normalize(zeroVector - centre);
		right = crossproduct(look, worldAxisY);	/* �̹� ����ȭ �Ǿ����� */
		up = crossproduct(right, look);			/* ���������� ����ȭ �Ǿ����� */
		collideSphereRadius = radius;
		gravityFieldRadius = 8.f * radius;
		angle_rotationAxis = 25.1f;
		speed_rotate = 1.f;
		printf("Planet::MARS type initiation \n");
		break;

	case NAME::JUPITER:
		radius = 500.f;
		centre = normalize(myVector4f(urdDirection(dre), 0.f, urdDirection(dre))) * 13300.f;
		look = normalize(zeroVector - centre);
		right = crossproduct(look, worldAxisY);	/* �̹� ����ȭ �Ǿ����� */
		up = crossproduct(right, look);			/* ���������� ����ȭ �Ǿ����� */
		collideSphereRadius = radius;
		gravityFieldRadius = 8.f * radius;
		angle_rotationAxis = 3.13f;
		speed_rotate = 2.66f;
		printf("Planet::JUPITER type initiation \n");
		break;

	case NAME::SATURN:
		radius = 400.f;
		centre = normalize(myVector4f(urdDirection(dre), 0.f, urdDirection(dre))) * 18000.f;
		look = normalize(zeroVector - centre);
		right = crossproduct(look, worldAxisY);	/* �̹� ����ȭ �Ǿ����� */
		up = crossproduct(right, look);			/* ���������� ����ȭ �Ǿ����� */
		collideSphereRadius = radius;
		gravityFieldRadius = 8.f * radius;
		angle_rotationAxis = 26.73f;
		speed_rotate = 2.18f;
		printf("Planet::SATURN type initiation \n");
		break;

	case NAME::URANUS:
		radius = 230.f;
		centre = normalize(myVector4f(urdDirection(dre), 0.f, urdDirection(dre))) * 22500.f;
		look = normalize(zeroVector - centre);
		right = crossproduct(look, worldAxisY);	/* �̹� ����ȭ �Ǿ����� */
		up = crossproduct(right, look);			/* ���������� ����ȭ �Ǿ����� */
		collideSphereRadius = radius;
		gravityFieldRadius = 8.f * radius;
		angle_rotationAxis = 97.77f;
		speed_rotate = 1.41f;
		printf("Planet::URANUS type initiation \n");
		break;

	case NAME::NEPTUNE:
		radius = 220.f;
		centre = normalize(myVector4f(urdDirection(dre), 0.f, urdDirection(dre))) * 28000.f;
		look = normalize(zeroVector - centre);
		right = crossproduct(look, worldAxisY);	/* �̹� ����ȭ �Ǿ����� */
		up = crossproduct(right, look);			/* ���������� ����ȭ �Ǿ����� */
		collideSphereRadius = radius;
		gravityFieldRadius = 8.f * radius;
		angle_rotationAxis = 28.32f;
		speed_rotate = 1.5f;
		printf("Planet::NEPTUNE type initiation \n");
		break;

	case NAME::MOON:
		radius = 40.f;
		centre = myVector4f(500.f, 0.f, 0.f);
		look = normalize(zeroVector - centre);
		right = crossproduct(look, worldAxisY);	/* �̹� ����ȭ �Ǿ����� */
		up = crossproduct(right, look);			/* ���������� ����ȭ �Ǿ����� */		
		collideSphereRadius = radius;
		gravityFieldRadius = 8.f * radius;
		angle_rotationAxis = 6.f;
		printf("Planet::MOON type initiation \n");
		break;

	case NAME::SPACE:
		radius = 30000.f;
		centre = myVector4f(0.f, 0.f, 0.f);
		printf("Planet::SPACE type initiation \n");
		break;

	default:
		printf("Planet::Noting initiated\n");
		break;
	}
}

Planet::~Planet()
{
}

void Planet::setCentre(const myVector4f pos)
{
	centre = pos;
}

myVector4f Planet::getCentre() const
{
	return centre;
}

void Planet::setVel(const myVector4f vel)
{
	Planet::vel = vel;
}

myVector4f Planet::getVel() const
{
	return vel;
}

void Planet::setAccel(const myVector4f accel)
{
	Planet::accel = accel;
}

myVector4f Planet::getAccel() const
{
	return accel;
}

int Planet::getKinds() const
{
	return kinds;
}

float Planet::getRadius() const
{
	return collideSphereRadius;
}

float Planet::getGavityRadius() const
{
	return gravityFieldRadius;
}

void Planet::drawCollideSphere()
{
	if (CollideSphereFlag) {
		glColor3f(1.f, 0.f, 0.f);
		glutWireSphere(collideSphereRadius, Planet::SLICES, Planet::STACKS);
		glColor3f(0.f, 0.f, 1.f);
		glutWireSphere(gravityFieldRadius, Planet::SLICES, Planet::STACKS);
		glColor3f(1.f, 1.f, 1.f);
	}
}

void Planet::rotateFaceEarth()
{
	float angleY{ acosf(dotproduct(myVector4f(look.x,0.f,look.z), - worldAxisX)) };
	if (look.z < 0.f) angleY = -angleY;
	angle_rotate = ToDegree(angleY);
}

void Planet::render(void)
{
	glColor3f(1, 1, 1);
	if (kinds == EARTH_CLOUD) {			/* ���� ���ĺ��� ���� */
		glColor4f(1.f, 1.f, 1.f, 0.5f);
	}
	if (kinds == SPACE) {
		glDisable(GL_COLOR_MATERIAL);	/* ���� ��ü���� ��ǻ�� ���� */
		glMaterialfv(GL_FRONT, GL_DIFFUSE, SpaceColor.m);
	}
	else {
		glEnable(GL_COLOR_MATERIAL);
	}
	glPushMatrix();
	{
		glTranslatef(centre.x, centre.y, centre.z);		/* �� ��ġ */
		glRotatef(angle_rotationAxis, 0.f, 0.f, -1.f);	/* �༺ ������ ���� */
		switch (kinds)			/* ���� ���� �༺�� */
		{
		case NAME::SATURN:
		case NAME::URANUS:
		case NAME::NEPTUNE:
			drawRing();
			break;
		}
		glPushMatrix();
		{
			glRotatef(angle_rotate, 0.f, 1.f, 0.f);			/* ���� ���� */
			if (kinds == NAME::MOON) {
				glRotatef(180.f, 0.f, 1.f, 0.f);			/* ��ó�� �������� ���� �ո��� ������ �ٶ�����Ѵ�. */
			}
			MakeSphere(0, 0, 0, radius, 37);
		}
		glPopMatrix();
		drawCollideSphere();
	}
	glPopMatrix();
	glEnable(GL_COLOR_MATERIAL);
}

void Planet::update(float eTime)
{
	float localSpeed = 10.f;
	if (isEqual(angle_rotate, 360.f)) angle_rotate = 0.f;
	if (isEqual(angle_rotateRing, 360.f)) angle_rotateRing = 0.f;

	/* ������ ��쿡�� ȸ���� �����ϰ� ������Ʈ�� �������� �ʴ´�. */
	if (kinds == NAME::EARTH_CLOUD) { angle_rotate += speed_rotate * eTime * localSpeed; return; }

	/* ���� ��� */
	look = normalize(zeroVector - centre);
	right = crossproduct(look, worldAxisY);
	up = crossproduct(right, look);

	/* ���� ��쿡 �ո��� �׻� ������ ���Ѵ�. */
	if (kinds == NAME::MOON) {
		rotateFaceEarth();
	}

	/* ���� ���� ���� */
	angle_rotate += speed_rotate * eTime * localSpeed;

	/* �� ���� ���� ����*/
	if (kinds == NAME::SATURN) angle_rotateRing += eTime;			/* ���������� �伺�� ���� ����ó�� �ϵ���. */

	/* �ӵ� ��� ���� �� �̵� */
	vel = right * speed_move;
	centre = centre + vel * eTime;
}

void Planet::drawRing()
{
	glPushMatrix();
	glRotatef(angle_rotateRing, 0.f, 1.f, 0.f);
	glDisable(GL_CULL_FACE);
	if (kinds == SATURN) {			/* �伺 �� */
		glPushMatrix();
		{
			GLUquadricObj * qobj;
			qobj = gluNewQuadric();
			glScalef(780.f, 0.1f, 780.f);
			glRotatef(90.f, -1.f, 0.f, 0.f);
			glColor4f(1.f, 1.f, 1.f, 0.7f);
			gluDisk(qobj, 0.79, 0.8, 50, 5);
			glColor4f(1.f, 1.f, 0.85f, 0.7f);
			gluDisk(qobj, 0.805, 1.00, 50, 5);
			glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
			gluDisk(qobj, 1.05, 1.145, 50, 5);
			gluDisk(qobj, 1.155, 1.165, 50, 5);
		}
		glPopMatrix();
	}
	else if (kinds == URANUS) {		/* �ؿռ� �� */
		glPushMatrix();
		{
			GLUquadricObj * qobj;
			qobj = gluNewQuadric();
			glScalef(680.f, 0.1f, 680.f);
			glRotatef(90.f, -1.f, 0.f, 0.f);
			glColor4f(1.f, 1.f, 1.f, 0.2f);
			gluDisk(qobj, 0.88, 0.885, 50, 5);
			glColor4f(0.8f, 0.8f, 1.f, 0.7f);
			gluDisk(qobj, 0.89, 0.9, 50, 5);
		}
		glPopMatrix();
	}
	else if (kinds == NEPTUNE) {
		glPushMatrix();
		{
			GLUquadricObj * qobj;
			qobj = gluNewQuadric();
			glScalef(680.f, 0.1f, 680.f);
			glRotatef(90.f, -1.f, 0.f, 0.f);
			glColor4f(0.5f, 0.5f, 1.f, 0.3f);
			gluDisk(qobj, 0.38, 0.4, 50, 5);
			glColor4f(0.8f, 0.8f, 1.f, 0.7f);
			gluDisk(qobj, 0.89, 0.9, 50, 5);
		}
		glPopMatrix();
	}
	glColor3f(1.f, 1.f, 1.f);
	glEnable(GL_CULL_FACE);
	glPopMatrix();
}

void Planet::MakeSphere(float x, float y, float z, float r, int n)
{
	float theta1, theta2, theta3;
	float ex, ey, ez, px, py, pz;

	if (r < 0)
		r = -r;
	if (n < 0)
		n = -n;

	if (n < 4 || r <= 0)
	{
		glBegin(GL_POINTS);
		glVertex3f(x, y, z);
		glEnd();
		return;
	}

	for (int j = 0; j < n; ++j)
	{
		theta1 = j * 2.f * PI / n - HALF_PI;
		theta2 = (j + 1) * 2.f * PI / n - HALF_PI;

		glBegin(GL_QUAD_STRIP);
		for (int i = 0; i <= n; ++i)
		{
			theta3 = i * 2.f * PI / n;

			ex = cosf(theta2) * cosf(theta3);
			ey = sinf(theta2);
			ez = cosf(theta2) * sinf(theta3);
			px = x + r * ex;
			py = y + r * ey;
			pz = z + r * ez;

			glNormal3f(ex, ey, ez);
			if (kinds == NAME::SUN || kinds == NAME::SPACE) {
				/* �¾�� ������ ���ʿ� �������� �����ϴϱ�, ����� ��������� ���� �ް� ��. */
				glNormal3f(-ex, -ey, -ez);
			}
			glTexCoord2f(static_cast<float>(i) / static_cast<float>(n), static_cast<float>(2 * (j + 1)) / static_cast<float>(n));
			glVertex3f(px, py, pz);

			ex = cosf(theta1) * cosf(theta3);
			ey = sinf(theta1);
			ez = cosf(theta1) * sinf(theta3);
			px = x + r * ex;
			py = y + r * ey;
			pz = z + r * ez;

			glNormal3f(ex, ey, ez);
			if (kinds == NAME::SUN || kinds == NAME::SPACE) {
				/* �¾�� ������ ���ʿ� �������� �����ϴϱ�, ����� ��������� ���� �ް� ��. */
				glNormal3f(-ex, -ey, -ez);
			}
			glTexCoord2f(static_cast<float>(i) / static_cast<float>(n), static_cast<float>(2 * j) / static_cast<float>(n));
			glVertex3f(px, py, pz);

		}
		glEnd();
	}
}

