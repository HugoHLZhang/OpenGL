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
	Vector3 pos;
	Vector3 origine;
	Vector3 target;

	float fov;
	float aspectRatio;
	float azimuth;
	float elevation;
	float distance;

	float sensitivity;
	float speed;
public:
	Camera(const Vector3& position = Vector3(0, 0, 0), const Vector3& origin = Vector3(0, 0, 0), const Vector3& target = Vector3(0, 0, 0));
	~Camera();
	Matrix4x4 LookAt(const Vector3& position, const Vector3& target = Vector3(0, 0, 0), const Vector3& up = Vector3(0, 1, 0));
	
	Matrix4x4 LookAt(){ return LookAt(pos, target); }

	void UpdateCamera();
	
	inline Vector3& GetPosition() { return pos; }
	inline Vector3& GetOrigin() { return origine; }
	inline Vector3& GetTarget() { return target; }
	inline float& GetFOV() { return fov; }
	inline float& GetAspectRatio() { return aspectRatio; }
	inline float& GetAzimuth() { return azimuth; }
	inline float& GetElevation() { return elevation; }
	inline float& GetDistance() { return distance; }
	inline float& GetSensitivity() { return sensitivity; }
	inline float& GetSpeed() { return speed; }

	inline Vector3* GetpPosition() { return &pos; }
	inline Vector3* GetpOrigin() { return &origine; }
	inline Vector3* GetpTarget() { return &target; }
	inline float* GetpFOV() { return &fov; }
	inline float* GetpAspectRatio() { return &aspectRatio; }
	inline float* GetpAzimuth() { return &azimuth; }
	inline float* GetpElevation() { return &elevation; }
	inline float* GetpDistance() { return &distance; }
	inline float* GetpSensitivity() { return &sensitivity; }
	inline float* GetpSpeed() { return &speed; }

	void ProcessMouse(float xoffset, float yoffset);
	void ProcessScroll(float yoffset);
};