#pragma once
#include "SceneNode.h"
#include <map>
#include <string>
class CubeRobot :
    public SceneNode
{
public:
    CubeRobot(Mesh* cube, std::map<std::string,GLuint64> textures);
    ~CubeRobot(void) {};
    void Update(float dt) override;
protected:
    SceneNode* head;
    SceneNode* leftArm;
    SceneNode* rightArm;
};

