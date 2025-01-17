#ifndef _PCH_H
#define _PCH_H


#include <System/DeltaTime.h>
#include <System/Framerate.h>
#include "Bullet/btBulletDynamicsCommon.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <limits.h>
#include <time.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <math.h>
#include <chrono>
#include <thread>
#include <string>
#include <stdio.h>
#include <future>
#include <functional>

#define WIN32_LEAN_AND_MEAN

#include <d3d11_4.h>
#include <dxgi1_6.h>



#pragma comment(lib, "dxgi.lib")

#include <psapi.h>

//GL related includes
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/common.hpp>
#include "glm/ext.hpp"
#include <System/Lerp.h>

#include "System/Log.h"
#include "System/Randomizer.h"

//Raknet
#include <raknet/MessageIdentifiers.h>
#include <raknet/RakPeerInterface.h>
#include <raknet/RakNetTypes.h>
#include <raknet/RakSleep.h>
#include <raknet/GetTime.h>
#include <raknet/BitStream.h>
#include <RakNet/NetworkIDObject.h>
#include <RakNet/NetworkIDManager.h>

#include <Pickups/PickupTypes.h>
#include <Networking/NetworkGlobals.h>
#include <Networking/NetworkPackets.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>


//Custom
#include <Spells/SpellFormat.h>
#include <HUD/HudTextureMap.h>
#include <Mesh/MeshFormat.h>
#include <Mesh/Mesh.h>
#include <Mesh/MeshMap.h>
#include <Mesh/SkeletonMap.h>
#include <Mesh/AnimationMap.h>
#include <Renderer/Shader.h>
#include <Renderer/ShaderMap.h>
#include <Renderer/Camera.h>
#include <Renderer/Renderer.h>
#include <System/Input.h>
#include <System/SoundHandler.h>
#include <GameObject/ObjectTypes.h>	

// DSTR
#include <Destruction/TriangleNode.h>
#include <Destruction/Geometry.h>
#include <Destruction/DelaunayGenerator.h>
#include <Destruction/VoroniCalculator.h>
#include <Destruction/VoroniClipper.h>
#include <Destruction/DstrGenerator.h>

// ImGui
//#include <imgui/imgui.h>
//#include <imgui/imgui_impl_glfw_gl3.h>
//#include <stdio.h>
//#include <GL/glew.h> 
//#include <GLFW/glfw3.h>
//#include <imgui/imfilebrowser.h>


const std::string CEGUI_TYPE = "OgreTray";
const std::string SHADERPATH = "Assets/Shaders/";
const std::string TEXTUREPATH = "Assets/Textures/";
const std::string MESHPATH = "Assets/Meshes/";
const std::string SOUNDEFFECTPATH = "Assets/SoundEffects/";
const std::string CHARACTER = "CharacterBody";

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;
const int MAX_BONES = 64;

constexpr int SPACE_ASCII = 32;
constexpr int DESIRED_PADDING = 6;
constexpr float LINE_HEIGHT = 0.3f;

#endif
