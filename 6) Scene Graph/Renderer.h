#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/CubeRobot.h"
#include <map>
#include <string>
class Renderer :
    public OGLRenderer
{
public:
    Renderer(Window& parent);
    ~Renderer(void);

    void UpdateScene(float dt)override;
    void RenderScene()override;
protected:
    void DrawNode(SceneNode* n);
    SceneNode* root;
    Camera* camera;
    Mesh* cube;
    Shader* shader;
    GLuint texture0;
    GLuint64 textureHandle0;
    GLuint texture1;
    GLuint64 textureHandle1;
    GLuint texture2;
    GLuint64 textureHandle2;
    std::map<std::string, GLuint64> textures;
};

