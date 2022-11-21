#pragma once
#include "SceneNode.h"
#include <map>
#include <string>
class CubeRobot :
    public SceneNode
{
public:
    CubeRobot(GLuint matrixUBO, Mesh* cube, Vector3 pos = Vector3(0, 0, 0));
    ~CubeRobot(void) {};
    void Update(float dt) override;
protected:
    SceneNode* head;
    SceneNode* leftArm;
    SceneNode* rightArm;
    Vector3 pos;
};

