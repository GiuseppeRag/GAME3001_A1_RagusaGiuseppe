#include "TempNode.h"

TempNode::TempNode()
{
	getRigidBody()->isColliding = false;
	getTransform()->position = glm::vec2(0.0f, 0.0f);
	setType(TEMP_NODE);
}

TempNode::~TempNode() = default;

void TempNode::draw()
{
}

void TempNode::update()
{
}

void TempNode::clean()
{
}
