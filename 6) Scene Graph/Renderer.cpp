#include "Renderer.h"
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	cube = Mesh::LoadFromMeshFile("OffsetCubeY.msh");
	camera = new Camera();
	shader = new Shader("sceneVertex.glsl", "sceneFragment.glsl");
	if (!shader->LoadSuccess()) {
		return;
	}
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, float(width) / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 30, 175));

	texture0 = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	textureHandle0 = glGetTextureHandleARB(texture0);
	glMakeTextureHandleResidentARB(textureHandle0);

	texture1 = SOIL_load_OGL_texture(TEXTUREDIR"stainedglass.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	textureHandle1 = glGetTextureHandleARB(texture1);
	glMakeTextureHandleResidentARB(textureHandle1);

	texture2 = SOIL_load_OGL_texture(TEXTUREDIR"doge.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	textureHandle2 = glGetTextureHandleARB(texture2);
	glMakeTextureHandleResidentARB(textureHandle2);

	textures.insert({ "stone",textureHandle0 });
	textures.insert({ "glass",textureHandle1 });
	textures.insert({ "doge",textureHandle2 });
	
	root = new SceneNode((Mesh*)NULL,Vector4(1,1,1,1));
	
	
	root->AddChild(new CubeRobot(cube));
	std::cout << "after\n";

	glEnable(GL_DEPTH_TEST);
	init = true;
}

Renderer::~Renderer(void) {
	delete root;
	delete shader;
	delete camera;
	delete cube;
}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(dt);
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(shader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"),1);
	DrawNode(root);
}

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, model.values);
		glUniform4fv(glGetUniformLocation(shader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "useTexture"), 1);
		//glUniformHandleui64ARB(glGetUniformLocation(this->shader->GetProgram(), "diffuseTex"));
		n->Draw(*this);
	}
	for (vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); i++) {
		DrawNode(*i);
	}
}