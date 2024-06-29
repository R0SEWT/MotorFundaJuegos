#pragma once
#include "Agent.h"

class Human : 
    public Agent
{
private:
    glm::vec2 direction;
public:
    Human();
    ~Human();
    void init(float speed, glm::vec2 position, int raza);
    void update(vector<string>& levelData,
        vector<Human*>& humans,
        vector<glm::vec2>& zombies);
    void scapeToZombie(vector<glm::vec2>& zombies);

    //funcionm para sobrescrivir draw
    virtual void draw() override;

};

