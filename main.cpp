#include <gl/freeglut.h>
#include "LibraryHeader.h"
#include "GlobalDefines.h"

/* �ۼ��� 2013182024 SMS, 2013182020 PIH
*/

/* ����� ȭ�� ���߾ӿ� ��ġ�Ϸ��� */
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
	/* ���� ���̴� */
	glShadeModel(GL_SMOOTH);
	/* ���� �˻� */
	glEnable(GL_DEPTH_TEST);
	/* �ø� */
	glEnable(GL_CULL_FACE);
	/* ���ĺ��� */
	glEnable(GL_BLEND);
	glBlendFunc(Src, Dest);
	/* �������͸� �������ͷ� */
	glEnable(GL_NORMALIZE);

	/* �ؽ�ó */
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_2D);

	/* �� */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//	���⿡ Initiate �Լ��� ȣ���� �����Դϴ�. 
	Initiate();

	glutMainLoop();

	//	���⿡ Release �Լ��� �޸� ������ ���ݽô�.
	Release();

	return 0;
}
