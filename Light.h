#pragma once
#include "LibraryHeader.h"
#include "CGMath.h"

struct Light {
	myVector4f pos;
	myVector4f ambient;
	myVector4f diffuse;
	myVector4f specular;
	float shininess{ 0.f };

	Light();
	~Light();

	void setLightPos(const myVector4f pos);

	void setLight(); /* �� ������ �����մϴ�. */
};
