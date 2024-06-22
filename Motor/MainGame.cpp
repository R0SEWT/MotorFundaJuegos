#include "MainGame.h"
#include <iostream>
#include "Error.h"
#include <random>
#include <ctime>

using namespace std;

MainGame::MainGame() {
	width = 800;
	height = 600;
	gameState = GameState::PLAY;
	camera2D.init(width, height);
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
	if (inputManager.isKeyPressed(SDLK_q)) {
		camera2D.setScale(camera2D.getScale() + SCALE_SPEED);
	}

	if (inputManager.isKeyPressed(SDLK_e)) {
		camera2D.setScale(camera2D.getScale() - SCALE_SPEED);
	}
	if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		//cout << "CLICK IZQUIERDO" << endl;
		createBullet();
	}

	if (inputManager.isKeyPressed(SDL_BUTTON_RIGHT)) {
		//cout << "CLICK DERECHo" << endl;
	}

	if (inputManager.isKeyPressed(SDL_BUTTON_MIDDLE)) {
		//cout << "CLICK CENTRO" << endl;
	}
}

void MainGame::createBullet() {
	
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
	player = new Player();
	player->init(10.0f, levels[currentLevel]->getPlayerPosition(),
		&inputManager, 10);
	std::mt19937 ramdomEngie(time(nullptr));

	std::uniform_int_distribution<int>randPosX(
		1, levels[currentLevel]->getWidth() - 2
	);
	std::uniform_int_distribution<int>randPoxY(
		1, levels[currentLevel]->getHeight() - 2
	);
	for (size_t i = 0; i < levels[currentLevel]->getNumHumans(); i++)
	{
		humans.push_back(new Human);
		glm::vec2 pos(randPosX(ramdomEngie) * TILE_WIDTH,
			randPoxY(ramdomEngie) * TILE_WIDTH);
		humans.back()->init(3.0f, pos);
	}
	for (auto pos_z : levels[currentLevel]->getZombiesPosition())
	{
		zombies.push_back(new Zombie);
		glm::vec2 pos();
		zombies.back()->init(2.0f, pos_z);
	}

	for (auto pos_s : levels[currentLevel]->getSpawnersPosition()) {
		spawns.push_back(new Spawner);
		glm::vec2 pos();
		spawns.back()->init(100, pos_s);
	}


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
	player->draw(spriteBatch);
	for (size_t i = 0; i < humans.size(); i++)
	{
		humans[i]->draw(spriteBatch);
	}
	for (auto &z : zombies)
	{
		z->draw(spriteBatch);
	}
	for (auto &s : spawns) {
		s->draw(spriteBatch);
		s->checkSpawnZombie(zombies, 2.0f);
	}
	for (auto& b : bullets) {
		b->draw(spriteBatch);
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
	camera2D.setPosition(player->getPosition());
}

void MainGame::update() {
	while (gameState != GameState::EXIT) {
		draw();
		processInput();
		updateElements();
		player->update(levels[currentLevel]->getLevelData(), humans, zombies);

		if (player->getShot()) { // disparo con cooldown
			bullets.push_back(new Bullet);
			bullets.back()->init(player->getPosition(), player->getDirection(), 2.0f);
			player->resetCDShot();
		}

		for (auto &h : humans)
		{
			h->update(levels[currentLevel]->getLevelData(), humans, zombies);
		}
		for (size_t i = 0; i < zombies.size(); i++)
		{
			zombies[i]->update(levels[currentLevel]->getLevelData(), humans, zombies);
			for (size_t j = 0; j < humans.size(); j++)
			{
				if (zombies[i]->collideWithAgent(humans[j])) {
					zombies.push_back(new Zombie);
					zombies.back()->init(2.0f, humans[j]->getPosition());
					delete humans[j];
					humans[j] = humans.back();
					humans.pop_back();

				}
			}
			for (size_t j = 0; j < bullets.size(); j++)
			{
				if (zombies[i]->collideWithAgent(bullets[j])) { // fixear eliiminaciones
					delete zombies[i];
					zombies[i] = zombies.back();
					zombies.pop_back();

					delete bullets[j];
					bullets[j] = bullets.back();
					bullets.pop_back();
					break;
				}
			}	
		}
		for (auto& s : spawns) {
			s->update();
		}
		for (auto& b : bullets) {
			b->update(levels[currentLevel]->getLevelData());
		}
	}

}

void MainGame::reset() {
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	for (size_t i = 0; i < zombies.size(); i++)
	{
		delete zombies[i];
	}
	zombies.clear();
	for (size_t i = 0; i < humans.size(); i++)
	{
		delete humans[i];
	}
	humans.clear();
	delete player;
	levels.clear();
	currentLevel = 0;
}
