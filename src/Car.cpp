#include "Car.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "Util.h"
#include "Game.h"
typedef glm::vec2 v2;

Car::Car() : m_maxSpeed(8.0f), m_orientation(v2(0.0f, -1.0f)), m_rotationAngle(0.0f), m_accelerationRate(10.0f), m_turnRate(10.0f)
{
	TextureManager::Instance()->load("../Assets/textures/Car.png", "car");
	auto size = TextureManager::Instance()->getTextureSize("car");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = v2(400.0f, 300.0f);
	getRigidBody()->velocity = v2(0.0f, 0.0f);
	getRigidBody()->acceleration = v2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;

	setType(CAR);
}

Car::~Car() {
	deleteWhiskers();
}


void Car::draw()
{
	TextureManager::Instance()->draw("car", getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, false);
	Util::DrawLine(v2(getTransform()->position.x + (getWidth() / 2), getTransform()->position.y + (getHeight() / 2)), 
		v2(getTransform()->position.x + (getWidth() / 2), getTransform()->position.y + (getHeight() / 2)) + m_orientation * 60.0f);
	if (whiskers.size() > 0) {
		whiskers[0]->draw();
		whiskers[1]->draw();
	}
}

void Car::update()
{
	switch (getAlgorithmType())
	{
	case SEEK:
		m_seekAndFleeAlgorithm();
		break;
	case FLEE:
		m_seekAndFleeAlgorithm();
		break;
	case ARRIVE:
		m_arriveAlgorithm();
		break;
	case AVOID:
		m_avoidAlgorithm();
		if (whiskers.size() > 0) {
			whiskers[0]->setStart(v2(getTransform()->position.x + (getWidth() / 2), getTransform()->position.y + (getHeight() / 2)));
			whiskers[1]->setStart(v2(getTransform()->position.x + (getWidth() / 2), getTransform()->position.y + (getHeight() / 2)));
			whiskers[0]->setEnd(v2(getTransform()->position.x + (getWidth() / 2), getTransform()->position.y + (getHeight() / 2)), m_rotationAngle + 35.0);
			whiskers[1]->setEnd(v2(getTransform()->position.x + (getWidth() / 2), getTransform()->position.y + (getHeight() / 2)), m_rotationAngle - 35.0);
		}
		break;
	}
}

void Car::clean()
{
}

void Car::addWhiskers()
{
	whiskers.push_back(new Whisker(v2(getTransform()->position.x + (getWidth() / 2), getTransform()->position.y + (getHeight() / 2)), m_rotationAngle + 35.0));
	whiskers.push_back(new Whisker(v2(getTransform()->position.x + (getWidth() / 2), getTransform()->position.y + (getHeight() / 2)), m_rotationAngle - 35.0));
}

void Car::deleteWhiskers()
{
	if (whiskers.size() > 0) {
		delete whiskers[0];
		whiskers[0] = nullptr;
		delete whiskers[1];
		whiskers[1] = nullptr;
		whiskers.shrink_to_fit();
		whiskers.resize(0);
	}
}

v2 Car::getOrientation()
{
	return m_orientation;
}

float Car::getRotation()
{
	return m_rotationAngle;
}

float Car::getTurnRate()
{
	return m_turnRate;
}

float Car::getAccelerationRate()
{
	return m_accelerationRate;
}

AlgorithmType Car::getAlgorithmType()
{
	return m_algorithmType;
}

Whisker* Car::getLeftWhisker()
{
	return whiskers[1];
}

Whisker* Car::getRightWhisker()
{
	return whiskers[0];
}

bool Car::hasWhiskers()
{
	return (whiskers.size() > 0);
}

void Car::setDestination(const v2 destination)
{
	m_destination = destination;
}

void Car::setMaxSpeed(float speed)
{
	m_maxSpeed = speed;
}

void Car::setOrientation(v2 orientation)
{
	m_orientation = orientation;
}

void Car::setRotation(float rotation)
{
	m_rotationAngle = rotation;

	float angleRads;
	if (getAlgorithmType() == FLEE)
		angleRads = (rotation + 90.0f) * Util::Deg2Rad;
	else
		angleRads = (rotation - 90.0f) * Util::Deg2Rad;
	setOrientation(v2(cos(angleRads), sin(angleRads)));
}

void Car::setTurnRate(float rate)
{
	m_turnRate = rate;
}

void Car::setAccelerationRate(float rate)
{
	m_accelerationRate = rate;
}

void Car::setStopDistance(float distance)
{
	m_stopDistance = distance;
}

void Car::setSlowDistance(float distance)
{
	m_slowDistance = distance;
}

void Car::setAlgorithmType(AlgorithmType type)
{
	m_algorithmType = type;
}

void Car::reset()
{
	m_maxSpeed = 8.0f; 
	m_orientation = v2(0.0f, -1.0f);
	m_rotationAngle = 0.0f;
	m_accelerationRate = 10.0f;
	m_turnRate = 10.0f;

	getTransform()->position = v2(-50.0f, 125.0f);
	getRigidBody()->velocity = v2(0.0f, 0.0f);
	getRigidBody()->acceleration = v2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
}

void Car::m_seekAndFleeAlgorithm()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	m_targetDirection = m_destination - getTransform()->position;
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);
	float turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity) {
		if (target_rotation > 0)
			setRotation(getRotation() + getTurnRate());
		else if (target_rotation < 0)
			setRotation(getRotation() - getTurnRate());
	}

	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();
	if (m_algorithmType == SEEK)
		getRigidBody()->velocity += getOrientation() * float(deltaTime) + 0.5f * getRigidBody()->acceleration * float(deltaTime);
	else
		getRigidBody()->velocity -= getOrientation() * float(deltaTime) + 0.5f * getRigidBody()->acceleration * float(deltaTime);
	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);
	getTransform()->position += getRigidBody()->velocity;
}

void Car::m_arriveAlgorithm()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	m_targetDirection = m_destination - getTransform()->position;
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);
	float turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity) {
		if (target_rotation > 0)
			setRotation(getRotation() + getTurnRate());
		else if (target_rotation < 0)
			setRotation(getRotation() - getTurnRate());
	}

	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();
	getRigidBody()->velocity += getOrientation() * float(deltaTime) + 0.5f * getRigidBody()->acceleration * float(deltaTime);

	const float distance = Util::distance(getTransform()->position, m_destination);
	if (distance < m_slowDistance) {
		m_maxSpeed =  8.0f * float(distance / m_slowDistance);
	}
	if (distance < m_stopDistance) {
		m_maxSpeed = 0.0f;
	}
	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);
	getTransform()->position += getRigidBody()->velocity;
}

void Car::m_avoidAlgorithm()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	if (whiskers[0]->getRigidBody()->isColliding || whiskers[1]->getRigidBody()->isColliding)
		m_maxSpeed = 3.0f;
	else
		m_maxSpeed = 8.0f;

	m_targetDirection = m_destination - getTransform()->position;
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);
	float turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity) {
		if (target_rotation > 0)
			setRotation(getRotation() + getTurnRate());
		else if (target_rotation < 0)
			setRotation(getRotation() - getTurnRate());
	}
	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();
	getRigidBody()->velocity += getOrientation() * float(deltaTime) + 0.5f * getRigidBody()->acceleration * float(deltaTime);
	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);
	getTransform()->position += getRigidBody()->velocity;
}
