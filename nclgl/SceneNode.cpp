#include "SceneNode.h"
#include "OGLRenderer.h"
SceneNode::SceneNode(GLuint matrixUBO,Mesh* mesh, Vector4 colour) {
	this->mesh = mesh;
	this->colour = colour;
	mat = nullptr;
	anim = nullptr;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	texture = 0;
	frameTime = 0;
	currentFrame = 0;
	shader = nullptr;
	this->matrixUBO = matrixUBO;
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

void SceneNode::Draw(OGLRenderer& r) {
	if (anim != nullptr) {
		if (r.currentShader != shader) {
			r.currentShader = shader;
			glUseProgram(shader->GetProgram());
			glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
			glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(worldTransform.values));
			std::vector<Matrix4> frameMatrices;
			const Matrix4* invBindPose = mesh->GetInverseBindPose();
			const Matrix4* frameData = anim->GetJointData(currentFrame);

			for (int i = 0; i < mesh->GetJointCount(); i++)
			{
				frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
			}
			glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "joints"), frameMatrices.size(), false, (float*)frameMatrices.data());
			for (int i = 0; i < mesh->GetSubMeshCount(); i++)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, matTextures[i]);
				mesh->DrawSubMesh(i);
			}

		}
	}
	else {
		if (mesh)mesh->Draw();
	}
	
}

void SceneNode::Update(float dt) {
	frameTime -= dt;
	if (parent) { worldTransform = parent->worldTransform * transform; }
	if (anim != nullptr) {
		while (frameTime < 0.0f) {
			currentFrame = (currentFrame + 1) % anim->GetFrameCount();
			frameTime += 1.0f / anim->GetFrameRate();
			std::cout << frameTime;
		}
	}
	else { worldTransform = transform; }
	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); i++) {
		(*i)->Update(dt);
	}
}