#pragma once
#include "Mesh.h"
class Particle :
    public Mesh
{
public:
    Particle();
    ~Particle();
protected:
    GLuint texture;

};

