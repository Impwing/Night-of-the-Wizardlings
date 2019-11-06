#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H
#include <Pch/Pch.h>
#include <System/State.h>

#include <GUI/Gui.h>
#include <GFX/MaterialMap.h>
#include <GameObject/GameObject.h>
#include <Player/Player.h>
#include <GameObject/WorldObject.h>
#include <GameObject/AnimatedObject.h>
#include <Spells/Spell.h>
#include <System/BulletPhysics.h>
#include <Renderer/HudObject.h>


bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1,
	const btCollisionObjectWrapper* obj2, int id2, int index2);

class PlayState : public State {

public:
	PlayState();
	virtual ~PlayState() override;
	virtual void update(float dt) override;
	virtual void render() override;

private:
	/* Callbacks */
	void onSpellHit_callback();

private:
	int key = 1;

	bool m_GUIOpen;
	
	//Any inherited GameObject class added to this vector will support basic rendering
	std::vector<GameObject*> m_objects;

	SpellHandler* m_spellHandler;
	Player* m_player;
	Camera* m_camera;
	SkyBox* m_skybox;
	DeflectRender* m_deflectBox;
	BulletPhysics* m_bPhysics;
	std::vector<HudObject*> m_icons;
	HudObject* m_crosshairHUD;
	HudObject* m_damageOverlay;
	HudObject* m_deflectCrosshairHUD;
	HudObject* m_hitCrosshair;
	HudObject* m_hpBar;

	CEGUI::PushButton* m_mainMenu;
	CEGUI::PushButton* m_quit;
	CEGUI::MultiColumnList* m_scoreBoard;
	bool m_scoreboardExists;
	bool m_endGameBoardVisible;

	float hp = 1.0f; //Temp HP
	 
private: 

	bool onMainMenuClick(const CEGUI::EventArgs& e);
	bool onQuitClick(const CEGUI::EventArgs& e);
	
	void GUIHandler();
	void GUILoadScoreboard();
	void GUILoadButtons();
	void GUIclear();
};



#endif