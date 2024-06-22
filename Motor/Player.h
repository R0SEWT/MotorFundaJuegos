#pragma once
#include "Human.h"
#include "InputManager.h"


enum direction {
    TOP,
    DOWN,
    LEFT,
    RIGHT
};

class Player :
    public Human
{
private:
    InputManager* inputmanager;
    int shotColdown;
    int currentShotColdown;
    int timeForShot;
    bool shot;
    int direction;
public:
    Player();
    ~Player();
    void init(float speed, glm::vec2 position, InputManager* inputManager, int shotColdown);
    void update(vector<string>& levelData,
        vector<Human*>& humans,
        vector<Zombie*>& zombies);
    bool shotReady();
    void resetCDShot();
    glm::vec2 getDirection();

    void updateShotColdown(float camScale);
};

