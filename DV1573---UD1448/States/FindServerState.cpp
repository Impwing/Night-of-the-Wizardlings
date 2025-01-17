#include <Pch/Pch.h>
#include "FindServerState.h"
#include <System/StateManager.h>
#include "MenuState.h"
#include "PlayState.h"

#define GUI_SECTION "FINDSERVERSTATE"

FindServerState::FindServerState()
{
	HudObject* hudObject = new HudObject("Assets/Textures/title.png", glm::vec2(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2)), glm::vec2(1280, 720));
	m_hudHandler.insertHUDObject(hudObject, HUDID::TITLE);

	Client::getInstance()->startup();
	loadGui();
	
	m_serverListRefreshing = true;
	Client::getInstance()->refreshServerList();	
}

FindServerState::~FindServerState()
{
	removeAllRows();
	
	Gui::getInstance()->clearWidgetsInSection(GUI_SECTION);
}

void FindServerState::update(float dt)
{
	if (m_serverListRefreshing && Client::getInstance()->doneRefreshingServerList())
	{
		loadServersIntoList();
		m_serverListRefreshing = false;
	}
	//We want to clear the usernamebox upon selection
	if (m_inputTextOpen && m_usernameBox->isActive() && !m_inputTextSelected) {
		//Upon the selection frame
		m_inputTextSelected = true;
		if (m_inputTextSelected) {
			m_usernameBox->setText(""); //Clear the text box upon selection
		}
	}

	//We want to clear the ip box upon selection
	if (m_ipTextOpen && m_ipInput->isActive() && !m_ipTextSelected) {
		//Upon the selection frame
		m_ipTextSelected = true;
		if (m_ipTextSelected) {
			m_ipInput->setText(""); //Clear the text box upon selection
		}
	}	
}

void FindServerState::render()
{
	Renderer::getInstance()->renderHUD();
}

void FindServerState::loadGui()
{
	//Serverlist window
	m_serverList = static_cast<CEGUI::MultiColumnList*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/MultiColumnList", glm::vec4(0.20f, 0.25f, 0.60f, 0.40f), glm::vec4(0.0f), "serverlist"));
	m_serverList->addColumn("Server name", 0, CEGUI::UDim(0.65f, 0));
	m_serverList->addColumn("Players", 1, CEGUI::UDim(0.30f, 0));
	m_serverList->setSelectionMode(CEGUI::MultiColumnList::RowSingle);
	m_serverList->setShowHorzScrollbar(false);
	
	//Main menu
	m_backToMenu = static_cast<CEGUI::PushButton*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Button", glm::vec4(0.05f, 0.90f, 0.1f, 0.05f), glm::vec4(0.0f), "BackToMenu"));
	m_backToMenu->setText("Go back");
	m_backToMenu->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FindServerState::onBackToMenuClicked, this));

	//Join server button
	m_joinServer = static_cast<CEGUI::PushButton*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Button", glm::vec4(0.25f, 0.70f, 0.1f, 0.05f), glm::vec4(0.0f), "JoinServer"));
	m_joinServer->setText("Join");
	m_joinServer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FindServerState::onJoinServerClicked, this));


	//Refresh server list button
	m_refreshServerList = static_cast<CEGUI::PushButton*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Button", glm::vec4(0.45f, 0.70f, 0.1f, 0.05f), glm::vec4(0.0f), "RefreshServer"));
	m_refreshServerList->setText("Refresh");
	m_refreshServerList->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FindServerState::onRefreshServerListClicked, this));

	//Spectate server button
	m_spectateServer = static_cast<CEGUI::PushButton*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Button", glm::vec4(0.65f, 0.70f, 0.1f, 0.05f), glm::vec4(0.0f), "SpectateServer"));
	m_spectateServer->setText("Spectate");
	m_spectateServer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FindServerState::onSpectateServerClicked, this));

	//Connect via IP button
	m_connectViaIP = static_cast<CEGUI::PushButton*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Button", glm::vec4(0.85f, 0.70f, 0.1f, 0.05f), glm::vec4(0.0f), "ConnectViaIP"));
	m_connectViaIP->setText("Connect via IP");
	m_connectViaIP->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FindServerState::onConnectViaIPClicked, this));
	
	//The button to close the username input window
	m_backToList = static_cast<CEGUI::PushButton*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Button", glm::vec4(0.55f, 0.70f, 0.1f, 0.05f), glm::vec4(0.0f), "Close-username-input"));
	m_backToList->setText("Return");
	m_backToList->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FindServerState::onBackToListClicked, this));
	m_backToList->hide();

	//Username Input
	m_usernameBox = static_cast<CEGUI::Editbox*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Editbox", glm::vec4(0.425f, 0.55f, 0.15f, 0.05f), glm::vec4(0.0f), "username(join)"));
	m_usernameBox->setMaxTextLength(16);
	m_usernameBox->setText("Enter Username...");
	m_usernameBox->hide();	

	//IP Adress input
	m_ipInput = static_cast<CEGUI::Editbox*>(Gui::getInstance()->createWidget(GUI_SECTION, CEGUI_TYPE + "/Editbox", glm::vec4(0.425f, 0.55f, 0.15f, 0.05f), glm::vec4(0.0f), "IP-input"));
	m_ipInput->setMaxTextLength(16);
	m_ipInput->setText("Enter IP Adress...");
	m_ipInput->hide();
}

