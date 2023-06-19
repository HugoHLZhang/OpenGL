#include "Camera.h"
#include "../common/Matrix.h"
#include "../common/Vector.h"
#define _USE_MATH_DEFINES
#include <math.h>
Camera::Camera(const Vector3& position, const Vector3& origin, const Vector3& target) : m_Position(position), m_Origin(origin), m_Target(target)
{
	m_FOV = 45;
	m_AspectRatio = 1.0f;
	m_Azimuth = 0;
	m_Elevation = 0;
	m_Distance = 0;
	m_Sensitivity = 0.1f;
	m_Speed = 0.1f;
}

Camera::~Camera()
{

}

void Camera::ProcessKeyboard(float deltaTime, CameraMovement mvt)
{
	//float velocity = m_Origin * deltaTime;
	//if (mvt == FORWARD)
	//	m_Position += m_Origin * velocity;
	//if (mvt == BACKWARD)
	//	m_Position -= m_Origin * velocity;
	//if (mvt == LEFT)
	//	m_Position -= m_Origin * velocity;
	//if (mvt == RIGHT)
	//	m_Position += m_Origin * velocity;
}

void Camera::ProcessMouse(float xoffset, float yoffset)
{
	xoffset *= m_Sensitivity;
	yoffset *= m_Sensitivity;

	m_Azimuth -= xoffset;
	m_Elevation += yoffset;
	
	if (m_Elevation > 90.0f)
		m_Elevation = 90.0f;
	if (m_Elevation < -90.0f)
		m_Elevation = -90.0f;

	if (m_Azimuth > 180.0f)
		m_Azimuth = -180.0f;
	if (m_Azimuth < -180.0f)
		m_Azimuth = 180.0f;
	UpdateCamera();
}

void Camera::ProcessScroll(float yoffset) {
	m_Distance += m_Speed * yoffset;
	if (m_Distance == 0)
		m_Distance = 0.1f;
	UpdateCamera();
}

void Camera::UpdateCamera() {
	float x = m_Distance * cos(m_Elevation * M_PI / 180.0f) * cos(m_Azimuth * M_PI / 180.0f);
	float y = m_Distance * sin(m_Elevation * M_PI / 180.0f);
	float z = m_Distance * cos(m_Elevation * M_PI / 180.0f) * sin(m_Azimuth * M_PI / 180.0f);
	m_Position = m_Origin + Vector3(x, y, z);
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
