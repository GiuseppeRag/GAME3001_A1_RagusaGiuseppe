#pragma once
#ifndef __WHISKER__
#define __WHISKER__
#include "DisplayObject.h"

class Whisker : public DisplayObject
{
public:
	Whisker(glm::vec2 actorPos, float rotation, float size);
	~Whisker();

	void draw() override;
	void update() override;
	void clean() override;

	void setStart(glm::vec2 vec);
	void setEnd(glm::vec2 start, float rotation, float size);
	glm::vec2 getStart();
	glm::vec2 getEnd();
private:
	glm::vec2 m_start;
	glm::vec2 m_end;
	glm::vec2 convertToOrientation(float rotation);
};

#endif