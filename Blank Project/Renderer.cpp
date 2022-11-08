#include "Renderer.h"
#include "../nclgl/Light.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	particleTexture = SOIL_load_OGL_texture(TEXTUREDIR"particle.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	for (int x = 0; x < 2; x++)
	{
		ParticleInfo particle;
		particle.quad = Mesh::GenerateQuad();
		particle.modelMatrix = Matrix4();
		particle.modelMatrix.ToIdentity();
		particles.emplace_back(particle);
	}
	ParticleInfo particle;
	particle.quad = Mesh::GenerateQuad();
	particle.modelMatrix = Matrix4();
	particle.modelMatrix.ToIdentity();
	particles.emplace_back(particle);
	particleShader = new Shader("particleVertex.glsl", "particleFragment.glsl");

	heightMap = new HeightMap();
	texture = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	bumpMap = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	shader = new Shader("bumpVertex.glsl", "bumpFragment.glsl");
	if (!shader->LoadSuccess() || !particleShader->LoadSuccess() || !texture || !bumpMap) {
		return;
	}
	SetTextureRepeating(texture, true);
	SetTextureRepeating(bumpMap, true);

	Vector3 heightMapSize = heightMap->GetHeightMapSize();
	camera = new Camera(0, 0, Vector3(0, 0, 0));
	light = new Light(heightMapSize * Vector3(0.5f, 1.5f, 0.5f), Vector4(1, 0.9, 0.5, 1), Vector4(1, 0.9, 0.5, 1), heightMapSize.x * 0.5f);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / height, 45.0f);
	modelMatrix = Matrix4();
	modelMatrix.ToIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	matrixBind = glGetUniformBlockIndex(shader->GetProgram(), "matrices");
	glUniformBlockBinding(shader->GetProgram(), matrixBind, 0);
	glGenBuffers(1, &matrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(Matrix4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, matrixUBO, 0, 3 * sizeof(Matrix4));
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(modelMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Matrix4), sizeof(Matrix4), &(projMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	init = true;
}

Renderer::~Renderer(void) {
	delete camera;
	delete heightMap;
	delete shader;
	delete light;
}

void Renderer::UpdateScene(float dt) {
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_0))heightMap = new HeightMap();
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))particles.at(0).modelMatrix = particles.at(0).modelMatrix * Matrix4::Translation(Vector3(1, 1, 1));
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_2))particles.at(1).modelMatrix = particles.at(1).modelMatrix * Matrix4::Translation(Vector3(1, 1, 1));

	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &(viewMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(particleShader);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "particleTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particleTexture);
	UpdateShaderMatrices();

	Matrix4 matrix;
	matrix = Matrix4::Translation(particles.at(0).modelMatrix.GetPositionVector());
	//memcpy(&matrix.values, &particles.at(0).modelMatrix.values, sizeof(matrix.values));
	matrix.values[1] = viewMatrix.values[4];
	matrix.values[2] = viewMatrix.values[8];
	matrix.values[4] = viewMatrix.values[1];
	matrix.values[6] = viewMatrix.values[9];
	matrix.values[8] = viewMatrix.values[2];
	matrix.values[9] = viewMatrix.values[6];
	matrix.values[0] = viewMatrix.values[0];
	matrix.values[5] = viewMatrix.values[5];
	matrix.values[10] = viewMatrix.values[10];
	matrix = viewMatrix * matrix;
	glBufferSubData(GL_UNIFORM_BUFFER, 3*sizeof(Matrix4), sizeof(Matrix4), &(matrix.values));
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(particles.at(0).modelMatrix.values));
	particles.at(0).quad->Draw();



	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(particles.at(1).modelMatrix.values));
	particles.at(1).quad->Draw();
	//return;
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(modelMatrix.values));
	BindShader(shader);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(shader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bumpMap);

	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	UpdateShaderMatrices();
	SetShaderLight(*light);
	heightMap->Draw();
}