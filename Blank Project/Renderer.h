#pragma once
#include "../nclgl/OGLRenderer.h"
class HeightMap;
class Camera;
class Light;
class Shader;
class Particle;

#define MAX_PARTICLES 1000
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
    
protected:
    HeightMap* heightMap;
    Shader* shader;
    Camera* camera;
    Light* light;
    GLuint texture;
    GLuint bumpMap;

    GLuint matrixUBO;
    GLuint columnUBO;

    GLuint textureUBO;
    GLuint textureBind;

    std::vector<Particle*> particles;
    GLuint particleTexture;
    Shader* particleShader;

    float particleTime;

    Matrix4 modelViewMatrices[MAX_PARTICLES];
    //Vector3 scales[MAX_PARTICLES];
    Vector4 colours[MAX_PARTICLES];

    GLuint vbo1;
    GLuint vbo3;
    GLuint vbo4;
    GLuint vbo5;
    GLuint vbo6;
    GLuint vbo7;

    Particle* masterParticle;
};

