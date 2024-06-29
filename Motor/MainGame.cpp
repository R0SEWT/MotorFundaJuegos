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
	lives = 3;
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
		if (event.type == SDL_MOUSEMOTION) {
			inputManager.setMouseCoords(event.motion.x, event.motion.y);
		}
		handleInput();
	}

}

void MainGame::handleInput()
{
	const float SCALE_SPEED = 0.01f;


	if (inputManager.isKeyDown(SDLK_q)) {
		if (camera2D.getScale() < 5000.0f) {
			camera2D.setScale(camera2D.getScale() + SCALE_SPEED);
		}
	}

	if (inputManager.isKeyDown(SDLK_e)) {
		if (camera2D.getScale() > 0.6f) {
			camera2D.setScale(camera2D.getScale() - SCALE_SPEED);
		}
	}
	if (inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		//cout << "CLICK IZQUIERDO" << endl;

		glm::vec2 coords = camera2D.convertToScreenWorld(inputManager.getMouseCoords()); // coords relativas
		glm::vec2 direction = glm::normalize(coords - player->getPosition());
		createBullet(direction);

	}

	if (inputManager.isKeyPressed(SDL_BUTTON_RIGHT)) {
		//cout << "CLICK DERECHo" << endl;
	}

	if (inputManager.isKeyPressed(SDL_BUTTON_MIDDLE)) {
		//cout << "CLICK CENTRO" << endl;
	}

	hackChangeLvl();
}

void MainGame::hackChangeLvl() {
	if (inputManager.isKeyDown(SDLK_1)) {
		currentLevel = 0;
		currentLevel = --currentLevel;
		passLevel();
	}
	if (inputManager.isKeyDown(SDLK_2)) {
		currentLevel = 1;
		currentLevel = --currentLevel % levels.size();
		passLevel();
	}
	if (inputManager.isKeyDown(SDLK_3)) {
		currentLevel = 2;
		currentLevel = --currentLevel % levels.size();
		passLevel();
	}
	if (inputManager.isKeyDown(SDLK_4)) {
		currentLevel = 3;
		currentLevel = --currentLevel % levels.size();
		passLevel();
	}
	if (inputManager.isKeyDown(SDLK_5)) {
		currentLevel = 4;
		currentLevel = --currentLevel % levels.size();
		passLevel();
	}

	
}

void MainGame::createBullet(glm::vec2 direction) {
	if(!player->shotReady()) return;
	//player->infoCD();
	// A mas amplitud de camara, mayor el coldown
	player->updateShotColdown(camera2D.getScale());
	player->resetCDShot();
	glm::vec2 pos = player->getPosition();
	Bullet *bullet = new Bullet();
	bullet->init(pos, direction, 8.0f);
	bullets.push_back(bullet);
	
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
	window.create("Zoombies", width, height, 0);
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Glew not initialized");
	}
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // RGB para color celeste claro
	initLevel(0, 0);
	initShaders();
}

