#include "Renderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/MeshMaterial.h"
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	projMatrix = Matrix4::Perspective(1, 10000, (float)width / height, 45);
	camera = new Camera(-3, 0, Vector3(0, 1.4f, 4));
	shader = new Shader("skinningVertex.glsl", "texturedFragment.glsl");
	if (!shader->LoadSuccess()) {
		return;
	}
	mesh = Mesh::LoadFromMeshFile("wizard_weapon_macanim.msh");
	anim = new MeshAnimation("wizard_weapon_macanim.anm");
	material = new MeshMaterial("wizard_weapon_macanim.mat");

	for (int i = 0; i < mesh->GetSubMeshCount(); i++)
	{
		const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);
		const std::string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		std::string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextures.emplace_back(texID);
	}
	currentFrame = 0;
	frameTime = 0;
	init = true;
}

Renderer::~Renderer(void) {
	delete camera;
	delete mesh;
	delete anim;
	delete material;
	delete shader;
}

void Renderer::UpdateScene(float dt){
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	frameTime -= dt;
	//std::cout << frameTime / (1/anim->GetFrameRate()) << '\n';
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += 1.0f / anim->GetFrameRate();
	}
	
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(shader);
	glUniform1i(glGetUniformLocation(shader->GetProgram(),"diffuseTex"), 0);
	UpdateShaderMatrices();
	std::vector<Matrix4> frameMatrices;
	const Matrix4* invBindPose = mesh->GetInverseBindPose();
	const Matrix4* frameData = anim->GetJointData(currentFrame);

	/*Matrix4 rotation = ((Matrix4*)(anim->GetJointData(currentFrame)))->GetTransposedRotation();
	Quaternion rotationQ = Quaternion(rotation);

	Matrix4 nextRotation = ((Matrix4*)(anim->GetJointData((currentFrame + 1) % anim->GetFrameCount())))->GetTransposedRotation();
	Quaternion nextRotationQ = Quaternion(nextRotation);
	
	Quaternion frameRotation = Quaternion::Slerp(rotationQ, nextRotationQ, frameTime / (1 / anim->GetFrameRate()));

	Vector3 pos = frameData->GetPositionVector();
	Vector3 nextPos = anim->GetJointData((currentFrame + 1) % anim->GetFrameCount())->GetPositionVector();
	Vector3 framePos = Vector3(
		pos.x * (1.0f - frameTime / (1 / anim->GetFrameRate())) + (nextPos.x * frameTime / (1 / anim->GetFrameRate())),
		pos.y * (1.0f - frameTime / (1 / anim->GetFrameRate())) + (nextPos.y * frameTime / (1 / anim->GetFrameRate())),
		pos.z * (1.0f - frameTime / (1 / anim->GetFrameRate())) + (nextPos.z * frameTime / (1 / anim->GetFrameRate()))
		);
	*/
	//Matrix4 frame = Matrix4::Translation(framePos) * 

	for (int i = 0; i < mesh->GetJointCount(); i++)
	{
		frameMatrices.emplace_back(frameData[i]);
	}
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(),"joints"), frameMatrices.size(), false, (float*)frameMatrices.data());
	for (int i = 0; i < mesh->GetSubMeshCount(); i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		mesh->DrawSubMesh(i);
	}
}