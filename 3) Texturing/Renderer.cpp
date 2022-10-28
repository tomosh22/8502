#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	triangle = Mesh::GenerateTriangle();

	texture0 = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	textureHandle0 = glGetTextureHandleARB(texture0);
	glMakeTextureHandleResidentARB(textureHandle0);
	
	texture1 = SOIL_load_OGL_texture(TEXTUREDIR"stainedglass.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	textureHandle1 = glGetTextureHandleARB(texture1);
	glMakeTextureHandleResidentARB(textureHandle1);
	
	if (!texture0)return;

	shader = new Shader("texturedVertex.glsl", "texturedFragment.glsl");
	if (!shader->LoadSuccess())return;
	filtering = true;
	repeating = false;

	//glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), textureHandle0);
	BindShader(shader);
	glUniformHandleui64ARB(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), textureHandle1);

	init = true;
}

Renderer::~Renderer(void) {
	delete triangle;
	delete shader;
	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	UpdateShaderMatrices();
	
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture0);
	triangle->Draw();
}

void Renderer::UpdateTextureMatrix(float value) {
	Matrix4 push = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	Matrix4 pop = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	Matrix4 rotation = Matrix4::Rotation(value, Vector3(0.5f, 0.5f, 0));
	textureMatrix = pop * rotation * push;
}

void Renderer::ToggleRepeating() {
	return;
	repeating = !repeating;
	SetTextureRepeating(texture0, repeating);
}

void Renderer::ToggleFiltering() {
	return;
	filtering = !filtering;
	//glBindTexture(GL_TEXTURE_2D, texture0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
	//glBindTexture(GL_TEXTURE_2D, 0);
}