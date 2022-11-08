#pragma once
#include "../nclgl/OGLRenderer.h"
class HeightMap;
class Camera;
class Light;
class Shader;

struct ParticleInfo {
    Mesh* quad;
    Matrix4 modelMatrix;
};

class Renderer :
    public OGLRenderer
{
public:
    Renderer(Window& parent);
    ~Renderer(void);

    void RenderScene() override;
    void UpdateScene(float dt) override;
protected:
    HeightMap* heightMap;
    Shader* shader;
    Camera* camera;
    Light* light;
    GLuint texture;
    GLuint bumpMap;

    GLuint matrixUBO;
    GLuint matrixBind;

    GLuint textureUBO;
    GLuint textureBind;

    std::vector<ParticleInfo> particles;
    GLuint particleTexture;
    Shader* particleShader;
};

