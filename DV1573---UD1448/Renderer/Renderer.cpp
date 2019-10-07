#include <Pch/Pch.h>
#include "Renderer.h"

#define LIGHT_CULL "Light_Cull"
#define BASIC_FORWARD "Basic_Forward"
#define DEPTH_MAP "Depth_Map"
#define SKYBOX "Skybox_Shader"

#define TILE_SIZE 16

Renderer* Renderer::m_rendererInstance = 0;

Renderer::Renderer()
{
	m_gWindow = nullptr;
	m_camera = nullptr;

	createDepthMap();
	int x = -10;
	int z = -40;

	m_pLights.reserve(P_LIGHT_COUNT);
	for (int i = 0; i < P_LIGHT_COUNT; i++) {
		Pointlight pL;
		pL.position = glm::vec3(x += 15, 2.5f, 0);

		pL.radius = 5.0f;
		pL.attenuation = glm::vec3(1.0f, 0.09f, 0.032f);
		m_pLights.push_back(pL);
	}
	//Define Work Groups
	workGroups.x = (SCREEN_WIDTH + (SCREEN_WIDTH % TILE_SIZE)) / TILE_SIZE;
	workGroups.y = (SCREEN_HEIGHT + (SCREEN_HEIGHT % TILE_SIZE)) / TILE_SIZE;
	//INIT SHADER_STORAGE_BUFFER_OBJECT 
	glGenBuffers(1, &m_lightIndexSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_lightIndexSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(LightIndex), 0, GL_STATIC_DRAW);
	//GL related calls
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::createDepthMap() {

	//Create a depth map texture for the rendering system
	glGenFramebuffers(1, &m_depthFBO);

	glGenTextures(1, &m_depthMap);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1, 1, 1, 1 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//HDR and a sperate colour buffer
	glGenFramebuffers(1, &m_hdrFbo);

	glGenTextures(1, &m_colourBuffer);
	glBindTexture(GL_TEXTURE_2D, m_colourBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Inits the shaders for forward+
	initShaders();
}

void Renderer::initShaders() {
	ShaderMap::getInstance()->createShader(DEPTH_MAP, "Depth.vert", "Depth.frag");
	//Set the light index binding
	ShaderMap::getInstance()->createShader(LIGHT_CULL, "LightCullCompute.comp");
	ShaderMap::getInstance()->useByName(LIGHT_CULL);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_lightIndexSSBO);
	
	//ShaderMap::getInstance()->createShader(BASIC_FORWARD, "VertexShader.vert", "FragShader.frag");
}

void Renderer::bindMatrixes(const std::string& shaderName) {
	ShaderMap::getInstance()->getShader(shaderName)->setMat4("viewMatrix", m_camera->getViewMat());
	ShaderMap::getInstance()->getShader(shaderName)->setMat4("projMatrix", m_camera->getProjMat());
}

Renderer* Renderer::getInstance()
{
	if (m_rendererInstance == 0) {
		m_rendererInstance = new Renderer();
	}
	return m_rendererInstance;
}

void Renderer::init(GLFWwindow* window)
{
	m_gWindow = window;
}

void Renderer::setupCamera(Camera* camera)
{
	if (camera == nullptr) {
		return;
	}
	m_camera = camera;
}

void Renderer::submit(GameObject* gameObject, ObjectType objType)
{
	if (objType == STATIC) {
		m_staticObjects.push_back(gameObject);
	}
	else if (objType == DYNAMIC) {
		m_dynamicObjects.push_back(gameObject);
	}
}


void Renderer::destroy()
{
	delete m_rendererInstance;
}

void Renderer::bindMatrixes(const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
	//TODO: Also being done in render()
	ShaderMap::getInstance()->getShader("Basic_Forward")->setMat4("viewMatrix", m_camera->getViewMat());
	ShaderMap::getInstance()->getShader("Basic_Forward")->setMat4("projectionMatrix", m_camera->getProjMat());
}

void Renderer::renderSkybox(const SkyBox& skybox)
{
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	//glDepthMask(false);
	ShaderMap::getInstance()->useByName("Skybox_Shader");
	ShaderMap::getInstance()->getShader("Skybox_Shader")->setMat4("viewMatrix", glm::mat4(glm::mat3(m_camera->getViewMat())));
	ShaderMap::getInstance()->getShader("Skybox_Shader")->setMat4("projectionMatrix", m_camera->getProjMat());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getCubeMapTexture());
	glBindVertexArray(skybox.getVAO());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	//glDepthMask(true);
	glEnable(GL_CULL_FACE);
}

void Renderer::renderDepth(const GameObject& gameObject, const int& meshIndex) {
	ShaderMap::getInstance()->useByName(DEPTH_MAP);

	//Bind and draw the objects to the depth-buffer
	bindMatrixes(DEPTH_MAP);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	Mesh* mesh = MeshMap::getInstance()->getMesh(gameObject.getMeshN(meshIndex));
	Transform transform = gameObject.getTransform(meshIndex);

	glBindVertexArray(mesh->getBuffers().vao);

	glm::mat4 modelMatrix = glm::mat4(1.0f);

	/* !!!THIS IS NOT SUPPOSED TO HAPPEN IN THE RENDER FUNCTION!!!! */
	modelMatrix = glm::translate(modelMatrix, transform.position);
	modelMatrix = glm::scale(modelMatrix, transform.scale);
	modelMatrix *= glm::mat4_cast(transform.rotation);

	ShaderMap::getInstance()->getShader(DEPTH_MAP)->setMat4("modelMatrix", modelMatrix);

	glDrawElements(GL_TRIANGLES, mesh->getBuffers().nrOfFaces * 3, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderColor(const GameObject& gameObject, const int& meshIndex) {

	ShaderMap::getInstance()->useByName(BASIC_FORWARD);

	Mesh* meshRef = MeshMap::getInstance()->getMesh(gameObject.getMeshN(meshIndex));
	const Transform meshTransform = gameObject.getTransform(meshIndex);

	glBindVertexArray(meshRef->getBuffers().vao);

	glm::mat4 worldMatrix = glm::mat4(1.0f);
	worldMatrix = glm::translate(worldMatrix, meshTransform.position);
	worldMatrix = glm::scale(worldMatrix, meshTransform.scale);
	worldMatrix *= glm::mat4_cast(meshTransform.rotation);

	//Set matrices
	bindMatrixes(BASIC_FORWARD);
	ShaderMap::getInstance()->getShader(BASIC_FORWARD)->setMat4("modelMatrix", worldMatrix);

	//Drawcall
	glDrawElements(GL_TRIANGLES, meshRef->getBuffers().nrOfFaces * 3, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}

void Renderer::update(float dt) {
	m_camera->fpsControls(dt);
	m_camera->update(m_gWindow);
}

void Renderer::render(const GameObject& gameObject, int meshIndex) {

	//Render to the depth buffer
	renderDepth(gameObject, meshIndex);
	
	renderColor(gameObject, meshIndex);
}


Camera* Renderer::getMainCamera() const
{
	return m_camera;
}

