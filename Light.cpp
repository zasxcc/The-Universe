#include "Light.h"

Light::Light()
{
	pos.w = 1.f; /* 점광원입니다. */
	ambient = zeroVector;
	diffuse = { 1.f,1.f,1.f };
	specular = { 1.f,1.f,1.f };
}

Light::~Light()
{
}

void Light::setLightPos(const myVector4f pos)
{
	Light::pos = pos;
}

void Light::setLight()
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient.m);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse.m);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular.m);
	glLightfv(GL_LIGHT0, GL_POSITION, pos.m);
}
