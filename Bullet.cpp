#include "Bullet.h"

const myVector4f Diffuse{ 1.f,1.f,1.f }; /* Bullet과 Particle에만 적용시킬 물체 디퓨즈 속성입니다. */

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

myVector4f Bullet::getPos() const
{
	return pos;
}

void Bullet::setPos(const myVector4f & pos)
{
	Bullet::pos = pos;
}

myVector4f Bullet::getVel() const
{
	return vel;
}

void Bullet::setVel(const myVector4f & vel)
{
	Bullet::vel = vel;
}

float Bullet::getRadius() const
{
	return radius;
}

void Bullet::render()
{
	if (isAlive) {
		glPushMatrix();
		{
			glDisable(GL_LIGHTING);	/* 총알은 광원을 받지 않는다. */
			glColor3fv(color.m);
			glTranslatef(pos.x, pos.y, pos.z);
			glutSolidSphere(radius, Bullet::SLICES, Bullet::STACKS);
			glEnable(GL_LIGHTING);
			drawCollideSphere();
		}
		glPopMatrix();
	}
}

void Bullet::update(float eTime)
{
	checkWorldOut();
	pos = pos + vel * eTime;
}

void Bullet::checkWorldOut()
{
	if (30000.f < getDistance(zeroVector,pos)) {
		destroy();
	}
}

void Bullet::generate(const SpaceProbe& character)
{
	float speed{ 500.f }; /* 총알 고유 속도 */
	myVector4f cLook = character.getLook();
	isAlive = true;
	if (character.getKinds() == KIND_CHARACTER) {
		color = myVector4f(0.7f, 1.f, 0.2f);
	}
	else if (character.getKinds() == KIND_ENEMY) {
		color = myVector4f(0.6f, 0.f, 0.f);
	}
	pos = character.getPos() + character.getLook() * 15.f;
	vel = character.getLook() * speed + character.getVel();
}

void Bullet::destroy()
{
	isAlive = false;
	vel = zeroVector;
	pos = zeroVector;
}

void Bullet::drawCollideSphere()
{
	if (CollideSphereFlag) {
		glColor3f(1.f, 0.f, 0.f);
		glutWireSphere(collideSphereRadius, Bullet::SLICES, Bullet::STACKS);
		glColor3f(1.f, 1.f, 1.f);
	}
}

void Bullet::makeParticle(Particle * pList)
{
	for (int i = 0; i < MAX_PARTICLE; ++i) {
		pList[i].generate(pos);
	}
	destroy();
}

Particle::Particle()
{
}

Particle::~Particle()
{
}

myVector4f Particle::getPos() const
{
	return pos;
}

void Particle::setPos(const myVector4f & pos)
{
	Particle::pos = pos;
}

myVector4f Particle::getVel() const
{
	return vel;
}

void Particle::setVel(const myVector4f & vel)
{
	Particle::vel = vel;
}

void Particle::drawCollideSphere()
{
	if (CollideSphereFlag) {
		glColor3f(1.f, 0.f, 0.f);
		glutWireSphere(collideSphereRadius, Particle::SLICES, Particle::STACKS);
		glColor3f(1.f, 1.f, 1.f);
	}
}

void Particle::render()
{
	if (isAlive) {
		glPushMatrix();
		{
			glDisable(GL_LIGHTING);	/* 총알은 광원을 받지 않는다. */
			glColor3fv(color.m);
			glTranslatef(pos.x, pos.y, pos.z);
			glutSolidSphere(radius, Particle::SLICES, Particle::STACKS);
			glEnable(GL_LIGHTING);
			drawCollideSphere();
		}
		glPopMatrix();
	}
}

void Particle::update(float eTime)
{
	checkLifetime();
	if (isAlive) {
		pos = pos + vel * eTime;
		lifetime -= eTime;
	}
}

void Particle::checkLifetime()
{
	if (isZero(lifetime) || lifetime < 0) {
		destroy();
	}
}

void Particle::generate(const myVector4f& pos)
{
	isAlive = true;
	lifetime = LIFETIME_FULL;
	const float speed = 100.f;
	Particle::pos = pos;
	color = myVector4f(1.f, 1.f, 0.25f);
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<float> urdDirection(-1.f, 1.f);
	vel = normalize(myVector4f(urdDirection(dre), urdDirection(dre), urdDirection(dre))) * speed;
}

void Particle::generate(const SpaceProbe & s)
{
	isAlive = true;
	lifetime = LIFETIME_FULL;
	const float speed = 100.f;
	Particle::pos = s.getPos();
	if (s.getKinds() == KIND_ENEMY) {
		color = myVector4f(1.f, 0.25f, 0.25f);
	}
	else if (s.getKinds() == KIND_CHARACTER) {
		color = myVector4f(0.25f, 0.4f, 1.f);
	}
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<float> urdDirection(-1.f, 1.f);
	vel = normalize(myVector4f(urdDirection(dre), urdDirection(dre), urdDirection(dre))) * speed;
}

void Particle::destroy()
{
	isAlive = false;
	pos = zeroVector;
	vel = zeroVector;
}
