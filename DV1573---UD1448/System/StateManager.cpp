#include <Pch/Pch.h>
#include "StateManager.h"


StateManager::StateManager()
{
	m_soundHandler = new SoundHandler();
	m_soundHandler->loadSound("Assets/SoundEffects/HarryPotterThemeSong.ogg");
	m_soundHandler->setSourceType(AL_STREAMING);
	m_soundHandler->playSound();
}


StateManager::~StateManager()
{
	clearStates();
	clearKillList();

	delete m_soundHandler;
}

void StateManager::popState()
{
	if (m_states.size() > 1) {
		
		m_killList.emplace_back(m_states[m_states.size() - 1]);
		m_states.pop_back();

	}
	else {
		logWarning("Trying to remove the only state left in the stack!");
	}
}

void StateManager::pushState(State* newState)
{
	newState->assignManager(this);
	m_states.emplace_back(newState);
	
}

void StateManager::clearAllAndSetState(State* newState)
{
	if (newState != nullptr) {
		clearStates();
		newState->assignManager(this);
		m_states.emplace_back(newState);		
	}
	else {
		logError("Trying to push a new state which is a nullptr!");
	}
}

void StateManager::update(float dt)
{
	// Clear and remove the states that is present in the kill list
	clearKillList();

	if (!m_states.empty()) {
		m_states.back()->update(dt);

		if (m_states.back()->getShouldMusicPlay() == false)
		{
			m_soundHandler->stopSound();
		}
	}

}

void StateManager::render()
{
	if (!m_states.empty()) {
		m_states.back()->render();
	}
}

void StateManager::clearStates()
{
	for (size_t i = 0; i < m_states.size(); i++) {
		m_killList.emplace_back(m_states[i]);
	}
	m_states.clear();
}

void StateManager::clearKillList()
{
	for (size_t i = 0; i < m_killList.size(); i++) {
		delete m_killList[i];
	}
	m_killList.clear();
}
