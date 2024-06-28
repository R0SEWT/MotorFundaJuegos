#pragma once
#include "Agent.h"
class Spawner :
    public Agent
{
private:
    int timeGeneration;
    int timeForZombie;
    bool spawn;
    int life;
public:
    Spawner();
    ~Spawner();
    void update();
    void init(int timeGeneration, glm::vec2 position, int life);
    void checkSpawnZombie(vector<Zombie*>& zombies, float speed);

    void loseLife() { life--;}
    int getLife() { return life; }
};

