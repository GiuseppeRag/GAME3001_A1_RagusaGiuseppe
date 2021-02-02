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
	if (m_pCar->hasWhiskers())
	{
		if (CollisionManager::lineRectCheck(m_pCar->getLeftWhisker()->getStart(), m_pCar->getLeftWhisker()->getEnd(), m_pObstacle->getTransform()->position,
			m_pObstacle->getWidth(), m_pObstacle->getHeight())) {
			if (!m_pCar->getLeftWhisker()->getRigidBody()->isColliding) {
				m_pCar->getRigidBody()->velocity * 0.4f;
				m_pCar->getLeftWhisker()->getRigidBody()->isColliding = true;
			}
			m_pTemp_Node->getTransform()->position = m_pCar->getRightWhisker()->getEnd();
			m_pCar->setDestination(m_pTemp_Node->getTransform()->position);
		}
		else {
			m_pCar->getLeftWhisker()->getRigidBody()->isColliding = false;
		}

		if (CollisionManager::lineRectCheck(m_pCar->getRightWhisker()->getStart(), m_pCar->getRightWhisker()->getEnd(),
			m_pObstacle->getTransform()->position, m_pObstacle->getWidth(), m_pObstacle->getHeight())) {
			if (!m_pCar->getRightWhisker()->getRigidBody()->isColliding) {
				m_pCar->getRigidBody()->velocity * 0.4f;
				m_pCar->getRightWhisker()->getRigidBody()->isColliding = true;
			}
			m_pTemp_Node->getTransform()->position = m_pCar->getLeftWhisker()->getEnd();
			m_pCar->setDestination(m_pTemp_Node->getTransform()->position);
		}
		else {
			m_pCar->getRightWhisker()->getRigidBody()->isColliding = false;
		}
	}

	if (m_pTemp_Node->isEnabled())
	{
		if (Util::distance(m_pCar->getTransform()->position, m_pTemp_Node->getTransform()->position) < 100)
			m_pCar->setDestination(m_pTarget->getTransform()->position);
	}
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
			std::cout << "Seek Algorithm Activated" << std::endl;
			m_pCar->setEnabled(true);
			m_pCar->reset();
			m_pCar->setDestination(m_pTarget->getTransform()->position);
			m_pCar->setAlgorithmType(SEEK);
			if (m_pCar->hasWhiskers())
				m_pCar->deleteWhiskers();

			m_pTarget->setEnabled(true);
			m_pTarget->getTransform()->position = v2(400.0f, 300.0f);

			m_pObstacle->setEnabled(false);
			m_pTemp_Node->setEnabled(false);

			m_pInstructionsLabel->setEnabled(false);
			m_pSeekLabel->setEnabled(false);
			m_pFleeLabel->setEnabled(false);
			m_pArriveLabel->setEnabled(false);
			m_pAvoidLabel->setEnabled(false);
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
		{
			std::cout << "Flee Algorithm Activated" << std::endl;
			m_pCar->setEnabled(true);
			m_pCar->reset();
			m_pCar->getTransform()->position = v2(300.0f, 250.0f);
			m_pCar->setDestination(m_pTarget->getTransform()->position);
			m_pCar->setAlgorithmType(FLEE);
			if (m_pCar->hasWhiskers())
				m_pCar->deleteWhiskers();

			m_pTarget->setEnabled(true);
			m_pTarget->getTransform()->position = v2(400.0f, 300.0f);

			m_pObstacle->setEnabled(false);
			m_pTemp_Node->setEnabled(false);

			m_pInstructionsLabel->setEnabled(false);
			m_pSeekLabel->setEnabled(false);
			m_pFleeLabel->setEnabled(false);
			m_pArriveLabel->setEnabled(false);
			m_pAvoidLabel->setEnabled(false);
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3))
		{
			std::cout << "Arrive Algorithm Activated" << std::endl;
			m_pCar->setEnabled(true);
			m_pCar->reset();
			m_pCar->setDestination(m_pTarget->getTransform()->position);
			m_pCar->setAlgorithmType(ARRIVE);
			if (m_pCar->hasWhiskers())
				m_pCar->deleteWhiskers();

			m_pTarget->setEnabled(true);
			m_pTarget->getTransform()->position = v2(400.0f, 300.0f);

			m_pObstacle->setEnabled(false);
			m_pTemp_Node->setEnabled(false);

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
			std::cout << "Avoid Algorithm Activated" << std::endl;
			m_pCar->setEnabled(true);
			m_pCar->reset();
			m_pCar->setAlgorithmType(AVOID);
			m_pCar->addWhiskers();

			m_pObstacle->setEnabled(true);
			m_pTemp_Node->setEnabled(true);
			m_pTarget->setEnabled(true);
			m_pTarget->getTransform()->position = v2(600.0f, 300.0f);
			m_pCar->setDestination(m_pTarget->getTransform()->position);

			m_pInstructionsLabel->setEnabled(false);
			m_pSeekLabel->setEnabled(false);
			m_pFleeLabel->setEnabled(false);
			m_pArriveLabel->setEnabled(false);
			m_pAvoidLabel->setEnabled(false);
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

	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = v2(250.0f, 250.0f);
	m_pObstacle->setEnabled(false);
	addChild(m_pObstacle);

	m_pTemp_Node = new TempNode();
	m_pTemp_Node->setEnabled(false);

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
	
	ImGui::Begin("GAME3001 Assingment 1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Button("Seek"))
	{
		std::cout << "Seek Algorithm Activated" << std::endl;
		m_pCar->setEnabled(true);
		m_pCar->reset();
		m_pCar->setDestination(m_pTarget->getTransform()->position);
		m_pCar->setAlgorithmType(SEEK);
		if (m_pCar->hasWhiskers())
			m_pCar->deleteWhiskers();

		m_pTarget->setEnabled(true);
		m_pTarget->getTransform()->position = v2(400.0f, 300.0f);

		m_pObstacle->setEnabled(false);
		m_pTemp_Node->setEnabled(false);

		m_pInstructionsLabel->setEnabled(false);
		m_pSeekLabel->setEnabled(false);
		m_pFleeLabel->setEnabled(false);
		m_pArriveLabel->setEnabled(false);
		m_pAvoidLabel->setEnabled(false);
	}

	ImGui::SameLine();

	if (ImGui::Button("Flee"))
	{
		std::cout << "Flee Algorithm Activated" << std::endl;
		m_pCar->setEnabled(true);
		m_pCar->reset();
		m_pCar->getTransform()->position = v2(300.0f, 250.0f);
		m_pCar->setDestination(m_pTarget->getTransform()->position);
		m_pCar->setAlgorithmType(FLEE);
		if (m_pCar->hasWhiskers())
			m_pCar->deleteWhiskers();

		m_pTarget->setEnabled(true);
		m_pTarget->getTransform()->position = v2(400.0f, 300.0f);

		m_pObstacle->setEnabled(false);
		m_pTemp_Node->setEnabled(false);

		m_pInstructionsLabel->setEnabled(false);
		m_pSeekLabel->setEnabled(false);
		m_pFleeLabel->setEnabled(false);
		m_pArriveLabel->setEnabled(false);
		m_pAvoidLabel->setEnabled(false);
	}

	ImGui::SameLine();

	if (ImGui::Button("Arrive"))
	{
		std::cout << "Arrive Algorithm Activated" << std::endl;
		m_pCar->setEnabled(true);
		m_pCar->reset();
		m_pCar->setDestination(m_pTarget->getTransform()->position);
		m_pCar->setAlgorithmType(ARRIVE);
		if (m_pCar->hasWhiskers())
			m_pCar->deleteWhiskers();

		m_pTarget->setEnabled(true);
		m_pTarget->getTransform()->position = v2(400.0f, 300.0f);

		m_pObstacle->setEnabled(false);
		m_pTemp_Node->setEnabled(false);

		m_pInstructionsLabel->setEnabled(false);
		m_pSeekLabel->setEnabled(false);
		m_pFleeLabel->setEnabled(false);
		m_pArriveLabel->setEnabled(false);
		m_pAvoidLabel->setEnabled(false);

		m_pCar->setSlowDistance(m_pTarget->getWidth() * 6);
		m_pCar->setStopDistance(m_pTarget->getWidth());
	}

	ImGui::SameLine();

	if (ImGui::Button("Avoid"))
	{
		std::cout << "Avoid Algorithm Activated" << std::endl;
		m_pCar->setEnabled(true);
		m_pCar->reset();
		m_pCar->setAlgorithmType(AVOID);
		m_pCar->addWhiskers();

		m_pObstacle->setEnabled(true);
		m_pTemp_Node->setEnabled(true);
		m_pTarget->setEnabled(true);
		m_pTarget->getTransform()->position = v2(600.0f, 300.0f);
		m_pCar->setDestination(m_pTarget->getTransform()->position);

		m_pInstructionsLabel->setEnabled(false);
		m_pSeekLabel->setEnabled(false);
		m_pFleeLabel->setEnabled(false);
		m_pArriveLabel->setEnabled(false);
		m_pAvoidLabel->setEnabled(false);
	}

	ImGui::Separator();

	if (ImGui::Button("Reset"))
	{
		std::cout << "Resetting The Screen" << std::endl;
		m_pCar->setEnabled(false);
		m_pCar->setAlgorithmType(NO_ALGORITHM);
		m_pTarget->setEnabled(false);
		m_pObstacle->setEnabled(false);
		m_pTemp_Node->setEnabled(false);
		if (m_pCar->hasWhiskers())
			m_pCar->deleteWhiskers();

		m_pSeekLabel->setEnabled(true);
		m_pFleeLabel->setEnabled(true);
		m_pArriveLabel->setEnabled(true);
		m_pAvoidLabel->setEnabled(true);
		m_pInstructionsLabel->setEnabled(true);
	}

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
