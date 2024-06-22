#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

void Bullet::init(glm::vec2 position, glm::vec2 direction, float speed)
{
	color.set(255, 255, 0, 255);
	this->direction = direction;
	this->speed = speed;
	this->position = position;

}

void Bullet::update(vector<string>& levelData)
{
	position += direction * speed;
	collideWithLevel(levelData);

}
