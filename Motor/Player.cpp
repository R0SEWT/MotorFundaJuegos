#include "Player.h"
#include <SDL/SDL.h>

#include <iostream>
using namespace std;

Player::Player()
{
}

Player::~Player()
{
}

void Player::init(float speed, glm::vec2 position, InputManager* inputManager, int shotColdown)
{
	this->speed = speed;
	this->position = position;
	this->inputmanager = inputManager;
	this->shotColdown = shotColdown;
	this->timeForShot = shotColdown;
	this->currentShotColdown = shotColdown;
	this->shot = false;
	direction = TOP;

	color.set(20, 20, 200, 255);
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

	if (inputmanager->isKeyDown(SDLK_SPACE) && timeForShot < 0) {
		shot = true;
	}
	else{
		timeForShot--;
	}

	collideWithLevel(levelData);
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
	shot = false;
}


void Player::updateShotColdown(float camScale) 
{
	if (camScale != 0) {
		currentShotColdown = shotColdown / camScale;
	}
	cout << currentShotColdown << endl;
}