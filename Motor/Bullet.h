#pragma once
#include "Agent.h"
class Bullet :
    public Agent
{
private:
    glm::vec2 direction;

public:
    Bullet();
    ~Bullet();
    void init(glm::vec2 position, glm::vec2 direction, float speed);
    void draw();
    void update();
    bool iSForDestroy(vector<string>& levelData);

};

