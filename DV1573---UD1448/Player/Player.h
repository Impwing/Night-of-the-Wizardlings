#pragma once
#include <Pch/Pch.h>
#include <Spells/AttackSpell.h>

class Player
{
public:
	Player(std::string name = "", glm::vec3 playerPosition = glm::vec3(0.0f, 0.0f, 0.0f), Camera* camera = NULL);
	~Player();

	void update(float deltaTime);
	void playerJump();
	void move(float deltaTime);
	void attack(float deltaTime);
	void updateAttack(float deltaTime);
	void createRay(); //create ray for spells
	void renderSpell();
	void spawnPlayer(glm::vec3 pos);
	bool isDead();
	Camera* getCamera();

	//-----Get-----//
	glm::vec3 getPlayerPos() const;
	int getHealth() const;
	std::string getName() const;

	//-----Set-----//
	void setPlayerPos(glm::vec3 pos);
	void setHealth(int health);
	void setSpeed(float speed);

private:
	std::vector<AttackSpell> normalSpell;
	glm::vec3 directionVector;
	glm::vec3 playerPosition;
	glm::vec3 inputVector;
	glm::vec3 moveDir;
	AttackSpell* tempSpell;
	Camera* playerCamera;
	float attackCooldown;
	float spellSpeed = 1;
	float speed;
	int nrOfSpells;
	int health;
	std::string name;

};