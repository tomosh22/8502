#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/CubeRobot.h"
class HeightMap;
class Camera;
class Renderer :
    public OGLRenderer
{
public:
    Renderer(Window& parent);
    ~Renderer(void);
    void RenderScene() override;
    void UpdateScene(float dt) override;
    void DrawNode(SceneNode* n);
protected:
    HeightMap* heightMap;
    Shader* shader;
    Camera* camera;
    GLuint terrainTex;
};

