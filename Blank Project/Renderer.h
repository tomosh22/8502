#pragma once
#include "../nclgl/OGLRenderer.h"
#include <array>
#include <stack>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/MeshMaterial.h"
class HeightMap;
class Camera;
class Light;
class Shader;
class Particle;
class MeshMaterial;
class MeshAnimation;

#define MAX_PARTICLES 10000

struct CameraState {
	Vector3 pos;
	float pitch;
	float yaw;
};

class Renderer :
	public OGLRenderer
{
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene() override;
	void UpdateScene(float dt) override;

	void RenderParticle(Particle* p);
	void RenderParticles();
	void UpdateParticles(float dt);
	void GenerateParticles(float dt, Vector3 position, int radius);
	Matrix4 GenerateTransposedMatrix(Particle* p);

	void RenderGrass();

	void SetupWaterFBOs();
	void RenderReflection();

	void RenderTrees();
	void RenderSpiders();

	void DrawSkybox();

	void ImGui();

	void SetupPortalFBOs();
	void RenderPortal();

	void FillBuffers();
	void DrawPointLights();
	void DrawDirectionalLights();
	void CombineBuffers();
	void GenerateScreenTexture(GLuint& into, bool depth = false);

	void DrawNode(SceneNode* n);

	void CameraRoutine(float dt, CameraState state,float time);
protected:
	float frameRate;

	HeightMap* heightMap;

	Shader* shader;
	Camera* camera;
	Light* light;
	float lightIntensity;
	Vector4 lightDiffuseColour;
	Vector4 lightSpecularColour;
	int lightRadius;
	GLuint texture0;
	GLuint texture1;
	GLuint texture2;
	GLuint bumpMap0;
	GLuint bumpMap1;
	GLuint bumpMap2;
	GLuint splatMap;

	GLuint matrixUBO;
	GLuint columnUBO;

	GLuint textureUBO;
	GLuint textureBind;

	std::array<Particle*, MAX_PARTICLES>* particles;
	std::stack<Particle*>* aliveParticles;
	std::stack<Particle*>* deadParticles;
	int particleIndex;
	GLuint particleTexture;
	Shader* particleShader;
	float particleLifetime;
	Vector4 particleStartColour;
	Vector4 particleEndColour;
	int particleRadius;

	float particleTime;

	//Matrix4 modelViewMatrices[MAX_PARTICLES];
	//Vector3 scales[MAX_PARTICLES];
	//Vector4 colours[MAX_PARTICLES];

	GLuint vbo1;
	GLuint vbo3;
	GLuint vbo4;
	GLuint vbo5;
	GLuint vbo6;
	GLuint vbo7;

	Mesh* masterParticle;
	std::array<std::array<float, 4>, MAX_PARTICLES>* column0s;
	std::array<std::array<float, 4>, MAX_PARTICLES>* column1s;
	std::array<std::array<float, 4>, MAX_PARTICLES>* column2s;
	std::array<std::array<float, 4>, MAX_PARTICLES>* column3s;
	std::array<std::array<float, 4>, MAX_PARTICLES>* coloursGPU;

	Shader* grassShader;
	Mesh* grassQuad;
	GLuint noiseTex;
	Vector4 grassStartColour;
	Vector4 grassEndColour;

	int tesselationLevel;

	float timePassed;


	GLuint reflectionFBO;
	GLuint reflectionColour;
	GLuint reflectionDepth;

	GLuint refractionFBO;
	GLuint refractionColour;
	GLuint refractionDepth;
	GLuint refractionDepthTex;

	Mesh* guiMapQuad;
	Shader* waterShader;
	Mesh* waterQuad;
	float blendFactor;
	int waterHeight;
	GLuint waterTex;
	GLuint waterNormal;
	bool reflectGrass;
	bool reflectParticles;

	Mesh* treeMesh;
	MeshMaterial* treeMat;
	std::vector<GLuint> treeTextures;

	Mesh* spiderMesh;
	MeshMaterial* spiderMat;
	std::vector<GLuint> spiderTextures;
	MeshAnimation* spiderAnim;
	int currentFrame;
	float frameTime;

	Shader* animShader;

	GLuint cubeMap;
	Mesh* skyboxQuad;
	Shader* skyboxShader;

	GLuint portalFBO;
	GLuint portalColour;
	GLuint portalDepth;
	Mesh* portalQuad;
	Mesh* portalViewpointQuad;
	Shader* portalShader;

	Shader* pointLightShader;
	Shader* combineShader;

	GLuint bufferFBO;
	GLuint bufferColourTex;
	GLuint bufferNormalTex;
	GLuint bufferDepthTex;

	GLuint pointLightFBO;
	GLuint lightDiffuseTex;
	GLuint lightSpecularTex;

	Light* pointLights;
	Mesh* sphere;
	Mesh* deferQuad;

	SceneNode* root;
	SceneNode* roleT;
	Shader* skinningShader;
	SceneNode* testCube;

	Vector3 lightDir;
	Vector4 lightDiffuse;
	Vector4 lightSpecular;

	Vector4 fogColour;
	bool drawNormals;

	bool freeCam;
	int currentCheckpoint;
	int checkpoints;
	CameraState* cameraStates;
	bool destChosen;
	float cameraTime;
	CameraState* startState;
	CameraState* endState;
};

