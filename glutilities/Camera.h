#pragma once
#include "../common/Matrix.h"
#include "../common/Vector.h"

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
	
private:
	Vector3 m_Position;
	Vector3 m_Origin;
	Vector3 m_Target;

	float m_FOV;
	float m_AspectRatio;
	float m_Azimuth;
	float m_Elevation;
	float m_Distance;

	float m_Sensitivity;
	float m_Speed;
public:
	Camera(const Vector3& position = Vector3(0, 0, 0), const Vector3& origin = Vector3(0, 0, 0), const Vector3& target = Vector3(0, 0, 0));
	~Camera();
	Matrix4x4 LookAt(const Vector3& position, const Vector3& target = Vector3(0, 0, 0), const Vector3& up = Vector3(0, 1, 0));
	
	Matrix4x4 LookAt(){ return LookAt(m_Position, m_Target); }

	void UpdateCamera();
	
	inline Vector3& GetPosition() { return m_Position; }
	inline Vector3& GetOrigin() { return m_Origin; }
	inline Vector3& GetTarget() { return m_Target; }
	inline float& GetFOV() { return m_FOV; }
	inline float& GetAspectRatio() { return m_AspectRatio; }
	inline float& GetAzimuth() { return m_Azimuth; }
	inline float& GetElevation() { return m_Elevation; }
	inline float& GetDistance() { return m_Distance; }
	inline float& GetSensitivity() { return m_Sensitivity; }
	inline float& GetSpeed() { return m_Speed; }

	inline Vector3* GetpPosition() { return &m_Position; }
	inline Vector3* GetpOrigin() { return &m_Origin; }
	inline Vector3* GetpTarget() { return &m_Target; }
	inline float* GetpFOV() { return &m_FOV; }
	inline float* GetpAspectRatio() { return &m_AspectRatio; }
	inline float* GetpAzimuth() { return &m_Azimuth; }
	inline float* GetpElevation() { return &m_Elevation; }
	inline float* GetpDistance() { return &m_Distance; }
	inline float* GetpSensitivity() { return &m_Sensitivity; }
	inline float* GetpSpeed() { return &m_Speed; }
		


	void ProcessKeyboard(float deltaTime, CameraMovement mvt);
	void ProcessMouse(float xoffset, float yoffset);
	void ProcessScroll(float yoffset);
	
};