void FindServerState::loadServersIntoList()
{
	auto& servers = Client::getInstance()->getServerList();
	removeAllRows();


	for (size_t i = 0; i < servers.size(); i++) {
		m_serverList->addRow();
		CEGUI::ListboxTextItem* itemMultiColumnList;
		itemMultiColumnList = new CEGUI::ListboxTextItem(servers[i].second.serverName, servers[i].first);
		itemMultiColumnList->setSelectionBrushImage(CEGUI_TYPE + "Images" + "/GenericBrush");
		itemMultiColumnList->setSelectionColours(CEGUI::Colour(0.8f, 0.8f, 0.0f));
		m_serverList->setItem(itemMultiColumnList, 0, static_cast<CEGUI::uint>(i)); // ColumnID, RowID
		itemMultiColumnList = new CEGUI::ListboxTextItem(std::to_string(servers[i].second.connectedPlayers) + "/" + std::to_string(servers[i].second.maxPlayers), servers[i].first+1);
		m_serverList->setItem(itemMultiColumnList, 1, static_cast<CEGUI::uint>(i)); // ColumnID, RowID
		itemMultiColumnList->setSelectionColours(CEGUI::Colour(0.8f, 0.8f, 0.0f));
	}
	m_serverList->setSortColumnByID(1); 
	m_serverList->setSortDirection(CEGUI::ListHeaderSegment::SortDirection::Descending);


}

void FindServerState::removeAllRows()
{
	for (int i = m_serverList->getRowCount() - 1; i >= 0 ; i--) {
		m_serverList->removeRow(i);
	}
}

void FindServerState::usernameInput()
{
	//is write text open?
	if (!m_inputTextOpen) {
		m_serverList->hide();
		m_refreshServerList->hide();
		m_spectateServer->hide();
		m_ipInput->hide();
		m_connectViaIP->hide();
		m_joinServer->show();
		m_usernameBox->show();
		m_backToList->show();

		m_inputTextOpen = true;
	}	
}

void FindServerState::usernameInputAfterIP()
{
	//is write text open?
	if (!m_inputTextOpen) {
		m_serverList->hide();
		m_refreshServerList->hide();
		m_spectateServer->hide();
		m_ipInput->hide();
		m_connectViaIP->show();
		m_joinServer->hide();
		m_usernameBox->show();
		m_backToList->show();

		m_inputTextOpen = true;
	}
}

void FindServerState::ipInput()
{
	if (!m_ipTextOpen) {
		m_serverList->hide();
		m_refreshServerList->hide();
		m_spectateServer->hide();
		m_usernameBox->hide();
		m_joinServer->hide();
		m_connectViaIP->show();
		m_ipInput->show();
		m_backToList->show();

		m_ipTextOpen = true;
	}
}

bool FindServerState::onBackToMenuClicked(const CEGUI::EventArgs& e)
{
	Renderer::getInstance()->clear();
	m_stateManager->clearAllAndSetState(new MenuState());
	return true;
}

