#pragma once
#include <glm\gtc\matrix_transform.hpp>
#include "InputManager.h"

class Camera2D
{
private:
	InputManager* inputManager;
	glm::vec2 position;
	float scale;
	float changeScale;
	int screenWidth;
	int screenHeight;
	bool needsUpdateMatrix;
	glm::mat4 cameraMatrix;
	glm::mat4 orthoMatrix;

public:
	void update();
	void init(int screenWidth, int screenHeight, InputManager* inputManager, float changeScale);
	glm::vec2 getPosition() {
		return position;
	}
	glm::vec2 convertToScreenWorld(glm::vec2 screenScords);
	float getScale() {
		return scale;
	}
	void setPosition(const glm::vec2& newPosition) {
		position = newPosition;
		needsUpdateMatrix = true;
	}
	void setScale(float newScale) {
		scale = newScale;
		needsUpdateMatrix = true;
	}
	glm::mat4 getCameraMatrix() {
		return cameraMatrix;
	}

	void setChangeScale(float changeScale) {
		this->changeScale = changeScale;
	}
	Camera2D();
	~Camera2D();
};

