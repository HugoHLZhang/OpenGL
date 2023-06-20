#include "Camera.h"
#include "../common/Matrix.h"
#include "../common/Vector.h"
#define _USE_MATH_DEFINES
#include <math.h>
Camera::Camera(const Vector3& position, const Vector3& origin, const Vector3& target) : pos(position), origine(origin), target(target)
{
	fov = 45;
	aspectRatio = 1.0f;
	azimuth = 0;
	elevation = 0;
	distance = 0;
	sensitivity = 0.1f;
	speed = 0.1f;
}

Camera::~Camera()
{

}

void Camera::ProcessMouse(float xoffset, float yoffset)
{
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	azimuth -= xoffset;
	elevation += yoffset;
	
	if (elevation > 90.0f)
		elevation = 90.0f;
	if (elevation < -90.0f)
		elevation = -90.0f;

	if (azimuth > 180.0f)
		azimuth = -180.0f;
	if (azimuth < -180.0f)
		azimuth = 180.0f;
	UpdateCamera();
}

void Camera::ProcessScroll(float yoffset) {
	distance += speed * yoffset;
	if (distance == 0)
		distance = 0.1f;
	UpdateCamera();
}

void Camera::UpdateCamera() {
	float x = distance * cos(elevation * M_PI / 180.0f) * cos(azimuth * M_PI / 180.0f);
	float y = distance * sin(elevation * M_PI / 180.0f);
	float z = distance * cos(elevation * M_PI / 180.0f) * sin(azimuth * M_PI / 180.0f);
	pos = origine + Vector3(x, y, z);
}

Matrix4x4 Camera::LookAt(const Vector3& position,const Vector3& target, const Vector3& up_) {
	UpdateCamera();
    Matrix4x4 result;
    Vector3 forward = -(target - position).normalize();
	Vector3 right = forward.cross(up_).normalize();
	Vector3 up = right.cross(forward).normalize();
	result.Set(0,0,right[0]);
	result.Set(0,1,right[1]);
	result.Set(0,2,right[2]);
	result.Set(0,3,-right.dot(position));
	
	result.Set(1,0,up[0]);
	result.Set(1,1,up[1]);
	result.Set(1,2,up[2]);
	result.Set(1,3,-up.dot(position));
	
	result.Set(2,0,forward[0]);
	result.Set(2,1,forward[1]);
	result.Set(2,2,forward[2]);
	result.Set(2,3,-forward.dot(position));
	result.Set(3,3,1);
	
    return result;
}