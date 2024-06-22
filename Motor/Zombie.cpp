#include "Zombie.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include "Human.h"


Zombie::Zombie()
{
	
}

Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 position)
{
	srand(time(NULL));
	this->speed = speed;
	color.set(0, 200, 0, 255); // verde
	this->position = position;
	this->furia_duration = 0;

	// obtner direccion aleatoria
	
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float>randDir(-1.0f, 1.0f);
	direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (direction.length() == 0) {
		direction = glm::vec2(1.0f, 1.0f);
	}
}

void Zombie::update(vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies)
{
	collideWithLevel(levelData);
	Human* closeHuman = getNearestHuman(humans);
	if (closeHuman != nullptr) {
		glm::vec2 direction = glm::normalize(
			closeHuman->getPosition() - position
		);
		position += direction * speed;
	}

}

Human* Zombie::getNearestHuman(vector<Human*>& humans)
{
	Human* closeHuman = nullptr;
	float smallestDistance = 888888.0f;
	for (size_t i = 0; i < humans.size(); i++)
	{
		glm::vec2 dist = humans[i]->getPosition() - position;
		float distance = glm::length(dist);
		if (distance < smallestDistance) {
			smallestDistance = distance;
			closeHuman = humans[i];
		}
	}
	return closeHuman;
}
