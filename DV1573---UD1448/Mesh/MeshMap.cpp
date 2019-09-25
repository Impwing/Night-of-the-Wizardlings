#include "Pch/Pch.h"
#include "MeshMap.h"

MeshMap* MeshMap::m_MeshMapInstance = 0;

MeshMap::MeshMap() {}

MeshMap* MeshMap::getInstance()
{
	if (m_MeshMapInstance == 0) {
		m_MeshMapInstance = new MeshMap();
	}
	return m_MeshMapInstance;
}

void MeshMap::cleanUp()
{
	std::map<std::string, Mesh*>::iterator it;

	for (it = m_Mesh.begin(); it != m_Mesh.end(); it++) {
		delete it->second;
	}

	m_Mesh.clear();
}

bool MeshMap::existsWithName(std::string name)
{
	if (m_Mesh.find(name) != m_Mesh.end()) {
		return true;
	}
	return false;
}


void MeshMap::useByName(std::string name)
{
	//if (existsWithName(name)) {
	//	m_Meshs[name]->use();
	//}
}

Mesh* MeshMap::getMesh(std::string name)
{
	if (existsWithName(name))
	{
		return m_Mesh[name];
	}

	return nullptr;
}

Mesh* MeshMap::createMesh(std::string name, Mesh Mesh)
{
	if (existsWithName(name))
	{
		return nullptr;
	}
	Mesh* newMesh = new Mesh();
	*newMesh = Mesh;
	m_Mesh[name] = newMesh;
	return newMesh;
}

void MeshMap::destroy()
{
	cleanUp();
	delete m_MeshMapInstance;
}
