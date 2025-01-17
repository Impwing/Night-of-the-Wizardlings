#ifndef _SPELL_CREATOR_STATE_H
#define _SPELL_CREATOR_STATE_H
#include <Pch/Pch.h>
#include <System/State.h>
#include <Mesh/MeshFormat.h>
#include <imgui/imgui.h>
#include <imgui/imfilebrowser.h>

#include <SpellLoader/spellLoader.h>

//class SpellLoader;

//bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1,
//	const btCollisionObjectWrapper* obj2, int id2, int index2);


class SpellCreatorState : public State {

public:

	SpellCreatorState();
	virtual ~SpellCreatorState() override;
	virtual void update(float dt) override;
	void Update(float dt);
	virtual void render() override;

	void loadTexture();
    void updateToolSettings(OBJECT_TYPE type);

    //-----Edit spell Functions-----//
    void editAttackSpell();
	void editAOEAttackSpell();

    //-----Edit spell Events function-----//
    void editSpellEvents();
    void setSpellEvents(int eventNr);

    //IMGUI
	bool is_ImGui() { return true; }
	void guiInfo();

	bool my_tool_active = true;
    bool isProjectile = false;
	bool isAOE = false;
    bool isEnhanceProjectile = false;
    SpellLoader myLoader;
    bool loadASpell = false;
    //-----test data-----//
    std::string m_name = "fireball";
	SpellLoading::Projectile normalSpell;
    SpellLoading::AOESpell aoeSpell;

	//particles
	PSinfo tempPS;
	TextureInfo tempTxt;

	SpellLoading::SpellEvents spellEvents;
	SpellLoading::SpellEvents nrOfParticleSystems;
    //-----File opener/Path to Exports-----//
	ImGui::FileBrowser fileDialog;
	ImGui::FileBrowser textureDialog;
	
    char m_spellName[256] = "newSpell";

private:

    Player* m_player;
    SpellHandler* m_spellHandler;
	SpellEditor* m_spellEditor;
    BulletPhysics* m_bPhysics;
    std::vector<Pointlight*> m_pointlights;
    SkyBox* m_skybox;
    Camera* m_camera;
    std::vector<GameObject*> m_objects;

    AttackSpell* normalAttSpell;

	std::string m_textureName;

    bool m_AttackSpellAlive = true;
    bool m_FireSpellAlive = true;

	bool shouldRenderSpell;
	int m_emission = 1;

	OBJECT_TYPE m_Type = NORMALATTACK;
};
#endif



// Notes
// # Added _CRT_SECURE_NO_WARNINGS in: Properties/ C/C++ / Preprocessor
// # Changed the language standard in: Properties/ C/C++ / Language
