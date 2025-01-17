#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H
#include <Pch/Pch.h>
#include <System/State.h>
#include <GUI/Gui.h>
#include <GFX/MaterialMap.h>
#include <GameObject/GameObject.h>
#include <Player/Player.h>
#include <GameObject/WorldObject.h>
#include <GameObject/MapObject.h>
#include <GameObject/AnimatedObject.h>
#include <GameObject/DestructibleObject.h>
#include <Spells/Spell.h>
#include <System/BulletPhysics.h>
#include <HUD/HudObject.h>
#include <HUD/HudHandler.h>
#include <GFX/Pointlight.h>
#include <System/MemoryUsage.h>
#include <HUD/WorldHudObject.h>

//bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1,
//	const btCollisionObjectWrapper* obj2, int id2, int index2);

class PlayState : public State {

public:
	//PlayState(){}
	PlayState(bool spectator);
	void loadMap();
	void loadSkyDebris(Renderer* renderer);
	void loadDecor();
	void loadLights();
	void loadDestructables();
	virtual ~PlayState() override;
	virtual void update(float dt) override;
	void removeDeadObjects();
	virtual void render() override;

	static bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1,
		const btCollisionObjectWrapper* obj2, int id2, int index2);	

	void InitParticle();
private:
	/* Callbacks */
	void onSpellHit_callback();

	/* Helper */
	void update_isPlaying(const float& dt);
	void update_isSpectating(const float& dt);

private:
	int key = 1;	// Delete this??

	// Hardcoded solution for choosing the map
	// Should probably be changed depending on how we want to handle different maps
	// 0 is default
	// 1 is debug
	int m_map = 0;

	float m_rotVal;
	bool m_GUIOpen;
	
	//Any inherited GameObject class added to this vector will support basic rendering
	std::vector<GameObject*> m_objects;
	std::vector<Pointlight*> m_pointlights;

	DstrGenerator m_dstr;
	DstrGenerator m_dstr_alt1;

	//AnimatedObject* m_firstPerson;
	SpellHandler* m_spellHandler;
	Player* m_player;
	Camera* m_camera;
	SkyBox* m_skybox;
	MemoryUsage mu;
	HudHandler m_hudHandler;
	
	CEGUI::PushButton* m_mainMenu;
	CEGUI::PushButton* m_quit;
	CEGUI::MultiColumnList* m_scoreBoard;
	bool m_scoreboardExists;
	bool m_endGameBoardVisible;
	bool m_hideHUD;

	glm::vec3 m_lastPositionOfMyKiller;
	GUIText* m_gameTimeText;
	GUIText* m_readyText;
	GUIText* m_numberOfPlayersReadyText;
	GUIText* m_killNotification;
	GUIText* m_memoryText;
	GUIText* m_fpsText;
	float startY;

	ParticleBuffers* deathBuffer;
private: 

	bool onMainMenuClick(const CEGUI::EventArgs& e);
	bool onQuitClick(const CEGUI::EventArgs& e);

	void HUDHandler();
	void GUIHandler();
	void GUILoadScoreboard();
	void GUILoadButtons();
	void GUIclear();
};



#endif