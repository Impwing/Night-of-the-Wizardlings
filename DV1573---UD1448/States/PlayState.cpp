#include <Pch/Pch.h>
#include "PlayState.h"

// TODO move to mesh
#include <Networking/Client.h>

PlayState::PlayState()
{
	m_bPhysics = new BulletPhysics(-10);
	m_spellHandler = new SpellHandler(m_bPhysics);
	ShaderMap::getInstance()->getShader(BASIC_FORWARD)->setInt("albedoTexture", 0);
	Renderer::getInstance();
	m_camera = new Camera();
	m_player = new Player(m_bPhysics, "Player", glm::vec3(0.0f, 1.8f, 0.0f), m_camera, m_spellHandler);

	Renderer::getInstance()->setupCamera(m_player->getCamera());

	//TODO: organized loading system?
	m_skybox = new SkyBox();
	m_skybox->prepareBuffers();

	//Test enviroment with 4 meshes inside 1 GameObject, inherited transforms
	m_objects.push_back(new WorldObject("TestScene"));
	m_objects[m_objects.size() - 1]->loadMesh("TestScene.mesh");
	m_objects[m_objects.size() - 1]->setWorldPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	Renderer::getInstance()->submit(m_objects[m_objects.size() - 1], STATIC);
	
	//Cube and sphere centered in scene
	m_objects.push_back(new WorldObject("TestCube"));
	m_objects[m_objects.size() - 1]->loadMesh("TestCube.mesh");
	m_objects[m_objects.size() - 1]->setWorldPosition(glm::vec3(5.0f, 0.0f, 0.0f));
	Renderer::getInstance()->submit(m_objects[m_objects.size() - 1], STATIC);


	m_objects.push_back(new WorldObject("TestSphere"));
	m_objects[m_objects.size() - 1]->loadMesh("TestSphere.mesh");
	m_objects[m_objects.size() - 1]->setWorldPosition(glm::vec3(10.0f, 2.0f, -4.0f));
	//m_objects[m_objects.size() - 1]->setWorldPosition(glm::vec3(5.0f, 1.0f, -2.0f));
	Renderer::getInstance()->submit(m_objects[m_objects.size() - 1], STATIC);

	m_objects.push_back(new WorldObject("TestCube"));
	m_objects[m_objects.size() - 1]->loadMesh("TestCube.mesh");
	m_objects[m_objects.size() - 1]->setWorldPosition(glm::vec3(10.0f, 2.0f, -1.0f));
	Renderer::getInstance()->submit(m_objects[m_objects.size() - 1], STATIC);
	
	//////Animated rectangle
	//m_objects.push_back(new AnimatedObject("TestRectangle"));
	//m_objects[m_objects.size() - 1]->loadMesh("TestRectangle.mesh");
	//m_objects[m_objects.size() - 1]->setWorldPosition(glm::vec3(0.0f, 0.0f, -4.0f));
	//Renderer::getInstance()->submit(m_objects[m_objects.size() - 1], ANIMATEDSTATIC);
	//
	////Animated goblino
	m_firstPerson = new AnimatedObject("TestFps");
	m_firstPerson->loadMesh("FirstPersonMesh.mesh");

	m_objects.push_back(m_firstPerson);
	//m_objects[m_objects.size() - 1]->loadMesh("FirstPersonMesh.mesh");
	Transform tempTransform;
	tempTransform.scale = glm::vec3(0.53f, 0.53f, 0.53f);
	//tempTransform.position = glm::vec3(-3.0f, 0.0f, 3.0f);
	//m_objects[m_objects.size() - 1]->setTransform(tempTransform);
	Renderer::getInstance()->submit(m_objects[m_objects.size() - 1], ANIMATEDSTATIC);

	CollisionObject obj = box;
	m_bPhysics->createObject(obj, 0.0f, glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(100.0f, 2.0f, 100.0f), 1.0);
	gContactAddedCallback = callbackFunc;

	for (size_t i = 1; i < m_objects.size(); i++)
	{
		Transform temp = m_objects.at(i)->getTransform();

		m_bPhysics->createObject(obj, 0.0f, temp.position,
			glm::vec3(temp.scale.x/2, temp.scale.y, temp.scale.y/2));
	}
	logTrace("Playstate created");
}

PlayState::~PlayState()
{
	logTrace("Deleting playstate..");
	for (GameObject* object : m_objects)
		delete object;
	delete m_skybox;
	delete m_player;
	delete m_bPhysics;
	delete m_spellHandler;

	MaterialMap::getInstance()->destroy();
	MeshMap::getInstance()->destroy();
	AnimationMap::getInstance()->destroy();
	SkeletonMap::getInstance()->destroy();
}

void PlayState::update(float dt)
{	
	Client::getInstance()->updateNetworkEntities(dt);
	m_bPhysics->update(dt);
	Renderer::getInstance()->update(dt);
	m_spellHandler->spellUpdate(dt);
	m_player->update(dt);
	m_firstPerson->setWorldPosition(m_camera->getPosition());

	//m_firstPerson.set
	for (GameObject* object : m_objects)
	{
		object->update(dt);
	}


	if (Input::isKeyPressed(GLFW_KEY_P)) {
		auto& list = Client::getInstance()->getNetworkSpells();
		logTrace("Active spells on client: {0}", list.size());
	}
}

void PlayState::render()
{
	//Move the render skybox to be a private renderer function
	Renderer::getInstance()->renderSkybox(*m_skybox);
	Renderer::getInstance()->render();
	m_spellHandler->renderSpell();

}

//This function is called everytime two collision objects collide
bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1,
	const btCollisionObjectWrapper* obj2, int id2, int index2)
{
	return false;
}
