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
#include "Calaca.h"

enum class GameState {
	PLAY, EXIT
};

class MainGame
{
private:
	int width;
	int height;
	SpriteBatch spriteBatch;
	SpriteBatch hudBatch;
	vector<Level*> levels;
	vector<Calaca*> calacas;

	int timeForNextCalaca;
	double alphaReduce;
	double alphaReduceTotal;
	int currentLevel;
	Window window;
	GLS_Program program;
	Sprite sprite;
	Camera2D camera2D;
	InputManager inputManager;
	void init();
	void initLevel();
	void processInput();
	void initShaders();
	void handleInput();
	void updateElements();
	void createBullet();

	void createRandomCalaca();

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

