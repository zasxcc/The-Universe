#include <gl/freeglut.h>
#include "LibraryHeader.h"
#include "GlobalDefines.h"

/* 작성자 2013182024 SMS, 2013182020 PIH
*/

/* 모니터 화면 정중앙에 배치하려고 */
int INIT_WIN_POS_X = MAX_MONITOR_WIDTH / 2 - MY_WINDOW_WIDTH / 2;
int INIT_WIN_POS_Y = MAX_MONITOR_HEIGHT / 2 - MY_WINDOW_HEIGHT / 2;

GLenum Src = GL_SRC_ALPHA;
GLenum Dest = GL_ONE_MINUS_SRC_ALPHA;

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//	double buffering display mode
	glutInitWindowPosition(INIT_WIN_POS_X, INIT_WIN_POS_Y);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("THE UNIVERSE");

	//	Set Call Back Fuctions
	glutDisplayFunc(CB_renderScene);					//	Display
	glutReshapeFunc(CB_reshape);						//	Reshape
	glutIdleFunc(CB_idle);								//	Idle
	glutMouseFunc(CB_mouseMoveFunc);					//	Mouse Move (Click)
	glutPassiveMotionFunc(CB_mousePassiveMotionFunc);	//	Mouse Move (Position)
	glutKeyboardFunc(CB_keyDownFunc);					//	Keyboard Down
	glutKeyboardUpFunc(CB_keyUpFunc);					//	Keyboard Up
		
	//	GL Mode Enable	//
	/* 고정 쉐이더 */
	glShadeModel(GL_SMOOTH);
	/* 깊이 검사 */
	glEnable(GL_DEPTH_TEST);
	/* 컬링 */
	glEnable(GL_CULL_FACE);
	/* 알파블렌딩 */
	glEnable(GL_BLEND);
	glBlendFunc(Src, Dest);
	/* 법선벡터를 단위벡터로 */
	glEnable(GL_NORMALIZE);

	/* 텍스처 */
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_2D);

	/* 빛 */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//	여기에 Initiate 함수를 호출할 예정입니다. 
	Initiate();

	glutMainLoop();

	//	여기에 Release 함수와 메모리 해제를 해줍시다.
	Release();

	return 0;
}
