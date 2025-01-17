#include "Pch/Pch.h"
#include "LobbyState.h"
#include <Networking/LocalServer.h>
#include <Networking/Client.h>
#include "MenuState.h"
#include <System/StateManager.h>
#include "PlayState.h"

#define GUI_SECTION "LOBBYSTATE"

LobbyState::LobbyState()
{
	activeText = -1;
	loadGui();
	HudObject* hudObject = new HudObject("Assets/Textures/title.png", glm::vec2(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2)), glm::vec2(1280, 720));
	m_hudHandler.insertHUDObject(hudObject, HUDID::TITLE);
}

LobbyState::~LobbyState()
{
	Gui::getInstance()->clearWidgetsInSection(GUI_SECTION);
}

void LobbyState::update(float dt)
{
	for (size_t i = 0; i < m_inputBoxes.size(); i++)
	{
		if (m_inputBoxes[i].inputBox.first->isActive()) {
			if (!m_inputBoxes[i].cleared) {
				m_inputBoxes[i].cleared = true;
				m_inputBoxes[i].inputBox.first->setText("");
			}
			activeText = i;
		}
	}	
	inputHandling();
}

//Handle keyboard input 
void LobbyState::inputHandling() {

	if (Input::isKeyPressed(GLFW_KEY_TAB)) {
		activeText++;
		m_inputBoxes[activeText % m_inputBoxes.size()].inputBox.first->activate();
	}

	if (Input::isKeyPressed(GLFW_KEY_ENTER)) {
		SoundHandler::getInstance()->stopSound(ThemeSong0);
		SoundHandler::getInstance()->freeBuffer(ThemeSong0);
		bool start = true;
		for (size_t i = 0; i < m_inputBoxes.size(); i++)
		{
			if (!m_inputBoxes[i].cleared) {
				start = false;
			}
		}
		if (start) {
			startServer();
		}
	}
}



void LobbyState::render()
{
   	Renderer::getInstance()->renderHUD();
}

void LobbyState::loadGui()
{
	//Setup Servername input
	InputBox inputSNameBox;
	inputSNameBox.inputBox.first = static_cast<CEGUI::Editbox*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Editbox", glm::vec4(0.425f, 0.40f, 0.15f, 0.05f), glm::vec4(0.0f), "ServerNameInput"));
	inputSNameBox.inputBox.first->setMaxTextLength(16);
	inputSNameBox.defaultText = "Server Name...";
	inputSNameBox.inputBox.first->setText(inputSNameBox.defaultText);
	inputSNameBox.inputBox.second = SERVERNAME;
	m_inputBoxes.emplace_back(inputSNameBox);

	//Setup Username input
	InputBox inputUNameBox;
	inputUNameBox.inputBox.first = static_cast<CEGUI::Editbox*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Editbox", glm::vec4(0.425f, 0.50f, 0.15f, 0.05f), glm::vec4(0.0f), "UserNameInput"));
	inputUNameBox.inputBox.first->setMaxTextLength(16);
	inputSNameBox.defaultText = "User Name...";
	inputUNameBox.inputBox.first->setText(inputSNameBox.defaultText);
	inputUNameBox.inputBox.second = USERNAME;
	m_inputBoxes.emplace_back(inputUNameBox);

	/* BUTTONS */
	m_startServerBtn = static_cast<CEGUI::PushButton*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Button", glm::vec4(0.425f, 0.65f, 0.15f, 0.05f), glm::vec4(0.0f), "StartServer"));
	m_startServerBtn->setText("Launch server");
	m_startServerBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LobbyState::onStartSeverClicked, this));

	m_backToMenuBtn = static_cast<CEGUI::PushButton*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Button", glm::vec4(0.05f, 0.90f, 0.1f, 0.05f), glm::vec4(0.0f), "BackToMenuBtn"));
	m_backToMenuBtn->setText("Go back");
	m_backToMenuBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LobbyState::onBackToMenuClicked, this));
}

bool LobbyState::onStartSeverClicked(const CEGUI::EventArgs& e)
{
	SoundHandler::getInstance()->stopSound(ThemeSong0);
	startServer();
	return true;
}

void LobbyState::startServer()
{
	Renderer::getInstance()->clear();
	LocalServer::getInstance()->startup(getInputBoxByID(SERVERNAME)->getText().c_str());
	Client::getInstance()->startup();
	Client::getInstance()->setUsername(getInputBoxByID(USERNAME)->getText().c_str());
	Client::getInstance()->connectToMyServer();	

	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_stateManager->clearAllAndSetState(new PlayState(false));
}

bool LobbyState::onBackToMenuClicked(const CEGUI::EventArgs& e)
{
	Renderer::getInstance()->clear();
	m_stateManager->clearAllAndSetState(new MenuState());
	return true;
}

CEGUI::Editbox* LobbyState::getInputBoxByID(const GUIInputID& id) const
{
	for (size_t i = 0; i < m_inputBoxes.size(); i++)
	{
		if (m_inputBoxes[i].inputBox.second == id) {
			return m_inputBoxes[i].inputBox.first;
		}
	}
	return nullptr;
}
