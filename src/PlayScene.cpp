#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

typedef glm::vec2 v2;

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			//No controller logic. All keyboard
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			TheGame::Instance()->quit();
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
		{
			m_pCar->setEnabled(true);
			m_pCar->setAlgorithmType(SEEK);

			m_pTarget->setEnabled(true);

			m_pInstructionsLabel->setEnabled(false);
			m_pSeekLabel->setEnabled(false);
			m_pFleeLabel->setEnabled(false);
			m_pArriveLabel->setEnabled(false);
			m_pAvoidLabel->setEnabled(false);
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
		{
			m_pCar->setEnabled(true);
			m_pCar->setAlgorithmType(FLEE);

			m_pTarget->setEnabled(true);

			m_pInstructionsLabel->setEnabled(false);
			m_pSeekLabel->setEnabled(false);
			m_pFleeLabel->setEnabled(false);
			m_pArriveLabel->setEnabled(false);
			m_pAvoidLabel->setEnabled(false);
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3))
		{
			m_pCar->setEnabled(true);
			m_pCar->setAlgorithmType(ARRIVE);

			m_pTarget->setEnabled(true);

			m_pInstructionsLabel->setEnabled(false);
			m_pSeekLabel->setEnabled(false);
			m_pFleeLabel->setEnabled(false);
			m_pArriveLabel->setEnabled(false);
			m_pAvoidLabel->setEnabled(false);

			m_pCar->setSlowDistance(m_pTarget->getWidth() * 6);
			m_pCar->setStopDistance(m_pTarget->getWidth());
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_4))
		{
			m_pCar->setEnabled(true);
			m_pCar->setAlgorithmType(AVOID);
			m_pCar->addWhiskers();

			m_pTarget->setEnabled(true);

			m_pInstructionsLabel->setEnabled(false);
			m_pSeekLabel->setEnabled(false);
			m_pFleeLabel->setEnabled(false);
			m_pArriveLabel->setEnabled(false);
			m_pAvoidLabel->setEnabled(false);
		}

		//DEBUG ONLY
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_5))
		{
			m_pCar->deleteWhiskers();
		}
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = v2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	m_pSeekLabel = new Label("Press 1 to activate the 'Seek' Algorithm", "Consolas");
	m_pSeekLabel->getTransform()->position = v2(Config::SCREEN_WIDTH * 0.5f, 100.0f);

	m_pFleeLabel = new Label("Press 2 to activate the 'Flee' Algorithm", "Consolas");
	m_pFleeLabel->getTransform()->position = v2(Config::SCREEN_WIDTH * 0.5f, 200.0f);

	m_pArriveLabel = new Label("Press 3 to activate the 'Arrive' Algorithm", "Consolas");
	m_pArriveLabel->getTransform()->position = v2(Config::SCREEN_WIDTH * 0.5f, 300.0f);

	m_pAvoidLabel = new Label("Press 4 to activate the 'Avoid' Algorithm", "Consolas");
	m_pAvoidLabel->getTransform()->position = v2(Config::SCREEN_WIDTH * 0.5f, 400.0f);

	/* Objects */

	m_pTarget = new Target();
	m_pTarget->getTransform()->position = v2(400.0f, 300.0f);
	m_pTarget->setEnabled(false);
	addChild(m_pTarget);

	m_pCar = new Car();
	m_pCar->getTransform()->position = v2(100.0f, 100.0f);
	m_pCar->setEnabled(false);
	m_pCar->setDestination(m_pTarget->getTransform()->position);
	addChild(m_pCar);

	//
	addChild(m_pInstructionsLabel);
	addChild(m_pSeekLabel);
	addChild(m_pFleeLabel);
	addChild(m_pArriveLabel);
	addChild(m_pAvoidLabel);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 Assingment 1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Button("Seek"))
	{
		std::cout << "Seek Algorithm Activated" << std::endl;
	}

	ImGui::Separator();

	static float float3[3] = { 0.0f, 1.0f, 1.5f };
	if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	{
		std::cout << float3[0] << std::endl;
		std::cout << float3[1] << std::endl;
		std::cout << float3[2] << std::endl;
		std::cout << "---------------------------\n";
	}
	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
