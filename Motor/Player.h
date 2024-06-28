#pragma once
#include "Human.h"
#include "InputManager.h"
#include "algorithm"


enum direction {
    TOP,
    DOWN,
    LEFT,
    RIGHT
};

class Player :
    public Agent
{
private:
    InputManager* inputmanager;
    float shotColdown;
    int currentShotColdown;
    int timeForShot;
    int direction;
    bool godMode;
    pair <int, int> aux_speed_coldown;
public:
    Player();
    ~Player();
    void init(float speed, glm::vec2 position, InputManager* inputManager, float shotColdown);
    void update(vector<string>& levelData,
        vector<Human*>& humans,
        vector<Zombie*>& zombies);
    bool shotReady();
    void resetCDShot();
    glm::vec2 getDirection();

    void updateShotColdown(float camScale);
    void infoCD();
    void GodMode();
};

