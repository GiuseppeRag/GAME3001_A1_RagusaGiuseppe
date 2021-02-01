#pragma once
#ifndef __CAR__
#define __CAR__
#include "DisplayObject.h"
#include "AlgorithmType.h"
#include "Whisker.h"

class Car : public DisplayObject
{
private:
	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;

	float m_rotationAngle;
	float m_maxSpeed;
	float m_maxAcceleration;
	float m_turnRate;
	float m_accelerationRate;
	float m_stopDistance;
	float m_slowDistance;
	AlgorithmType m_algorithmType;
	std::vector<Whisker*> whiskers;

	//Private functions
	void m_seekAndFleeAlgorithm();
	void m_arriveAlgorithm();
	void m_avoidAlgorithm();

public:
	Car();
	~Car();
	void draw() override;
	void update() override;
	void clean() override;

	//Getters
	glm::vec2 getOrientation();
	float getRotation();
	float getTurnRate();
	float getAccelerationRate();
	AlgorithmType getAlgorithmType();

	//Setters
	void setDestination(const glm::vec2 destination);
	void setMaxSpeed(float speed);
	void setOrientation(glm::vec2 orientation);
	void setRotation(float rotation);
	void setTurnRate(float rate);
	void setAccelerationRate(float rate);
	void setStopDistance(float distance);
	void setSlowDistance(float distance);
	void setAlgorithmType(AlgorithmType type);

	//Whisker Managers
	void addWhiskers();
	void deleteWhiskers();
};

#endif // !__CAR__
