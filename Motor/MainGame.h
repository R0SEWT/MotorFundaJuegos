#pragma once
#include <SDL/SDL.h>
#include <GL/eglew.h>
#include "Sprite.h"
#include "GLS_Program.h"
#include "Window.h"
#include "Camera2D.h"
#include "InputManager.h"
#include "Level.h"
#include "Player.h"
#include "Human.h"
#include "Zombie.h"
#include "SpriteBatch.h"
#include "Spawner.h"
#include "Bullet.h"
#include "Quadtree.h"

enum class GameState {
	PLAY, EXIT
};

class MainGame
{
private:
	int width;
	int height;
	int lives;
	SpriteBatch spriteBatch;
	SpriteBatch hudBatch;
	
	vector<Level*> levels;
	vector<Human*> humans;
	vector<Zombie*> zombies;
	vector<Spawner*> spawns;
	vector<Bullet*> bullets;
	Player* player;
	double alphaReduce;
	double alphaReduceTotal;
	int currentLevel;
	Window window;
	GLS_Program program;
	Sprite sprite;
	Camera2D camera2D;
	InputManager inputManager;
	Quadtree* quadtree;

	void init();
	void initLevel(int currentLevel, bool resetPlayer = false);
	void processInput();
	void initShaders();
	void handleInput();
	void updateElements();
	void createBullet(glm::vec2 direction);
	void passLevel();
	void resetLevel();
	void moveAndCollide();
	int getlives();
	void DecreaseLives();
	void hackChangeLvl();
	void updateQuadtree();
public:
	MainGame();
	~MainGame();
	GameState gameState;

	void run();
	void draw();
	void drawHud();
	void update();
	void reset();

};