bool FindServerState::onJoinServerClicked(const CEGUI::EventArgs& e)
{
	CEGUI::ListboxItem* item = m_serverList->getFirstSelectedItem();
	if (item != NULL)
	{
		Gui::getInstance()->setWidgetDestRect(m_joinServer, glm::vec4(0.35f, 0.70f, 0.1f, 0.05f), glm::vec4(0.0f));
		usernameInput();
		if (m_usernameBox->getText() == "Enter Username...") {
			return false;
		}
		std::string serverName = item->getText().c_str();
		unsigned int serverID = item->getID();
		item = m_serverList->getNextSelected(item);
	
		if (Client::getInstance()->doesServerExist(serverID))
		{
			const ServerInfo& serverInfo = Client::getInstance()->getServerByID(serverID);
			Client::getInstance()->connectToAnotherServer(serverInfo, false);
			Client::getInstance()->setUsername(m_usernameBox->getText().c_str());
		}

		while (!Client::getInstance()->isConnectedToSever())
		{
			if (Client::getInstance()->connectionFailed()) {
				std::printf("Server is full or in session!\n");
				return true;
			}
		}
		SoundHandler::getInstance()->stopSound(ThemeSong0);
		SoundHandler::getInstance()->freeBuffer(ThemeSong0);
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Renderer::getInstance()->clear();
		m_stateManager->clearAllAndSetState(new PlayState(false));
	}

	return true;
}

bool FindServerState::onConnectViaIPClicked(const CEGUI::EventArgs& e)
{	
	Gui::getInstance()->setWidgetDestRect(m_connectViaIP, glm::vec4(0.35f, 0.70f, 0.1f, 0.05f), glm::vec4(0.0f));
	ipInput();	
	if (m_ipInput->getText() == "Enter IP Adress...") {
		return false;
	}
	Gui::getInstance()->setWidgetDestRect(m_joinServer, glm::vec4(0.35f, 0.70f, 0.1f, 0.05f), glm::vec4(0.0f));
	usernameInputAfterIP();
	if (m_usernameBox->getText() == "Enter Username...") {
		return false;
	}	

	ServerInfo serverInfo;
	CEGUI::String AdressAndPort = m_ipInput->getText() + "|42405";	
	serverInfo.serverAddress.FromString(AdressAndPort.c_str());
	Client::getInstance()->connectToAnotherServer(serverInfo, false);
	Client::getInstance()->setUsername(m_usernameBox->getText().c_str());

	while (!Client::getInstance()->isConnectedToSever())
	{
		if (Client::getInstance()->connectionFailed()) {
			std::printf("Server is full or in session!\n");
			return true;
		}
	}
	SoundHandler::getInstance()->stopSound(ThemeSong0);
	SoundHandler::getInstance()->freeBuffer(ThemeSong0);
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Renderer::getInstance()->clear();
	m_stateManager->clearAllAndSetState(new PlayState(false));	

	return true;
}

bool FindServerState::onRefreshServerListClicked(const CEGUI::EventArgs& e)
{
	m_serverListRefreshing = true;
	Client::getInstance()->refreshServerList();
	return true;
}

bool FindServerState::onSpectateServerClicked(const CEGUI::EventArgs& e)
{
	CEGUI::ListboxItem* item = m_serverList->getFirstSelectedItem();

	if (item != NULL)
	{
		std::string serverName = item->getText().c_str();
		unsigned int serverID = item->getID();
		item = m_serverList->getNextSelected(item);

		if (Client::getInstance()->doesServerExist(serverID))
		{
			const ServerInfo& serverInfo = Client::getInstance()->getServerByID(serverID);
			Client::getInstance()->connectToAnotherServer(serverInfo, true);
		}


		while (!Client::getInstance()->isConnectedToSever())
		{
			
			if (Client::getInstance()->connectionFailed()) {

				logTrace("Failed to connect as a spectator.. Don't know why this would happen but lets' hope that this never occurs, maybe you have the wrong version of master? :)");
				return true;
			}
		}
		SoundHandler::getInstance()->stopSound(ThemeSong0);
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Renderer::getInstance()->clear();
		m_stateManager->clearAllAndSetState(new PlayState(true));
	}


	return true;
}

bool FindServerState::onBackToListClicked(const CEGUI::EventArgs& e)
{
	//Hide the button and the username input
	m_usernameBox->hide();
	m_ipInput->hide();
	m_backToList->hide();
	//Open up the server list
	m_serverList->show();
	m_refreshServerList->show();
	m_spectateServer->show();
	m_connectViaIP->show();
	m_joinServer->show();
	m_inputTextOpen = false;
	m_ipTextOpen = false;
	Gui::getInstance()->setWidgetDestRect(m_joinServer, glm::vec4(0.25f, 0.70f, 0.1f, 0.05f), glm::vec4(0.0f));
	Gui::getInstance()->setWidgetDestRect(m_connectViaIP, glm::vec4(0.85f, 0.70f, 0.1f, 0.05f), glm::vec4(0.0f));

	m_usernameBox->setText("Enter Username...");
	m_ipInput->setText("Enter IP Adress...");
	m_inputTextSelected = false;
	m_ipTextSelected = false;
	return true;
}
