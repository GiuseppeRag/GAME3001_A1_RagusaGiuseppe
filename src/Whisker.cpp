#include "Whisker.h"
#include "Util.h"

glm::vec2 Whisker::convertToOrientation(float rotation)
{
	float angleRads = (rotation - 90.0) * Util::Deg2Rad;
	return (glm::vec2(cos(angleRads), sin(angleRads)));
}

Whisker::Whisker(glm::vec2 actorPos, float rotation)
{
	m_start = actorPos;
	m_end = actorPos + (convertToOrientation(rotation) * 130.0f);
}

Whisker::~Whisker() = default;

void Whisker::draw()
{
	Util::DrawLine(m_start, m_end);
}

void Whisker::update()
{
}

void Whisker::clean()
{
}

void Whisker::setStart(glm::vec2 vec)
{
	m_start = vec;
}

void Whisker::setEnd(glm::vec2 start, float rotation)
{
	m_end = start + (convertToOrientation(rotation) * 130.0f);
}

glm::vec2 Whisker::getStart()
{
	return m_start;
}

glm::vec2 Whisker::getEnd()
{
	return m_end;
}
