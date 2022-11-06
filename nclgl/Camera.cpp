#include "Camera.h"
void Camera::UpdateCamera(float dt) {
	pitch -= Window::GetMouse()->GetRelativePosition().y;
	yaw -= Window::GetMouse()->GetRelativePosition().x;
	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);
	if (yaw < 0) {
		yaw += 360.0f;
	}
	else {
		if (yaw > 360) {
			yaw -= 360.0f;
		}
	}

	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));
	Vector3 forward = rotation * Vector3(0, 0, -1);
	Vector3 right = rotation * Vector3(1, 0, 0);
	float speed = 300.0f * dt;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W))position += forward * speed;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S))position -= forward * speed;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A))position -= right * speed;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D))position += right * speed;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT))position.y -= speed;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE))position.y += speed;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_C))roll += speed;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_V))roll -= speed;

	if (roll < 0) {
		roll += 360.0f;
	}
	else {
		if (roll > 360) {
			roll -= 360.0f;
		}
	}
	//roll = 0;
}

Matrix4 Camera::BuildViewMatrix() {
	return Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) * Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(-roll,Vector3(0,0,-1)) * Matrix4::Translation(-position);
}