#pragma once
#ifndef __TEMP_NODE__
#define __TEMP_NODE__
#include "DisplayObject.h"

class TempNode : public DisplayObject
{
public:
	TempNode();
	~TempNode();
	void draw() override;
	void update() override;
	void clean() override;
private:
};

#endif
