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

void Bullet::draw()
{
	spritebatch.init();
	spritebatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f); // por defecto la textura completa 
	glm::vec4 destRect(position.x, position.y, AGENT_WIDTH/2, AGENT_WIDTH/2);
	spritebatch.draw(destRect, uvRect, ResourceManager::getTexture("Images/circle.png").id, 0.0f, color);
	spritebatch.end();
	spritebatch.renderBatch();
}

void Bullet::update()
{
	position += direction * speed;
}

bool Bullet::iSForDestroy(vector<string>& levelData)
{
	if (collideWithLevel(levelData))
	{
		return true;
	}
	return false;
}


