#include "Renderer.h"
#include "../nclgl/Light.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Particle.h"
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glClearColor(0.2, 0.4, 0.2, 1);
	srand(time(0));
	particleTexture = SOIL_load_OGL_texture(TEXTUREDIR"flame_particle.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	grassShader = new Shader("grassVertex.glsl", "grassFragment.glsl","","","");
	if (!grassShader->LoadSuccess()) {
		return;
	}
	grassQuad = Mesh::GenerateQuad();
	grassQuad->modelMatrix = grassQuad->modelMatrix * Matrix4::Translation(Vector3(50, 50, -100));
	grassQuad->modelMatrix = grassQuad->modelMatrix * Matrix4::Scale(Vector3(50, 50, 50));
	grassQuad->modelMatrix = grassQuad->modelMatrix * Matrix4::Rotation(90,Vector3(1, 0, 0));
	tesselationLevel = 1;
	
	particleTime = 0;
	particleShader = new Shader("particleVertex.glsl", "particleFragment.glsl");
	BindShader(particleShader);
	particles = new std::array<Particle*,MAX_PARTICLES>();
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		particles->at(x) = new Particle();
	}
	particleIndex = 0;
	masterParticle = Mesh::GenerateQuad();

	column0s = new std::array<std::array<float, 4>, MAX_PARTICLES>();
	column1s = new std::array<std::array<float, 4>, MAX_PARTICLES>();
	column2s = new std::array<std::array<float, 4>, MAX_PARTICLES>();
	column3s = new std::array<std::array<float, 4>, MAX_PARTICLES>();
	coloursGPU = new std::array<std::array<float, 4>, MAX_PARTICLES>();

	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &vbo3);
	glGenBuffers(1, &vbo4);
	glGenBuffers(1, &vbo5);
	glGenBuffers(1, &vbo6);
	
	


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
	camera = new Camera(0, 0, Vector3(50, 50, 50));
	light = new Light(heightMapSize * Vector3(0.5f, 1.5f, 0.5f), Vector4(1, 0.9, 0.5, 1), Vector4(1, 0.9, 0.5, 1), heightMapSize.x * 0.5f);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / height, 45.0f);
	modelMatrix = Matrix4();
	modelMatrix.ToIdentity();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glGenBuffers(1, &matrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glUniformBlockBinding(shader->GetProgram(), glGetUniformBlockIndex(shader->GetProgram(), "matrices"), 0);
	glUniformBlockBinding(particleShader->GetProgram(), glGetUniformBlockIndex(particleShader->GetProgram(), "matrices"), 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(Matrix4), NULL, GL_STATIC_DRAW);



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
	delete particles;
	delete particleShader;
	delete masterParticle;
	delete column0s;
	delete column1s;
	delete column2s;
	delete column3s;
	delete coloursGPU;
	delete grassQuad;
	delete grassShader;
	//todo fix memory leak
	/*for (int i = 0; i < MAX_PARTICLES; i++)
	{
		delete particles->at(i);
	}
	delete particles;*/
}

void Renderer::GenerateParticles(float dt, Vector3 position, int radius) {
	particleTime += dt;
		for (int x = 0; x < 100; x++)
		{
			if (particleIndex == MAX_PARTICLES - 1) {
				return;
			}
			particles->at(particleIndex++)->InitParticle(
				position + Vector3((rand() % radius) - radius / 2, (rand() % radius) - radius / 2, (rand() % radius) - radius / 2),
				Vector3((rand() % 50) - 25, -100 - rand() % 50, (rand() % 50) - 25),
				Vector3(0, -50, 0),
				3,
				Vector4(0, 0, 1, 1),
				Vector4(0, 1, 1, 1),
				false,
				Vector3(5, 5, 5),
				Vector3(3, 3, 3),
				false
			);
			particleTime = 0;
		}
	
}

void Renderer::UpdateParticles(float dt) {
	for (int x = 0; x < particleIndex; x++)
	{
		if (!particles->at(x)->UpdateParticle(dt)) {
			particles->at(x)->alive = false;
			particles->at(x)->elapsed = 0;
			particles->at(x)->colour = particles->at(x)->startColour;
			particles->at(x)->modelMatrix.ToIdentity();
			std::swap(particles->at(x), particles->at(--particleIndex));
			column0s->at(particleIndex) = { 0,0,0,0 };
			column1s->at(particleIndex) = { 0,0,0,0 };
			column2s->at(particleIndex) = { 0,0,0,0 };
			column3s->at(particleIndex) = { 0,0,0,0 };
			coloursGPU->at(particleIndex) = { 0,0,0,0 };
		}
	}
}

