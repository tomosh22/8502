#pragma once
#include "../nclgl/OGLRenderer.h"
#include <array>
#include <stack>
class HeightMap;
class Camera;
class Light;
class Shader;
class Particle;
class MeshMaterial;
class MeshAnimation;

#define MAX_PARTICLES 100000
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

    std::array<Particle*,MAX_PARTICLES>* particles;
    std::stack<Particle*>* aliveParticles;
    std::stack<Particle*>* deadParticles;
    int particleIndex;
    GLuint particleTexture;
    Shader* particleShader;

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
    Shader* guiShader;
    Mesh* waterQuad;
    float blendFactor;
    int waterHeight;

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
};

