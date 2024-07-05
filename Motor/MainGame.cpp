#include "MainGame.h"
#include <iostream>
#include "Error.h"
#include <random>
#include <ctime>


const int max_calacas = 20;
int coldown_delete = 30;

using namespace std;

MainGame::MainGame() {
	width = 800;
	height = 600;
	gameState = GameState::PLAY;
	camera2D.init(width, height);
	srand(time(nullptr));
}

MainGame::~MainGame() {

}
void MainGame::processInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			/*cout << "Posicion del mousec " << event.motion.x << " " << event.motion.y << endl;
			inputManager.setMouseCoords(event.motion.x, event.motion.y);
			glm::vec2 mouseCoords = camera2D.convertToScreenWorld(inputManager.getMouseCoords());
			//cout << "Nueva posicion de acuerdo a camara " <<  mouseCoords.x
				//	<< " " << mouseCoords.y << endl;*/
			break;
		case SDL_KEYUP:
			inputManager.releaseKey(event.key.keysym.sym);
			break;
		case SDL_KEYDOWN:
			inputManager.pressKey(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.releaseKey(event.button.button);
			break;
		}
		handleInput();
	}

}

void MainGame::handleInput()
{
	const float SCALE_SPEED = 0.1f;
	const float CAMERA_SPEED = 4.0f;

	if (inputManager.isKeyDown(SDLK_UP)) {
		camera2D.setScale(camera2D.getScale() + SCALE_SPEED);
	}

	if (inputManager.isKeyDown(SDLK_DOWN)) {
		camera2D.setScale(camera2D.getScale() - SCALE_SPEED);
	}

	//camera2D.setPosition();

	if (inputManager.isKeyDown(SDLK_a)) {
		camera2D.setPosition(camera2D.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}

	if (inputManager.isKeyDown(SDLK_d)) {
		camera2D.setPosition(camera2D.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}

	if (inputManager.isKeyDown(SDLK_w)) {
		camera2D.setPosition(camera2D.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}

	if (inputManager.isKeyDown(SDLK_s)) {
		camera2D.setPosition(camera2D.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}

	// eliminar calacas

	if (inputManager.isKeyDown(SDLK_e) && (coldown_delete <= 0 )) { // eliminar 1 calaca
		if (calacas.size() > 0) {
			delete calacas[calacas.size() - 1];
			calacas.pop_back();
		}
		coldown_delete = 20;
	}
	else if (inputManager.isKeyDown(SDLK_f) && (coldown_delete <= 0)) { // eliminar 2 calacas
		if (calacas.size() > 1) {
			delete calacas[calacas.size() - 1];
			calacas.pop_back();
			delete calacas[calacas.size() - 1];
			calacas.pop_back();
		}
		else if (calacas.size() > 0) {
			delete calacas[calacas.size() - 1];
			calacas.pop_back();
		}
		coldown_delete = 20;
	}
	else {
		if (coldown_delete > 0) {
			coldown_delete--;
		}
	}

}



void MainGame::initShaders()
{
	program.compileShaders("Shaders/colorShaderVert.txt", "Shaders/colorShaderFrag.txt");
	program.addAtribute("vertexPosition");
	program.addAtribute("vertexColor");
	program.addAtribute("vertexUV");
	program.linkShader();
}

void MainGame::init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window.create("Mundo 1", width, height, 0);
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Glew not initialized");
	}
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	initLevel();
	initShaders();
}

void MainGame::initLevel() {
	levels.push_back(new Level("Level/level1.txt"));
	currentLevel = 0;
	spriteBatch.init();
	std::mt19937 ramdomEngie(time(nullptr));

	std::uniform_int_distribution<int>randPosX(
		1, levels[currentLevel]->getWidth() - 2
	);
	std::uniform_int_distribution<int>randPoxY(
		1, levels[currentLevel]->getHeight() - 2
	);

	timeForNextCalaca = 100; // 500 frames
}

void MainGame::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program.use();
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 cameraMatrix = camera2D.getCameraMatrix();
	GLuint pCameraLocation = program.getUniformLocation("pCamera");
	glUniformMatrix4fv(pCameraLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	GLuint imageLocation = program.getUniformLocation("myImage");
	glUniform1i(imageLocation, 0);
	spriteBatch.begin();

	levels[currentLevel]->draw();

	for (size_t i = 0; i < calacas.size(); i++) {
		calacas[i]->draw();
	}

	spriteBatch.end();
	spriteBatch.renderBatch();
	drawHud();
	glBindTexture(GL_TEXTURE_2D, 0);
	program.unuse();
	window.swapWindow();

	
}

void MainGame::drawHud()
{
	
}

void MainGame::run() {
	init();
	update();
}

void MainGame::updateElements() {
	
	camera2D.update();
}

void MainGame::update() {
	while (gameState != GameState::EXIT) {
		draw();
		processInput();
		updateElements();

		if (timeForNextCalaca > 0) {
			timeForNextCalaca--;
		}
		else if (calacas.size() >= max_calacas) {
			timeForNextCalaca = 100;
		}
		else {
			timeForNextCalaca = 100;
			
			int randTexture = rand() % 10 + 1;
			calacas.push_back(new Calaca());
			glm::vec2 pos = levels[currentLevel]->getPlayerPosition();
			calacas.back()->init(2.0f, pos);
			cout << randTexture << endl;
			calacas.back()->setRandomSprite(randTexture);
		}

		for (size_t i = 0; i < calacas.size(); i++) {
			calacas[i]->update(levels[currentLevel]->getLevelData());
		}
	}

}

void MainGame::reset() {
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	levels.clear();
	currentLevel = 0;
}
