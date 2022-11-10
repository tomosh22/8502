#pragma once
#include "Mesh.h"
class Vector3;



class Particle 
    
{
public:
    
    Particle() {
        alive = false; duration = 0; elapsed = 0; oneFramePassed = false; twoFramesPassed = false;
    };
    void InitParticle(
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
    ~Particle() {};
    bool UpdateParticle(float dt);
    Vector4 GetColour() { return colour; }
    Vector3 GetScale() { return scale; }
    Matrix4 modelMatrix;
    bool alive;
    Vector4 colour;
    Vector4 startColour;
    float elapsed;
    float duration;
    bool oneFramePassed;
    bool twoFramesPassed;
protected:
    Vector3 velocity;
    Vector3 gravity;
    
    
    Vector4 endColour;
    bool constColour;
    
    
    Vector3 startScale;
    Vector3 endScale;
    bool constScale;
    Vector3 scale;
    
    
};

