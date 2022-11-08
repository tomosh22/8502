#pragma once
#include "Mesh.h"
class Vector3;



class Particle :
    public Mesh
{
public:
    Particle();
    Particle(
        const Vector3& position,
        const Vector3& velocity,
        const Vector3& gravity,
        const float& duration,
        const Vector4& startColour,
        const Vector4& endColour,
        const bool& constColour,
        const Vector3& startScale,
        const Vector3& endScale,
        const bool& constScale);
    ~Particle();
    bool UpdateParticle(float dt);
    Vector4 GetColour() { return colour; }
    Vector3 GetScale() { return scale; }
    GLuint GetVAO() { return arrayObject; }
protected:
    GLuint CreateVBO();
    Vector3 velocity;
    Vector3 gravity;
    float duration;
    Vector4 startColour;
    Vector4 endColour;
    bool constColour;
    Vector4 colour;
    float elapsed;
    Vector3 startScale;
    Vector3 endScale;
    bool constScale;
    Vector3 scale;
};

