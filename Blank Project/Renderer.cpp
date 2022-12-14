#include "Renderer.h"
#include "../nclgl/Light.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Particle.h"
#include "../nclgl/MeshMaterial.h"
#include "../nclgl/MeshAnimation.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	
	//glClearColor(0.2, 0.4, 0.2, 1);
	frameRate = 0;
	timePassed = 0;
	srand(time(0));
	particleTexture = SOIL_load_OGL_texture(TEXTUREDIR"flame_particle.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	grassShader = new Shader("grassVertex.glsl", "grassFragment.glsl","grassGeometry.glsl","grassDomain.glsl","grassHull.glsl");
	if (!grassShader->LoadSuccess()) {
		return;
	}
	grassQuad = Mesh::GenerateQuadWithIndices();
	grassQuad->modelMatrix = grassQuad->modelMatrix * Matrix4::Translation(Vector3(4000, 475, 4000));
	grassQuad->modelMatrix = grassQuad->modelMatrix * Matrix4::Scale(Vector3(200, 200, 200));
	grassQuad->modelMatrix = grassQuad->modelMatrix * Matrix4::Rotation(90,Vector3(1, 0, 0));
	tesselationLevel = 16;
	grassStartColour = Vector4(0, 0.2, 0, 1);
	grassEndColour = Vector4(0.8, 0.9, 0, 1);
	noiseTex = SOIL_load_OGL_texture(TEXTUREDIR"noise.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	particleTime = 0;
	particleLifetime = 3;
	particleShader = new Shader("particleVertex.glsl", "particleFragment.glsl");
	BindShader(particleShader);
	particles = new std::array<Particle*,MAX_PARTICLES>();
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		particles->at(x) = new Particle();
	}
	particleIndex = 0;
	masterParticle = Mesh::GenerateQuad();
	particleStartColour = Vector4(0, 0, 1, 1);
	particleEndColour = Vector4(1, 0, 1, 1);
	particleRadius = 500;

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
	
	animShader = new Shader("animatedMeshLitVertex.glsl", "animatedMeshLitFragment.glsl");
	if (!animShader->LoadSuccess()) {
		return;
	}

	/*treeMesh = Mesh::LoadFromMeshFile("P_Bush01.msh");
	treeMat = new MeshMaterial("P_Bush01.mat");
	for (int i = 0; i < treeMesh->GetSubMeshCount(); i++)
	{
		const MeshMaterialEntry* matEntry = treeMat->GetMaterialForLayer(i);
		const std::string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		std::string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		treeTextures.emplace_back(texID);
	}
	treeMesh->modelMatrix.ToIdentity();
	treeMesh->modelMatrix = treeMesh->modelMatrix * Matrix4::Translation(Vector3(100, 100, 100));
	treeMesh->modelMatrix = treeMesh->modelMatrix * Matrix4::Scale(Vector3(200, 200, 200));*/


	/*spiderMesh = Mesh::LoadFromMeshFile("spider2.msh");
	spiderMat = new MeshMaterial("spider2.mat");
	spiderAnim = new MeshAnimation("spider2.anm");
	for (int i = 0; i < spiderMesh->GetSubMeshCount(); i++)
	{
		const MeshMaterialEntry* matEntry = spiderMat->GetMaterialForLayer(i);
		const std::string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		std::string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		spiderTextures.emplace_back(texID);
	}
	spiderMesh->modelMatrix.ToIdentity();
	spiderMesh->modelMatrix = spiderMesh->modelMatrix * Matrix4::Translation(Vector3(2000, 400, 5800));
	spiderMesh->modelMatrix = spiderMesh->modelMatrix * Matrix4::Scale(Vector3(200, 200, 200));*/

	
	
	portalQuad = Mesh::GenerateQuad();
	portalQuad->modelMatrix.ToIdentity();
	portalQuad->modelMatrix = portalQuad->modelMatrix * Matrix4::Translation(Vector3(1000, 2000, 8000));
	portalQuad->modelMatrix = portalQuad->modelMatrix * Matrix4::Scale(Vector3(500, 1000, 500));
	portalViewpointQuad = Mesh::GenerateQuad();
	portalViewpointQuad->modelMatrix.ToIdentity();
	portalViewpointQuad->modelMatrix = portalViewpointQuad->modelMatrix * Matrix4::Translation(Vector3(5000, 2000, 8000));
	portalViewpointQuad->modelMatrix = portalViewpointQuad->modelMatrix * Matrix4::Scale(Vector3(500, 1000, 500));
	portalShader = new Shader("portalVertex.glsl", "portalFragment.glsl");
	if (!portalShader->LoadSuccess()) {
		return;
	}
	SetupPortalFBOs();


	currentFrame = 0;
	frameTime = 0;
	

	heightMap = new HeightMap(TEXTUREDIR"heightmap.png",true);
	texture = SOIL_load_OGL_texture(TEXTUREDIR"red_laterite_soil_stones_diff_4k.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	bumpMap = SOIL_load_OGL_texture(TEXTUREDIR"red_laterite_soil_stones_nor_gl_4k.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	shader = new Shader("bumpVertex.glsl", "bumpFragment.glsl");
	if (!shader->LoadSuccess() || !particleShader->LoadSuccess() || !texture || !bumpMap) {
		return;
	}
	
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "clipHeight"), -99999);
	SetTextureRepeating(texture, true);
	SetTextureRepeating(bumpMap, true);

	Vector3 heightMapSize = heightMap->GetHeightMapSize();
	camera = new Camera(0, 0, Vector3(3500,1000,5500));

	lightIntensity = 1;
	lightDiffuseColour = Vector4(0.2, 1, 0.2,1);
	lightSpecularColour = Vector4(0, 1, 0,1);
	lightRadius = 1000;
	light = new Light(Vector3(3300, 2000, 5850), lightDiffuseColour * lightIntensity, lightSpecularColour, lightRadius);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / height, 45.0f);
	modelMatrix = Matrix4();
	modelMatrix.ToIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CLIP_DISTANCE0);
	
	glGenBuffers(1, &matrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glUniformBlockBinding(shader->GetProgram(), glGetUniformBlockIndex(shader->GetProgram(), "matrices"), 0);
	glUniformBlockBinding(particleShader->GetProgram(), glGetUniformBlockIndex(particleShader->GetProgram(), "matrices"), 0);
	glUniformBlockBinding(grassShader->GetProgram(), glGetUniformBlockIndex(grassShader->GetProgram(), "matrices"), 0);
	glUniformBlockBinding(animShader->GetProgram(), glGetUniformBlockIndex(animShader->GetProgram(), "matrices"), 0);
	glUniformBlockBinding(portalShader->GetProgram(), glGetUniformBlockIndex(portalShader->GetProgram(), "matrices"), 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(Matrix4), NULL, GL_STATIC_DRAW);



	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(modelMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Matrix4), sizeof(Matrix4), &(projMatrix.values));

	
	


	SetupWaterFBOs();
	std::vector<Vector2> positions;
	positions.emplace_back(Vector2(0, 0));
	positions.emplace_back(Vector2(0, heightMap->GetHeightMapSize().z));
	positions.emplace_back(Vector2(heightMap->GetHeightMapSize().x, 0));
	
	positions.emplace_back(Vector2(heightMap->GetHeightMapSize().x, heightMap->GetHeightMapSize().z));
	
	
	
	
	waterHeight = 200;
	waterQuad = Mesh::GenerateQuad(positions);
	waterQuad->modelMatrix.ToIdentity();
	waterQuad->modelMatrix = waterQuad->modelMatrix * Matrix4::Translation(Vector3(0, waterHeight, 0));
	waterQuad->modelMatrix = waterQuad->modelMatrix * Matrix4::Rotation(90, Vector3(1, 0, 0));
	reflectGrass = true;
	reflectParticles = true;
	
	guiShader = new Shader("texturedVertex.glsl", "texturedFragment.glsl");
	glUniformBlockBinding(guiShader->GetProgram(), glGetUniformBlockIndex(guiShader->GetProgram(), "matrices"), 0);
	blendFactor = 0.5;
	if (!guiShader->LoadSuccess()) {
		return;
	}

	waterTex= SOIL_load_OGL_texture(TEXTUREDIR"Water 0341.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	waterNormal = SOIL_load_OGL_texture(TEXTUREDIR"Water 0341normal.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	SetTextureRepeating(waterTex, true);
	SetTextureRepeating(waterNormal, true);

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"px.jpg", TEXTUREDIR"nx.jpg",
		TEXTUREDIR"py.jpg", TEXTUREDIR"ny.jpg",
		TEXTUREDIR"pz.jpg", TEXTUREDIR"nz.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	if (!cubeMap) {
		return;
	}




	skyboxQuad = Mesh::GenerateQuad();
	skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	



	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(parent.GetHandle());
	ImGui_ImplOpenGL3_Init("#version 330");
	

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



