#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	triangle = Mesh::GenerateTriangle();
	matrixShader = new Shader("matrixVertex.glsl", "colourFragment.glsl");
	if (!matrixShader->LoadSuccess())return;
	init = true;
	fov = 45.0f;
	camera = new Camera();
	SwitchToOrthographic();
}

Renderer::~Renderer(void) {
	delete triangle;
	delete matrixShader;
}

void Renderer::SwitchToPerspective(){
	std::cout << "persp\n";
	projMatrix = Matrix4::Perspective(1.0f,10000.0f,float(width) / height,fov);
}

void Renderer::SwitchToOrthographic() {
	std::cout << "ortho\n";
	projMatrix = Matrix4::Orthographic(1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}

void Renderer::RenderScene(){
	glClear(GL_COLOR_BUFFER_BIT);
	BindShader(matrixShader);
	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "projMatrix"), 1, false, projMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "viewMatrix"), 1, false, viewMatrix.values);
	Vector3 tempPos;
	for (int x = 0; x < 3; x++)
	{
		tempPos = position;
		tempPos.z += x * 500.0f;
		tempPos.x -= x * 100.0f;
		tempPos.y -= x * 100.0f;

		modelMatrix = Matrix4::Translation(tempPos) * Matrix4::Rotation(rotation, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(scale, scale, scale));
		glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);
		triangle->Draw();
	}
}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	OGLRenderer::UpdateShaderMatrices();
}