void Renderer::UpdateScene(float dt) {
	std::cout << (float)1 / dt << " fps\n" << particleIndex << " particles\n";
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_0)) { delete heightMap; heightMap = new HeightMap(); }
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))particles->at(0)->modelMatrix = particles->at(0)->modelMatrix * Matrix4::Translation(Vector3(1, 1, 1));
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_2))particles->at(1)->modelMatrix = particles->at(1)->modelMatrix * Matrix4::Translation(Vector3(1, 1, 1));
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP))tesselationLevel = tesselationLevel == 64 ? 64 : tesselationLevel + 1;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN))tesselationLevel = tesselationLevel == 1 ? 1 : tesselationLevel-1;
	
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &(viewMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	UpdateParticles(dt);
	GenerateParticles(dt,Vector3(0,0,0),100);
	
}





Matrix4 Renderer::GenerateTransposedMatrix(Particle* p) {
	Matrix4 matrix;
	matrix = Matrix4::Translation(p->modelMatrix.GetPositionVector());
	matrix.values[1] = viewMatrix.values[4];
	matrix.values[2] = viewMatrix.values[8];
	matrix.values[4] = viewMatrix.values[1];
	matrix.values[6] = viewMatrix.values[9];
	matrix.values[8] = viewMatrix.values[2];
	matrix.values[9] = viewMatrix.values[6];
	matrix.values[0] = viewMatrix.values[0];
	matrix.values[5] = viewMatrix.values[5];
	matrix.values[10] = viewMatrix.values[10];
	matrix = viewMatrix * matrix * Matrix4::Scale(p->GetScale());
	return matrix;
}

void Renderer::RenderParticles() {
	
	BindShader(particleShader);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(modelMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glUniform1i(glGetUniformLocation(particleShader->GetProgram(), "particleTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particleTexture);
	
	for (int x = 0; x < particleIndex;x++)
	{
		if (!particles->at(x)->alive) {
			return;
		}
		Matrix4 matrix = GenerateTransposedMatrix(particles->at(x));
		for (int i = 0; i < 4; i++)
		{
			column0s->at(x).at(i) = matrix.values[i];
		}
		for (int i = 0; i < 4; i++)
		{
			column1s->at(x).at(i) = matrix.values[4+i];
		}
		for (int i = 0; i < 4; i++)
		{
			column2s->at(x).at(i) = matrix.values[8+i];
		}
		for (int i = 0; i < 4; i++)
		{
			column3s->at(x).at(i) = matrix.values[12+i];
		}

		coloursGPU->at(x).at(0) = particles->at(x)->GetColour().x;
		coloursGPU->at(x).at(1) = particles->at(x)->GetColour().y;
		coloursGPU->at(x).at(2) = particles->at(x)->GetColour().z;
		coloursGPU->at(x).at(3) = particles->at(x)->GetColour().w;
	}
	glBindVertexArray(masterParticle->GetVAO());

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, particleIndex * 4 * sizeof(float), column0s->data(), GL_STREAM_DRAW);
	glVertexAttribPointer(1, sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferData(GL_ARRAY_BUFFER, particleIndex * 4 * sizeof(float), column1s->data(), GL_STREAM_DRAW);
	glVertexAttribPointer(3, sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo4);
	glBufferData(GL_ARRAY_BUFFER, particleIndex * 4 * sizeof(float), column2s->data(), GL_STREAM_DRAW);
	glVertexAttribPointer(4, sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo5);
	glBufferData(GL_ARRAY_BUFFER, particleIndex * 4 * sizeof(float), column3s->data(), GL_STREAM_DRAW);
	glVertexAttribPointer(5, sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo6);
	glBufferData(GL_ARRAY_BUFFER, particleIndex * 4 * sizeof(float), coloursGPU->data(), GL_STREAM_DRAW);
	glVertexAttribPointer(6, sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);
	
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleIndex);
	

}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(modelMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	BindShader(shader);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(shader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bumpMap);

	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	SetShaderLight(*light);
	heightMap->Draw();
	RenderParticles();

	RenderGrass();
}

void Renderer::RenderGrass() {
	BindShader(grassShader);
	/*glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(grassQuad->modelMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(grassQuad->GetVAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	return;*/
	
	
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glUniform1i(glGetUniformLocation(grassShader->GetProgram(), "tesselationLevel"), tesselationLevel);

	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(grassQuad->modelMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &(viewMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(Matrix4), sizeof(Matrix4), &(projMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(grassQuad->GetVAO());
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);
}

