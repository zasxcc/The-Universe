#include "LibraryHeader.h"
#include "CGTime.h"
#include <timeapi.h>

/* ������(ȭ�� 1���� �����ð�)�� ���� ��ü �̵��� ����ó���� ���� �Լ� ���� */

DWORD g_prevTime = 0;

float calculateElapseTime()
{
	if (g_prevTime == 0) {
		g_prevTime = timeGetTime();
		return 0.f;
	}
	DWORD currTime = timeGetTime();
	DWORD elapsedTime = currTime - g_prevTime;
	g_prevTime = currTime;
	float eTime = static_cast<float>(elapsedTime) / 1000.f; /* DWORD �ڷ����� float���� */
	//printf("%f\n", eTime);	//*********************** TEST
	return eTime;
}