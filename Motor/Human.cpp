#include "Human.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Human::Human()
{
}

Human::~Human()
{
}

void Human::init(float speed, glm::vec2 position, int raza)
{
	this->speed = speed;
	color.set(255, 255, 255, 255); //color blanco
	// aplicar filtros de color para humanos
	// el rand da el mismo valor para todos los humanos
	// para correjir esto se puede usar un seed diferente para cada humano
	
	raza %= 6;

	switch (raza)
	{
	case 0:
		//beige
		color.set(245, 245, 220, 255);
		break;
	case 1:
		// marron claro
		color.set(205, 133, 63, 255);
		break;
	case 2:
		//marron
		color.set(139, 69, 19, 255);
		break;
	case 3:
		color.set(255, 255, 0, 255); //color amarillo
		break;
	case 4:
		//marro oscuro
		color.set(139, 69, 19, 255);
		break;
	case 5:
		// piel claro
		color.set(255, 218, 185, 255);
		break;
	default:
		break;
	}
	this->position = position;

	//Aletorizar primer frame del sprite e inicilizar variables
	srand(time(NULL));
	currentFrame = rand() % 4;
	valDirection = 1;
	animationSpeed = 0;

	////////////////Comportamientos/////////////
	// obtner direccion
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float>randDir(-1.0f, 1.0f);
	direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (direction.length() == 0) {
		direction = glm::vec2(1.0f, 1.0f);
	}
	//direction = glm::vec2(direction);

}

void Human::update(vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies)
{
	std::mt19937 randomEngie(time(nullptr));
	std::uniform_real_distribution<float>randRotate(-40.0f, 40.0f);
	position += direction * speed;
	if (collideWithLevel(levelData)) {
		direction = glm::rotate(direction, randRotate(randomEngie));
	}
	//Condicional si el movimiento es negativo(a la izquierda) cambiar direccion vertical del frame
	if (direction.x * speed < 0) {
		valDirection = -1;
	}
	//Condicional si el movimiento es positivo(a la derecha) cambiar direccion vertical del frame

	if (direction.x * speed >= 0) {
		valDirection = 1;
	}

	//reducir tiempo del frame de animacion
	animationSpeed++;
	if (animationSpeed == 8) {
		currentFrame = (currentFrame + 1) % 4;
		animationSpeed = 0;
	}

}

void Human::draw()
{

	spritebatch.init();
	spritebatch.begin();
	//crear uvRect para rectangulo del frame de la imagen a dibujar
	glm::vec4 uvRect(currentFrame * (1.0f / 4), 0.0f, valDirection * (1.0f / 4), 1.0f);
	//crear destRect para posición del dibujo
	glm::vec4 destRect(position.x, position.y, AGENT_WIDTH * 0.9, AGENT_WIDTH * 0.8);
	spritebatch.draw(destRect, uvRect, ResourceManager::getTexture("Images/human.png").id, 0.0f, color);
	spritebatch.end();
	spritebatch.renderBatch();
}
