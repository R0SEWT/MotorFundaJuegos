#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// matrices de transformacion  (?)
class Camera2D
{
private:
	glm::vec2 position;
	float scale;
	int screenWidth;
	int screenHeight;

	glm::mat4 cameraMatrix;
	glm::mat4 orthoMatrix;
	bool needsUpdateMatrix;

public:
	Camera2D();
	~Camera2D();
	void update();
	void init(int screenWidth, int screenHeight);
	void setPosition(const glm::vec2& newPosition);
	void setScale(float newScale);

	glm::mat4 getCameraMatrix();

};

