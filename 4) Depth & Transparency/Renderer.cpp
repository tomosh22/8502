#include "Renderer.h"
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	meshes[0] = Mesh::GenerateQuad();
	meshes[1] = Mesh::GenerateTriangle();
	textures[0] = SOIL_load_OGL_texture(TEXTUREDIR"circuit.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	textures[1] = SOIL_load_OGL_texture(TEXTUREDIR"metall010-new-tileable.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	textures[2] = SOIL_load_OGL_texture(TEXTUREDIR"doge.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	textureHandles[0] = glGetTextureHandleARB(textures[0]);
	textureHandles[1] = glGetTextureHandleARB(textures[1]);
	textureHandles[2] = glGetTextureHandleARB(textures[2]);
	glMakeTextureHandleResidentARB(textureHandles[0]);
	glMakeTextureHandleResidentARB(textureHandles[1]);
	glMakeTextureHandleResidentARB(textureHandles[2]);
	if (!textures[0] || !textures[1]) {
		return;
	}
	positions[0] = Vector3(0, 0, -5);
	positions[1] = Vector3(0, 0, -5);
	shader = new Shader("texturedVertex.glsl", "dogeFragment.glsl");
	if (!shader->LoadSuccess()) { 
		return;
	}
	usingDepth = false;
	usingAlpha = false;
	modifyObject = true;
	blendMode = 0;
	projMatrix = Matrix4::Perspective(1.0f, 100.0f, (float)width / height, 45.0f);
	time = 0;
	init = true;
}

Renderer::~Renderer(void) {
	delete meshes[0];
	delete meshes[1];
	delete shader;
	glDeleteTextures(2, textures);
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	BindShader(shader);
	UpdateShaderMatrices();
	glUniformHandleui64ARB(glGetUniformLocation(shader->GetProgram(), "circuitTex"),textureHandles[0]);
	glUniformHandleui64ARB(glGetUniformLocation(shader->GetProgram(), "metalTex"),textureHandles[1]);
	glUniformHandleui64ARB(glGetUniformLocation(shader->GetProgram(), "dogeTex"),textureHandles[2]);
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "time"), time);
	for (unsigned int i = 0; i < 1; i++) {
		glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, (float*)&Matrix4::Translation(positions[i]));
		//glBindTexture(GL_TEXTURE_2D, textures[i]);
		meshes[i]->Draw();
	}
}

void Renderer::ToggleObject() {
	modifyObject = !modifyObject;
}
void Renderer::MoveObject(float by) {
	positions[(int)modifyObject].z += by;
}

void Renderer::ToggleBlendMode() {
	blendMode = (blendMode + 1) % 4;
	switch (blendMode) {
	case 0:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case 1:
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
		break;
	case 2:
		glBlendFunc(GL_ONE, GL_ZERO);
		break;
	case(3):
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	}
}

void Renderer::ToggleDepth() {
	usingDepth = !usingDepth;
	usingDepth ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void Renderer::ToggleAlphaBlend() {
	usingAlpha = !usingAlpha;
	usingAlpha ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void Renderer::UpdateScene(float dt) {
	time += dt;
	if(time > 1.0f)time-=1.0f;
	std::cout << time << '\n';
}