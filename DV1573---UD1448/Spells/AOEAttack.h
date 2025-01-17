#pragma once
#include <Pch/Pch.h>
#include <GameObject/GameObject.h>
#include <GameObject/WorldObject.h>
#include <Spells/Spell.h>
#include <Spells/fire.h>

class AOEAttack : public Spell
{
public:
	AOEAttack(glm::vec3 pos, glm::vec3 direction, const SpellBase* spellBase);
	AOEAttack(glm::vec3 pos);
	~AOEAttack();

	// Virtual functions
	virtual const bool& getHasCollided() const;
	virtual void hasCollided();
	virtual void update(float dt);
	virtual const float getDamage();
	void updateTool(float radius, float speed, float dt);

	bool spellOnGround();
	void setSpellBool(bool state);


private:
	glm::vec3 gravityVector = glm::vec3(0,-1,0);
	glm::vec3 newVer = glm::vec3(0, 0, 0);
	glm::vec3 tempVer = glm::vec3(0, 0, 0);
	glm::vec3 radiusVector = glm::vec3(0, 0, 0);

	float fireIsGone;
	bool loadFire = true;
	bool test123 = false;
	bool m_updateSpellPos = true;
	bool m_fire = false;

	//new---
	bool m_hasCollided = false;
	int m_bounceCounter = 0;
	float m_bounceTime = 0.0f;
};