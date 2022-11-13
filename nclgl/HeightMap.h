#pragma once
#include "Mesh.h"
#include <string>
#define DB_PERLIN_IMPL
#include "PerlinNoise.hpp"

class HeightMap :
    public Mesh
{
public:
    HeightMap(const std::string& name, bool perlin);
    HeightMap();
    ~HeightMap(void) {};
    Vector3 GetHeightMapSize() const { return heightMapSize; }
protected:
    Vector3 heightMapSize;
};

