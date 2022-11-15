#pragma once
#include "Matrix4.h"
#include "Matrix3.h"
#include "Window.h"
#include <algorithm>
class Camera
{
public:
	Camera(void) {
		yaw = .0f;
		pitch = .0f;
		roll = .0f;
	};
	Camera(float pitch, float yaw, Vector3 position) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;
		this->roll = 0;
	}
	~Camera(void) {};
	void UpdateCamera(float dt = 1.0f);
	Matrix4 BuildViewMatrix();
	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }
	float GetYaw() const { return yaw; }
	void SetYaw(float y) { yaw = y; }
	float GetPitch() const { return pitch; }
	void SetPitch(float p) { pitch = p; }
	float GetRoll() const { return roll; }
	void SetRoll(float r) { roll = r; }

	void GoUnderwater(int waterHeight) {
		pitch = - pitch;
		position.y -= waterHeight;
		position.y = -position.y;
		position.y += waterHeight;
	};
	Vector3 position;
protected:
	float yaw, pitch,roll;
	
};

