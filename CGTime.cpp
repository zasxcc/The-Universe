#include "LibraryHeader.h"
#include "CGTime.h"
#include <timeapi.h>

/* 프레임(화면 1장의 렌더시간)에 따른 객체 이동과 연산처리를 위한 함수 정의 */

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
	float eTime = static_cast<float>(elapsedTime) / 1000.f; /* DWORD 자료형을 float으로 */
	//printf("%f\n", eTime);	//*********************** TEST
	return eTime;
}