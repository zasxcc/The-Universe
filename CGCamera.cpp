#include "LibraryHeader.h"
#include "CGCamera.h"

Camera::Camera()
	: mode	{ Camera::Perspective }
	, pos	{ 0.f, 0.f, 0.f }
	, look	{ 0.f, 0.f,  -1.f }
	, up	{ 0.f, 1.f,  0.f}
	, right	{ 1.f, 0.f,	 0.f}
	, yaw	{ 0.f }
	, roll	{ 0.f }
	, pitch	{ 0.f }
{
}

Camera::~Camera()
{
}

myVector4f Camera::getPos() const
{
	return pos;
}
myVector4f Camera::getLook() const
{
	return look;
}

void Camera::setAccel(const myVector4f & accel)
{
	Camera::accel = accel;
}

void Camera::setPos(float x, float y, float z)
{
	pos = myVector4f(x, y, z);
}

void Camera::setPos(const myVector4f & pos)
{
	Camera::pos = pos;
}

void Camera::LookAt(const myVector4f & AT)
{
	myVector4f worldAxisY{ 0.f,1.f,0.f };
	look = normalize(AT - pos);
	right = crossproduct(look, worldAxisY);
	up = crossproduct(look, right);
}

void Camera::Rotate(float pitch, float yaw, float roll)
{
	/* pitch 상하 최소최대각은 -80 ~ 80 도. 90 상하좌우 반전방지 */
	if (Camera::pitch + pitch < -80.f) { pitch = -80.f - Camera::pitch; }
	else if (Camera::pitch + pitch > 80.f) { pitch = 80.f - Camera::pitch; }

	Camera::pitch += pitch;
	Camera::yaw += yaw;
	Camera::roll += roll;

	if (Camera::yaw > 360.f) { Camera::yaw -= 360.f; }
	if (Camera::yaw < 0.f) { Camera::yaw += 360.f; }

	auto mtx{ myMatrix44f::Rotate(look, roll) * myMatrix44f::Rotate(up, yaw) * myMatrix44f::Rotate(right, Camera::pitch) };
	const myVector4f worldAxisY{ 0.f,1.f,0.f };
	look.y = 0.f;
	look = normalize(mtx * normalize(look));
	right = normalize(crossproduct(look, worldAxisY));
	up = normalize(crossproduct(right, look));
}

void Camera::setProjectionMode(const ProjectionOption & input)
{
	Camera::mode = input;
}

void Camera::ModifyProjectionMode(int w, int h)
{
	switch (Camera::mode)
	{
	case Camera::Orthographic:
		ModeOrtho(w, h);
		break;
	case Camera::Perspective:
		ModePerspective(w, h);
		break;
	default:
		printf("ERROR[!] Camera Setting Error Occurs\n");
		break;
	}
}

void Camera::ModePerspective(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(frustum_fovy, static_cast<GLdouble>(w) / static_cast<GLdouble>(h), frustum_Znear, frustum_Zfar);

	myVector4f at{ pos + look };
	gluLookAt(
		pos.x, pos.y, pos.z,
		at.x, at.y, at.z,
		up.x, up.y, up.z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Camera::ModeOrtho(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(
		-static_cast<GLdouble>(w / 2),
		static_cast<GLdouble>(w / 2),
		-static_cast<GLdouble>(h / 2),
		static_cast<GLdouble>(h / 2),
		frustum_Znear,
		frustum_Zfar
	);

	myVector4f at{ pos + look };
	gluLookAt(
		pos.x, pos.y, pos.z,
		at.x, at.y, at.z,
		up.x, up.y, up.z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Camera::update(float eTime)
{
	vel = vel + accel * eTime;
	pos = pos + vel * eTime;
}

void Camera::CameraKeyMoving(unsigned char key, float eTime)
{
	switch (key)
	{
	case '0':
		accel = zeroVector;
		vel = zeroVector;
		break;
	case 'w': 
		pos = pos + look * speed * eTime;
		break;
	case 'a':
		pos = pos - right * speed * eTime;
		break;
	case 's':
		pos = pos - look * speed * eTime;
		break;
	case 'd':
		pos = pos + right * speed * eTime;
		break;
	case 'c': /* 치트모드 세팅 */
		speed = CHEAT_CAMERA_SPEED;
		break;
	case 'n':
		speed = DEFAULT_CAMERA_SPEED; 
		break;
	default:
		break;
	}
}

