#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/CubeRobot.h"
class Renderer :
    public OGLRenderer
{
public:
    Renderer(Window& parent);
    ~Renderer(void);
    void RenderScene() override;
    void UpdateScene(float dt) override;
protected:
    void PresentScene();
    void DrawPostProcess();
    void DrawScene();
    void DrawNode(SceneNode* n);

    void PresentSceneDoubleVision();
    void DrawPostProcessDoubleVision();
    void DrawSceneDoubleVision();

    Shader* sceneShader;
    Shader* processShader;
    Shader* doubleVisionShader;
    Camera* camera;
    Camera* birdsEyeCamera;
    Mesh* quad;
    HeightMap* heightMap;
    GLuint heightTexture;
    GLuint bufferFBO;
    GLuint processFBO;
    GLuint bufferColourTex[2];
    GLuint64 bufferColourTexHandle[2];
    GLuint bufferDepthTex;
    SceneNode* robot;
    Mesh* cube;
};

