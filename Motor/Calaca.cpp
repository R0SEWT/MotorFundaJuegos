#include "Calaca.h"
#include <glm/gtx/rotate_vector.hpp>
#include <random>
#include <iostream>


Calaca::Calaca()
{
	texturePath = "Images/circle.png";
}

Calaca::~Calaca()
{
}

void Calaca::init(float speed, glm::vec2 position)
{
	this->speed = speed;
	
	this->position = position;

	// obtner direccion
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float>randDir(-1.0f, 1.0f);
	direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (direction.length() == 0) {
		direction = glm::vec2(1.0f, 1.0f);
	}
	direction = glm::normalize(direction);

	color.set(255, 255, 255, 255);

}

void Calaca::update(vector<string>& levelData)
{
	std::mt19937 randomEngie(time(nullptr));
	std::uniform_real_distribution<float>randRotate(-40.0f, 40.0f);
	position += direction * speed;
	if (collideWithLevel(levelData)) {
		direction = glm::rotate(direction, randRotate(randomEngie));
	}
}

void Calaca::draw()
{
	spritebatch.init();
	spritebatch.begin();
	
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(position.x, position.y, AGENT_WIDTH * 0.9, AGENT_WIDTH * 0.8);
	spritebatch.draw(destRect, uvRect, ResourceManager::getTexture(texturePath).id, 0.0f, color);
	spritebatch.end();
	spritebatch.renderBatch();
}


void Calaca::setRandomSprite(int randTexture)
{
	//std::uniform_int_distribution<int>randTexture(1, 10); // 10 texturas
	
	
	this->texturePath = "Images/calaca" + to_string(randTexture) + ".png";

}
