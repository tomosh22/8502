#pragma once
#include "../nclgl/OGLRenderer.h"
#include <array>
#include <stack>
#include "../nclgl/HeightMap.h"
#include "../nclgl/Camera.h"
#include "../nclgl/Light.h"
class HeightMap;
class Camera;
class Light;
class Shader;

#define MAX_PARTICLES 10000
class Renderer :
    public OGLRenderer
{
public:
    Renderer(Window& parent);
    ~Renderer(void) {};

    void RenderScene() override;
    void UpdateScene(float dt) override;

    //void RenderParticle(Particle* p);
    void RenderParticles();
    void UpdateParticles(float dt);
    void GenerateParticles(float dt, Vector3 position, int radius);

    void RenderGrass();

    void SetupWaterFBOs();
    void RenderReflection();

    void RenderTrees();
    void RenderSpiders();

    void DrawSkybox();

    

    void SetupPortalFBOs();
    void RenderPortal();

    void FillBuffers();
    void DrawPointLights();
    void CombineBuffers();
    void GenerateScreenTexture(GLuint& into, bool depth = false);
protected:
    float frameRate;

    
    Shader* shader;
    
    Light* light;
    float lightIntensity;
    Vector4 lightDiffuseColour;
    Vector4 lightSpecularColour;
    int lightRadius;
    GLuint texture;
    GLuint bumpMap;

    GLuint matrixUBO;
    GLuint columnUBO;

    GLuint textureUBO;
    GLuint textureBind;

    
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

    Shader* sceneShader; // Shader to fill our GBuffers
     Shader * pointlightShader; // Shader to calculate lighting
     Shader * combineShader; // shader to stick it all together
    
      GLuint bufferFBO; //FBO for our G-Buffer pass
     GLuint bufferColourTex; // Albedo goes here
     GLuint bufferNormalTex; // Normals go here
     GLuint bufferDepthTex; // Depth goes here
    
      GLuint pointLightFBO; //FBO for our lighting pass
     GLuint lightDiffuseTex; // Store diffuse lighting
     GLuint lightSpecularTex;
     HeightMap* heightMap; // Terrain!
     Light * pointLights; // Array of lighting data
     Mesh * sphere; // Light volume
     Mesh * quad; //To draw a full -screen quad
     Camera * camera; //Our usual camera
     GLuint earthTex;
     GLuint earthBump;


};

