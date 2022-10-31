#include "SceneNode.h"
SceneNode::SceneNode(GLuint64 textureHandle,Mesh* mesh, Vector4 colour) {
	std::cout << "this should be called\n";
	this->mesh = mesh;
	this->colour = colour;
	this->textureHandle = textureHandle;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	texture = 0;
}

SceneNode::~SceneNode(void) {
	for (unsigned int i = 0; i < children.size(); i++) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->parent = this;
}

void SceneNode::Draw(const OGLRenderer& r) {
	
	if (mesh)mesh->Draw();
}

void SceneNode::Update(float dt) {
	if (parent) { worldTransform = parent->worldTransform * transform; }
	else { worldTransform = transform; }
	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); i++) {
		(*i)->Update(dt);
	}
}