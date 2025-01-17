#ifndef PARTICLE_H
#define PARTICLE_H

#include <Pch/Pch.h>
#include <Renderer/Camera.h>
#include <streambuf>
//#include <Particles/ParticleBuffers.h>

class ParticleBuffers;


struct TextureInfo //Information for the texture that we apply to the particles
{
	int width;
	int height;
	std::string name;
	int nrChannels; //Do I actually need this?
};

struct Particle //Basic particle data
{
	float distance;
	float time;

	bool startLoop;
	bool isAlive;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 rotation;
	glm::vec3 velocity;
};

struct PSinfo //All the fun particle information values
{
	glm::vec3 direction;
	glm::vec3 color;
	glm::vec3 blendColor;

	int maxParticles;
	bool cont;
	bool omnious;
	int seed;
	int fade;
	int scaleDirection;
	float glow;
	int swirl;
	bool randomSpawn;

	float width;
	float heigth;
	float lifetime;
	float emission;
	float spread;
	float force;
	float drag;
	float gravity;
};

class ParticleSystem
{
private:
	Camera* m_camera;

	TextureInfo m_txtInfo;
	PSinfo m_psInfo;

	glm::vec3 m_position;
	glm::vec3 m_direction;

	glm::mat4 m_quad;
	glm::mat4 m_lookAt;

	std::vector<glm::vec3> m_vertex;
	std::vector<glm::vec3> m_directionVector;

	std::vector<Particle> m_particle;
	std::vector<float> m_lifetime;

	GLuint m_texture;
	GLuint m_vao;
	GLuint m_vertexBuffer;
	GLuint m_directionalBuffer;
	GLuint m_lifetimeBuffer;
	GLuint m_shader;
	GLuint m_vertexPosition;
	GLuint m_vertexDirection;
	GLuint m_vertexLife;

	bool m_isPlaying;
	bool m_cont;

	float m_current;

	int m_nrOfActive;


public:
	//unsigned int shader is not used!
	ParticleSystem(ParticleBuffers* particleBuffers);
	//ParticleSystem();
	~ParticleSystem();

	PSinfo GetParticleInfo();
	TextureInfo GetTextureInfo();

	glm::mat4 GetQuad();

	bool Initialize();
	bool Build(PSinfo psInfo);
	bool Texture(TextureInfo txtInfo);
	bool Plays();

	int GetNrOfParticles();

	void Update(float time); //removed glm::vec3 cameraPos
	void TempInit(PSinfo psInfo);
	void Render(Camera* camera);
	void Pause();
	void Play();


	void SetPosition(glm::vec3 pos);
};

#endif