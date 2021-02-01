#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "Car.h"
#include "Target.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	// UI Items
	Label* m_pInstructionsLabel;
	Label* m_pSeekLabel;
	Label* m_pFleeLabel;
	Label* m_pArriveLabel;
	Label* m_pAvoidLabel;

	// Game Objects
	Car* m_pCar;
	Target* m_pTarget;
};

#endif /* defined (__PLAY_SCENE__) */