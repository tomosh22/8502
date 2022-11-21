#include "CubeRobot.h"
CubeRobot::CubeRobot(GLuint matrixUBO,Mesh* cube, Vector3 pos) {

	this->pos = pos;

	SceneNode* body = new SceneNode(matrixUBO,cube, Vector4(1, 0, 0, 1));
	body->SetModelScale(Vector3(10, 15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(0,35,0)+pos));
	body->SetBoundingRadius(15.0f);
	AddChild(body);

	head = new SceneNode(matrixUBO, cube, Vector4(0, 1, 0, 1));
	head->SetModelScale(Vector3(5, 5, 5));
	head->SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	head->SetBoundingRadius(5.0f);
	body->AddChild(head);

	leftArm = new SceneNode(matrixUBO, cube, Vector4(0, 0, 1, 1));
	leftArm->SetModelScale(Vector3(3, -18, 3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	leftArm->SetBoundingRadius(18.0f);
	body->AddChild(leftArm);

	rightArm = new SceneNode(matrixUBO, cube, Vector4(0, 0, 1, 1));
	rightArm->SetModelScale(Vector3(3, -18, 3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	rightArm->SetBoundingRadius(18.0f);
	body->AddChild(rightArm);

	SceneNode* leftLeg = new SceneNode(matrixUBO, cube, Vector4(0, 0, 1, 1));
	leftLeg->SetModelScale(Vector3(3, -17.5, 3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	leftLeg->SetBoundingRadius(18.0f);
	body->AddChild(leftLeg);

	SceneNode* rightLeg = new SceneNode(matrixUBO, cube, Vector4(0, 0, 1, 1));
	rightLeg->SetModelScale(Vector3(3, -17.5, 3));
	rightLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	rightLeg->SetBoundingRadius(18.0f);
	body->AddChild(rightLeg);

}

void CubeRobot::Update(float dt) {
	transform = transform * Matrix4::Translation(pos) * Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0)) * Matrix4::Translation(-pos);
	head->SetTransform(head->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));
	leftArm->SetTransform(leftArm->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(1, 0, 0)));
	rightArm->SetTransform(rightArm->GetTransform() * Matrix4::Rotation(30.0f * dt, Vector3(1, 0, 0)));
	SceneNode::Update(dt);
}