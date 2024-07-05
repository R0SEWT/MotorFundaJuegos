#pragma once
#include "Agent.h"
#include "ResourceManager.h"
#include "Level.h"


class Calaca :
    public Agent
{
private:
    glm::vec2 direction;
public:
    Calaca();
    ~Calaca();
    void init(float speed, glm::vec2 position);
    void update(vector<string>& levelData);
    virtual void draw() override;
    void setRandomSprite(int randTexture);

};

