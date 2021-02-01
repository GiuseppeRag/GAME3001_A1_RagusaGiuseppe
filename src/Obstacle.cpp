#include "Obstacle.h"
#include "TextureManager.h"
#include "SoundManager.h"

Obstacle::Obstacle()
{
	TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle");
	auto size = TextureManager::Instance()->getTextureSize("obstacle");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);
	getRigidBody()->isColliding = false;

	setType(OBSTACLE);
}

Obstacle::~Obstacle() = default;

void Obstacle::draw() {
	TextureManager::Instance()->draw("obstacle", getTransform()->position.x, getTransform()->position.y, 0, 255, false);
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}
