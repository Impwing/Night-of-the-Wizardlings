#include "Pch/Pch.h"
#include "Player.h"


Player::Player(std::string name, glm::vec3 playerPosition, Camera *camera)
{
	if (camera == NULL) {
		 playerCamera = new Camera();
	}
	this->playerCamera = camera;
	this->playerPosition = playerPosition;
	this->name = name;
	this->speed = 5;
	this->health = 100;
	this->attackCooldown = 0;
	this->nrOfSpells = 0;
	normalSpell.reserve(5);
}

Player::~Player()
{
	for (AttackSpell* object : normalSpell)
		delete object;
	delete playerCamera;
}

void Player::update(float deltaTime)
{

	glm::vec3 camFace = playerCamera->getCamFace();
	glm::vec3 camRight = playerCamera->getCamRight();

	camFace.y = 0;

	moveDir = glm::vec3(0.0f);

	if (glfwGetKey(playerCamera->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		moveDir -= camRight;
	}
	if (glfwGetKey(playerCamera->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		moveDir += camRight;
	}
	if (glfwGetKey(playerCamera->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		moveDir += camFace;
	}
	if (glfwGetKey(playerCamera->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		moveDir -= camFace;
	}

	if (glm::length(moveDir) >= 0.1f)
		moveDir = glm::normalize(moveDir);
	
	inputVector = moveDir;
	playerPosition += inputVector * speed * deltaTime;
	setPlayerPos(playerPosition);
	playerCamera->setCameraPos(playerPosition);
	playerCamera->update(playerCamera->getWindow());
	attack(deltaTime);
	std::cout << " " << camRight.x << " " << camRight.y << " " << camRight.z << std::endl;
}

void Player::attack(float deltaTime)
{
	if (glfwGetMouseButton(playerCamera->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && attackCooldown <= 0)
	{
		if (nrOfSpells < 5)
		{

			normalSpell.push_back(new AttackSpell("Spell", playerPosition));
			normalSpell[normalSpell.size() - 1]->loadMesh("SexyCube.mesh");
			normalSpell[normalSpell.size() - 1]->setWorldPosition(playerPosition);
			normalSpell[normalSpell.size() - 1]->translate(glm::vec3());
			attackCooldown = 1.0f;
			nrOfSpells++;
		}
		else
		{
			for (int i = 0; i < nrOfSpells; i++)
			{
				delete normalSpell[i];
			}
			nrOfSpells = 0;
			normalSpell.clear();
		}
		
	}
	if(attackCooldown > 0)
		attackCooldown = attackCooldown - 1 * deltaTime;

	for (AttackSpell* object : normalSpell)
	{
		object->bindMaterialToShader("Basic_Forward");
		Renderer::getInstance()->render(*object);
	}
}

void Player::setPlayerPos(glm::vec3 pos)
{
	this->playerPosition = pos;
}

void Player::spawnPlayer(glm::vec3 pos)
{
	this->playerPosition = pos;
}

void Player::setHealth(int health)
{
	this->health = health;
}

void Player::setSpeed(float speed)
{
	this->speed = speed;
}

glm::vec3 Player::getPlayerPos() const
{
	return this->playerPosition;
}

int Player::getHealth() const
{
	return this->health;
}

Camera* Player::getCamera()
{
	return playerCamera;
}

std::string Player::getName() const
{
	return this->name;
}

bool Player::isDead()
{
	if (this->health <= 0)
	{
		return true;
	}
	else
		return false;
}
