#pragma once
#include <SDL/SDL.h>
#include<GL/eglew.h>
#include <iostream>
#include <vector>
#include<cmath>
#include<memory>
#include "Sprite.h"
#include"GLS_Program.h"
#include "Window.h"
#include <map>

using namespace std;

enum class GameState {
	PLAY, EXIT
};

class MainGame {

private:
	// vector<Sprite> sprites   <-----  Se bugea y solo me muestra el ultimo en ser inicializado :c
	Sprite sprite;
	Sprite sprite1;
	Sprite sprite2;
	Sprite sprite3;

	int width;
	int height;
	Window* window;
	void init();
	void processInput();
	void initShaders();
	GLS_Program program;
	float time = 0;
	int cont = 1;

public:
	MainGame();
	~MainGame();
	GameState gameState;
	void run();
	void draw();
	void update();


};
