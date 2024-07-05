#include "Calaca.h"
#include <glm/gtx/rotate_vector.hpp>
#include <random>

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

void Calaca::setRandomSprite()
{
	//std::uniform_int_distribution<int>randTexture(1, 10); // 10 texturas
	srand(time(nullptr));
	int randTexture = rand() % 10 + 1;

	this->texturePath = "Images/calaca" + to_string(randTexture) + ".png";

}