void MainGame::initLevel(int currentLevel, bool resetPlayer) {
	spriteBatch.init();
	levels.push_back(new Level("Level/level1.txt"));
	levels.push_back(new Level("Level/level2.txt"));
	levels.push_back(new Level("Level/level3.txt"));
	levels.push_back(new Level("Level/level4.txt"));
	levels.push_back(new Level("Level/level5.txt"));

	// init player
	player = new Player();
	float speed = 10.0f;
	float shotColdown = 10.0f;
	player->init(speed, levels[currentLevel]->getPlayerPosition(),
		&inputManager, shotColdown);
	
	// init humans
	speed = 3.0f;
	std::mt19937 ramdomEngie(time(nullptr));

	std::uniform_int_distribution<int>randPosX(
		1, levels[currentLevel]->getWidth() - 2
	);
	std::uniform_int_distribution<int>randPoxY(
		1, levels[currentLevel]->getHeight() - 2
	);
	int raza = rand();
	for (size_t i = 0; i < levels[currentLevel]->getNumHumans(); i++)
	{
		humans.push_back(new Human);
		glm::vec2 pos(randPosX(ramdomEngie) * TILE_WIDTH,
			randPoxY(ramdomEngie) * TILE_WIDTH);
		humans.back()->init(speed, pos, (raza++)%1000);
	}
	
	// init zombies
	speed = 2.0f;
	for (auto &pos_z : levels[currentLevel]->getZombiesPosition())
	{
		zombies.push_back(new Zombie);
		zombies.back()->init(speed, pos_z);
	}

	// init spawners
	int time_generation = 150;
	int life = 10;

	for (auto &pos_s : levels[currentLevel]->getSpawnersPosition()) {
		spawns.push_back(new Spawner);
		spawns.back()->init(time_generation, pos_s, life);
	}
	float bounds[4] = {
		0.0f,
		0.0f,
		levels[currentLevel]->getWidth() * TILE_WIDTH,
		levels[currentLevel]->getHeight() * TILE_WIDTH
	};
	quadtree = new Quadtree();
	quadtree->init(0, bounds);

	updateQuadtree();

	
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
	for (size_t i = 0; i < humans.size(); i++)
	{
		humans[i]->draw();
	}
	for (auto &z : zombies)
	{
		z->draw();
	}
	for (auto &s : spawns) {
		s->draw();
	}
	for (auto& b : bullets) {
		b->draw();
	}
	player->draw();

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

void MainGame::updateQuadtree() {
	quadtree->clear();

	for (auto& z : zombies) {
		quadtree->insert(z->getPosition());
	}
}

void MainGame::moveAndCollide() {

	///////////////////////// PLAYER //////////////////////////
	player->update(levels[currentLevel]->getLevelData(), humans, zombies);

	///////////////////////// HUMANS //////////////////////////
	// Movimiento y colisiones
	for (auto& h : humans)
	{
		h->update(levels[currentLevel]->getLevelData(), humans, zombies);
	}


	////////////////////////// SPAWNS //////////////////////////
	// Spawn de zombies con cooldown
	for (auto& s : spawns) {
		s->update();
		s->checkSpawnZombie(zombies, 2.0f);
	}

	// destroy spawners

	for (size_t i = 0; i < spawns.size(); i++)
	{
		for(size_t j = 0; j < bullets.size(); j++)
		{
			if(spawns[i]->collideWithAgent(bullets[j])) {
				spawns[i]->loseLife();
				delete bullets[j];
				bullets[j] = bullets.back();
				bullets.pop_back();
				break;
			}
		}
	}

	// Eliminacion de spawners
	for (size_t i = 0; i < spawns.size(); i++)
	{
		if (spawns[i]->getLife() <= 0) {
			delete spawns[i];
			spawns[i] = spawns.back();
			spawns.pop_back();
		}
	}


	///////////////////////// ZOMBIES //////////////////////////
	for(auto &z: zombies) {
		z->update(levels[currentLevel]->getLevelData(), humans, zombies);
	}
	
	///////////////////////// BULLETS //////////////////////////
	// Asesinatos y eliminacion de balas

	for (auto& b : bullets) {
		b->update();
	}
	for (size_t i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->collideWithLevel(levels[currentLevel]->getLevelData())) {
			delete bullets[i];
			bullets[i] = bullets.back();
			bullets.pop_back();
		}
	}

	updateQuadtree();

	Agent* agent = new Agent();
	for(auto &b: bullets) {									// o(n*log(n)) = n * log(n) + o(n)
		std::vector<glm::vec2> nearbyObjects;
		quadtree->retrieve(nearbyObjects, b->getPosition());				// o(n) = log(n)
		for(auto& n: nearbyObjects) {							// o(log(n)) = log(n) + n
			glm::vec2 pos(n.x, n.y);
			agent->setPosition(pos);
			if (b->collideWithAgent(agent)) {						// o(log(n)) = o(log(n)) + n  
				delete b;
				b = bullets.back();
				bullets.pop_back();
				for(auto& z: zombies) {								// o(n) = o(n) + 1
					if(z->collideWithAgent(agent)) {						 // o(1) = 1
						delete z;
						z = zombies.back();
						zombies.pop_back();
						break;
					}
				}
				break;
			}
		}
	}

	updateQuadtree();

	//infecciones

	for (auto& h : humans) {
		std::vector<glm::vec2> nearbyObjects;
		quadtree->retrieve(nearbyObjects, h->getPosition());
		for (auto& n : nearbyObjects) {
			glm::vec2 pos(n.x, n.y);
			agent->setPosition(pos);
			if (h->collideWithAgent(agent)) {
				delete h;
				h = humans.back();
				humans.pop_back();
				zombies.push_back(new Zombie);
				zombies.back()->init(2.0f, pos);
				break;
			}
		}
	}

	delete agent;
}

int MainGame::getlives()
{
	return lives;
}

void MainGame::DecreaseLives()
{
	lives--;
}



void MainGame::update() {
	while (gameState != GameState::EXIT) {
		draw();
		processInput();
		updateElements();

		moveAndCollide();// mejorable con Spatial Partitioning
		if (zombies.size() == 0) {
			passLevel();
		}
		for (auto &z : zombies) 
		{
			if (z->collideWithAgent(player)) {
				//player->die();
				DecreaseLives();
				cout << "You died\n";
				cout << getlives() << endl;

				//cout << player->getLives() << endl;
				resetLevel();
				break;
			}
		}
		if (getlives() == 0) {
			reset();
			gameState = GameState::EXIT;
			cout<<"GAME OVER"<<endl;
		}

	}

}
// Reiniciar el juego al perder
void MainGame::reset() {

	glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // RGB para color celeste claro
	for (size_t i = 0; i < zombies.size(); i++)
	{
		delete zombies[i];
	}
	
	for (size_t i = 0; i < humans.size(); i++)
	{
		delete humans[i];
	}
	for (size_t i = 0; i < bullets.size(); i++)
	{
		delete bullets[i];
	}
	for (size_t i = 0; i < spawns.size(); i++)
	{
		delete spawns[i];
	}

	delete player;
	delete quadtree;
	zombies.clear();
	humans.clear();
	levels.clear();
	spawns.clear();
	bullets.clear();
}

void MainGame::passLevel() {
	currentLevel++;
	if (currentLevel == levels.size()) {
		cout << "GANASTE" << endl;
		gameState = GameState::EXIT;
	}
	else {
		reset();
		initLevel(currentLevel, false);
	}
}

void MainGame::resetLevel()
{
	reset();
	initLevel(currentLevel, false);
}

