#include "Player.h"
#include <SDL/SDL.h>
#include <algorithm> 
#include <iostream>

using namespace std;

Player::Player()
{
}

Player::~Player()
{
}

void Player::init(float speed, glm::vec2 position, InputManager* inputManager, float shotColdown)
{
	this->speed = speed;
	this->position = position;
	this->inputmanager = inputManager;
	this->shotColdown = shotColdown;
	this->timeForShot = shotColdown;
	this->currentShotColdown = shotColdown;
	this->godMode = false;
	this->aux_speed_coldown = make_pair(speed, shotColdown);

	direction = TOP;

	color.set(200, 200, 200, 255);
}

void Player::update(vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies)
{
	// 00 abajo izq

	if (inputmanager->isKeyDown(SDLK_w)) {
		position.y += speed;
		direction = TOP;
	}
	if (inputmanager->isKeyDown(SDLK_s)) {
		position.y -= speed;
		direction = DOWN;
	}
	if (inputmanager->isKeyDown(SDLK_a)) {
		position.x -= speed;
		direction = LEFT;
	}
	if (inputmanager->isKeyDown(SDLK_d)) {
		position.x += speed;
		direction = RIGHT;
	}

	collideWithLevel(levelData);
	if (timeForShot > 0) {
		timeForShot--;
	}
	GodMode();
	// cambiando el color del player como animacion
	if (godMode) {
			int r = (color.r + 100 * sin(0.3 * SDL_GetTicks()));  
			int g = (color.g + 100 * sin(0.3 * SDL_GetTicks())); // 100 es la amplitud de la onda y 0.0003 es la frecuencia
			int a = (color.a + 10 * sin(0.3 * SDL_GetTicks())); 

			r = 255;
			g = 255;
			a = glm::clamp(a, 60, 255);

			int b = 0;

			color.set(r, g, b, a);
	}
	
}


glm::vec2 Player::getDirection() {
	switch(direction) {

		case TOP:
			return glm::vec2(0, 1);
			break;
		case DOWN:
			return glm::vec2(0, -1);
			break;
		case LEFT:
			return glm::vec2(-1, 0);
			break;
		case RIGHT:
			return glm::vec2(1, 0);
			break;
	}
}

void Player::resetCDShot() {
	timeForShot = currentShotColdown;
}


void Player::updateShotColdown(float camScale) 
{
	if (camScale > 0) {
		currentShotColdown = shotColdown / camScale;
	}
	//cout << currentShotColdown << endl;
}

bool Player::shotReady() {
	if (timeForShot <= 0) {
		return true;
	}
	return false;
}

void Player::infoCD() {
	cout << shotColdown << ":" << currentShotColdown << "/" << currentShotColdown - shotColdown << endl;
}

void Player::GodMode()
{
	if (inputmanager->isKeyDown(SDLK_g)) {
		godMode = true;
		shotColdown = 1;
		speed = 15;

	}
	if (inputmanager->isKeyDown(SDLK_u)){
		godMode = false;
		shotColdown = aux_speed_coldown.second;
		speed = aux_speed_coldown.first;
		color.set(200, 200, 200, 255);
	}
}

void Player::draw()
{
	spritebatch.init();
	spritebatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f); // por defecto la textura completa 
	glm::vec4 destRect(position.x, position.y, AGENT_WIDTH, AGENT_WIDTH);
	spritebatch.draw(destRect, uvRect, ResourceManager::getTexture("Images/spiderman.png").id, 0.0f, color);
	spritebatch.end();
	spritebatch.renderBatch();
}