void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);

	BindShader(skyboxShader);
	UpdateShaderMatrices();
	skyboxQuad->Draw();
	glDepthMask(GL_TRUE);
}

void Renderer::GenerateParticles(float dt, Vector3 position, int radius) {
	particleTime += dt;
		for (int x = 0; x < 10; x++)
		{
			if (particleIndex == MAX_PARTICLES - 1) {
				return;
			}
			particles->at(particleIndex++)->InitParticle(
				position + Vector3((rand() % radius) - radius / 2, 0, (rand() % radius) - radius / 2),
				Vector3((rand() % 50) - 25, -250 - rand() % 50, (rand() % 50) - 25),
				Vector3(0, -50, 0),
				particleLifetime,
				particleStartColour,
				particleEndColour,
				false,
				Vector3(50, 50, 50),
				Vector3(0,0,0),
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
	timePassed += dt;
	frameRate = 1 / dt;
	//std::cout << (float)1 / dt << " fps\n" << particleIndex << " particles\n";
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_0)) { delete heightMap; heightMap = new HeightMap(TEXTUREDIR"heightmap.png",true); }
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))particles->at(0)->modelMatrix = particles->at(0)->modelMatrix * Matrix4::Translation(Vector3(1, 1, 1));
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_2))particles->at(1)->modelMatrix = particles->at(1)->modelMatrix * Matrix4::Translation(Vector3(1, 1, 1));
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP))tesselationLevel = tesselationLevel == 32 ? 32 : tesselationLevel + 1;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN))tesselationLevel = tesselationLevel == 1 ? 1 : tesselationLevel-1;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP))blendFactor = blendFactor >= 1.0f ? 1.0f : blendFactor + 0.01f;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN))blendFactor = blendFactor <= 0.0f ? 0.0f : blendFactor-0.01f;
	
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	//std::cout << camera->GetPosition() << '\n';
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &(viewMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	GenerateParticles(dt, Vector3(4000, 500, 4000), particleRadius);
	UpdateParticles(dt);

	//std::cout << '\n';
	std::sort(particles->begin(), particles->begin() + particleIndex, [&](const Particle* a, const Particle* b) {
		Vector3 aPos = a->modelMatrix.GetPositionVector();
		Vector3 bPos = b->modelMatrix.GetPositionVector();
		Vector3 camPos = viewMatrix.GetPositionVector();
		return Vector3(aPos - camPos).Length() < Vector3(bPos - camPos).Length();
		});

	frameTime -= dt;

	//todo uncomment for spider animation
	/*while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % spiderAnim->GetFrameCount();
		frameTime += 1.0f / spiderAnim->GetFrameRate();
	}*/
	
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

void Renderer::RenderTrees() {
	BindShader(shader);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(treeMesh->modelMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	for (int i = 0; i < treeMesh->GetSubMeshCount(); i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, treeTextures[i]);
		treeMesh->DrawSubMesh(i);
	}
}

void Renderer::RenderSpiders() {
	BindShader(animShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	UpdateShaderMatrices();
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(spiderMesh->modelMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	std::vector<Matrix4> frameMatrices;
	const Matrix4* frameData = spiderAnim->GetJointData(currentFrame);
	const Matrix4* invBindPose = spiderMesh->GetInverseBindPose();
	for (int i = 0; i < spiderMesh->GetJointCount(); i++)
	{
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}
	glUniformMatrix4fv(glGetUniformLocation(animShader->GetProgram(), "joints"), frameMatrices.size(), false, (float*)frameMatrices.data());
	for (int i = 0; i < spiderMesh->GetSubMeshCount(); i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spiderTextures[i]);
		spiderMesh->DrawSubMesh(i);
	}
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	


	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(modelMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	DrawSkybox();
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

	
	//RenderTrees();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//RenderSpiders();

	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDisable(GL_CULL_FACE);
	RenderGrass();
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	RenderReflection();
	RenderParticles();


	RenderPortal();

	ImGui();
	

	return;

	//rendering grass on terrain
	BindShader(grassShader);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glUniform1i(glGetUniformLocation(grassShader->GetProgram(), "tesselationLevel"), tesselationLevel);
	glUniform1f(glGetUniformLocation(grassShader->GetProgram(), "time"), timePassed);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(modelMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &(viewMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(Matrix4), sizeof(Matrix4), &(projMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(heightMap->GetVAO());
	//glDrawArrays(GL_PATCHES, 0, heightMap->GetHeightMapSize().x * heightMap->GetHeightMapSize().z / 42);
	glDrawElements(GL_PATCHES, heightMap->GetHeightMapSize().x * heightMap->GetHeightMapSize().z / 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	
}

void Renderer::ImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("hi");
	ImGui::Text(("FPS: " + std::to_string(frameRate)).c_str());
	/*std::string cameraPosString = "Camera Position:";
	cameraPosString.append(" X " + std::to_string(camera->GetPosition().x));
	cameraPosString.append(" Y " + std::to_string(camera->GetPosition().y));
	cameraPosString.append(" X " + std::to_string(camera->GetPosition().z));*/
	//ImGui::Text(cameraPosString.c_str());
	ImGui::SliderFloat3("Camera Position", (float*)&(camera->position), 0, 10000);
	if (ImGui::TreeNode("Particles")) {
		ImGui::Text(("Particle Count: " + std::to_string(particleIndex)).c_str());
		ImGui::SliderFloat("Lifetime", &particleLifetime, 0, 10);
		ImGui::SliderInt("Radius", &particleRadius,0,1000);
		ImGui::ColorEdit4("Start Colour", (float*)&particleStartColour);
		ImGui::ColorEdit4("End Colour", (float*)&particleEndColour);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Grass")) {
		ImGui::SliderFloat3("Position", &(grassQuad->modelMatrix.values[12]), 0, 10000);
		ImGui::SliderInt("Tesselation", &tesselationLevel, 1, 32);
		ImGui::ColorEdit4("Start Colour", (float*)&grassStartColour);
		ImGui::ColorEdit4("End Colour", (float*)&grassEndColour);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Water")) {
		ImGui::Checkbox("Reflect Grass", &reflectGrass);
		ImGui::Checkbox("Reflect Particles", &reflectParticles);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Light")) {
		ImGui::SliderFloat3("Position", (float*)&light->position, 0, 10000);
		ImGui::SliderFloat("Radius", &light->radius, 0, 10000);
		ImGui::SliderFloat("Intensity", &light->intensity, 0, 10);
		ImGui::ColorEdit4("Diffuse Colour", (float*)&light->diffuseColour);
		ImGui::ColorEdit4("Specular Colour", (float*)&light->specularColour);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Portal")) {
		ImGui::SliderFloat3("Portal A", &(portalQuad->modelMatrix.values[12]), 0, 10000);
		ImGui::SliderFloat3("Portal B", &(portalViewpointQuad->modelMatrix.values[12]), 0, 10000);
		ImGui::TreePop();
	}


	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::RenderGrass() {
	BindShader(grassShader);
	glUniform1i(glGetUniformLocation(grassShader->GetProgram(), "noiseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, noiseTex);
	/*glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(grassQuad->modelMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(grassQuad->GetVAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	return;*/
	
	
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glUniform1i(glGetUniformLocation(grassShader->GetProgram(), "tesselationLevel"), tesselationLevel);
	glUniform1f(glGetUniformLocation(grassShader->GetProgram(), "time"), timePassed);
	glUniform4fv(glGetUniformLocation(grassShader->GetProgram(), "startColour"),1, (float*)&grassStartColour);
	glUniform4fv(glGetUniformLocation(grassShader->GetProgram(), "endColour"),1, (float*)&grassEndColour);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(grassQuad->modelMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &(viewMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(Matrix4), sizeof(Matrix4), &(projMatrix.values));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(grassQuad->GetVAO());
	//glDrawArrays(GL_PATCHES, 0, 4);
	glDrawElements(GL_PATCHES, 6,GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::SetupPortalFBOs() {
	glGenTextures(1, &portalColour);
	glBindTexture(GL_TEXTURE_2D, portalColour);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &portalDepth);
	glBindTexture(GL_TEXTURE_2D, portalDepth);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);


	glGenFramebuffers(1, &portalFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, portalFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, portalColour, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, portalDepth, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, portalDepth, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !reflectionColour || !reflectionDepth) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::SetupWaterFBOs() {
	//glColor4f(1, 1, 1, 1);

	glGenTextures(1, &reflectionColour);
	glBindTexture(GL_TEXTURE_2D, reflectionColour);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &reflectionDepth);
	glBindTexture(GL_TEXTURE_2D, reflectionDepth);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);


	glGenFramebuffers(1, &reflectionFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionColour, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, reflectionDepth, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, reflectionDepth, 0);

	glGenTextures(1, &refractionColour);
	glBindTexture(GL_TEXTURE_2D, refractionColour);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	
	glGenTextures(1, &refractionDepth);
	glBindTexture(GL_TEXTURE_2D, refractionDepth);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glGenFramebuffers(1, &refractionFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionColour, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, refractionDepth, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, refractionDepth, 0);
	


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !reflectionColour || !reflectionDepth) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	std::vector<Vector2> positions;
	positions.emplace_back(Vector2(-1, 0.5));
	positions.emplace_back(Vector2(-0.2, 0.5));
	positions.emplace_back(Vector2(-0.2, 1));
	positions.emplace_back(Vector2(-1, 1));
	
	guiMapQuad = Mesh::GenerateQuad(positions);

	
}

void Renderer::RenderPortal() {
	


	glBindFramebuffer(GL_FRAMEBUFFER, portalFBO);
	glViewport(0, 0, width, height);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, portalColour, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//BindShader(shader);
	//glUniform1i(glGetUniformLocation(shader->GetProgram(), "clipHeight"), waterHeight);
	//glUniform1i(glGetUniformLocation(shader->GetProgram(), "clipping"), -1);
	camera->SetPosition(camera->GetPosition() + Vector3(4000, 0, 0));
	viewMatrix = camera->BuildViewMatrix();

	DrawSkybox();
	BindShader(shader);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bumpMap);

	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(modelMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &(viewMatrix.values));

	heightMap->Draw();


	//RenderSpiders();
	//RenderReflection();
	RenderGrass();
	RenderParticles();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	camera->SetPosition(camera->GetPosition() - Vector3(4000, 0, 0));
	viewMatrix = camera->BuildViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &(viewMatrix.values));
	BindShader(portalShader);
	glUniform1i(glGetUniformLocation(portalShader->GetProgram(), "portalTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, portalColour);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(portalQuad->modelMatrix.values));
	portalQuad->Draw();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(portalViewpointQuad->modelMatrix.values));
	portalViewpointQuad->Draw();
	
}
void Renderer::RenderReflection() {
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
	glViewport(0, 0, width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionColour, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	BindShader(shader);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "clipHeight"), waterHeight);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "clipping"), -1);
	camera->GoUnderwater(waterHeight);
	viewMatrix = camera->BuildViewMatrix();
	
	DrawSkybox();
	BindShader(shader);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(modelMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &(viewMatrix.values));
	
	heightMap->Draw();
	
	//RenderSpiders();
	if(reflectGrass)RenderGrass();
	if(reflectParticles)RenderParticles();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	BindShader(shader);
	camera->GoUnderwater(waterHeight);
	viewMatrix = camera->BuildViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(modelMatrix.values));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &(viewMatrix.values));
	
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionColour, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, refractionDepth);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "clipHeight"), waterHeight);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "clipping"), 1);
	heightMap->Draw();
	
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "clipHeight"), -99999);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "clipping"), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	BindShader(guiShader);
	SetShaderLight(*light);
	glUniform1f(glGetUniformLocation(guiShader->GetProgram(), "blendFactor"), blendFactor);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &(waterQuad->modelMatrix.values));
	glUniform1i(glGetUniformLocation(guiShader->GetProgram(), "reflectTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionColour);
	glUniform1i(glGetUniformLocation(guiShader->GetProgram(), "refractTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionColour);
	glUniform1i(glGetUniformLocation(guiShader->GetProgram(), "waterTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glUniform1i(glGetUniformLocation(guiShader->GetProgram(), "waterNormal"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, waterNormal);
	glUniform1f(glGetUniformLocation(guiShader->GetProgram(), "time"), timePassed);
	waterQuad->Draw();
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


