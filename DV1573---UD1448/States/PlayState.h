#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H
#include <Pch/Pch.h>
#include "System/State.h"
#include <Gui/Gui.h>

class PlayState : public State {

public:
	PlayState();
	virtual ~PlayState() override;
	virtual void update(float dt) override;
	virtual void render() override;

private:
	Gui m_gui;
	int key = 1;
	Cube *m_cube;
	Renderer* m_renderer;
	ShaderMap* m_shaderMap;
	Mesh m_mesh;

};



#endif