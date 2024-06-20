#include "Zombie.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>



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
	std::mt19937 randomEngie(time(nullptr));
	std::uniform_real_distribution<float>randRotate(-40.0f, 40.0f);
	

	position += direction * speed;

	if (furia_duration <= 0)
	{
		if (rand() % 1000 < 4) // 0.8% de enfurecerse por frame (cuando no tengan furia)
		{
			furia_duration = 550;
			color.set(1, 56, 9, 255); // verde oscuro 
		}
	}
	else { 
		position += direction * speed * 3.0f;
		furia_duration--;
		if (furia_duration == 0)
		{
			color.set(0, 200, 0, 255); // color normal
		}
	}

	if (collideWithLevel(levelData) || rand() % 1000 < 64) { // 6.4% de desorientarse por frame
		direction = glm::rotate(direction, randRotate(randomEngie));
	}
}
