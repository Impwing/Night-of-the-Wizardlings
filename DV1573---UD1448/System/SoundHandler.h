#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H
#include <Pch/Pch.h>
#include <OpenAl/al.h>
#include <OpenAL/alc.h>
#include <Networking/Client.h>

//Set custom names for easier reference to soundFile names
const std::string THEME_SONG0 = "HarryPotterThemeSong.ogg";
const std::string BASIC_ATTACK_SOUND = "BasicSpell2.ogg";
const std::string DEFLECT_SOUND = "YouShallNotPassogg.ogg";
const std::string ENHANCE_ATTACK_SOUND = "Magic sound effect.ogg";
const std::string TAKING_DAMAGE_SOUND = "TakingDamage.ogg";
const std::string STEPS_SOUND = "FootSteps2.ogg";
const std::string JUMP_SOUND = "Jump1.ogg";
const std::string HITMARK_SOUND = "Hitmark.ogg";

//Nr of sounds every player has in common.
const int NR_OF_COMMON_SOUNDS = 5;
//Nr of sounds only the client will hear.
const int NR_OF_CLIENT_SOUNDS = 4;
//Nr of sounds in total
const int NR_OF_SOUNDS = 9;
//Nr of sounds from the same source that can be played subsequently
const int NR_OF_SUBSEQUENT_SOUNDS = 3;

//Put sounds that you will hear from yourself and other players as a SoundIndexCommon enum.
//For example, you can hear other players' spells.
//Put sounds that will come only from this client as a SoundIndexClient enum.
//Also, make sure to increase NR_OF_CLIENT_SOUNDS or NR_OF_COMMON_SOUNDS depending 
//on what type of sound you added. And, of course, increase the number of total sounds aswell. 

//Sounds in common here:
enum SoundIndexCommon {		
	BasicAttackSound,
	DeflectSound,
	EnhanceAttackSound,	
	StepsSound,
	JumpSound	
};

//Sounds only for client here:
enum SoundIndexClient {	
	ThemeSong0,
	PickupSpawnSound,
	HitmarkSound,
	TakingDamageSound
};

struct PlayerSoundInfo
{
	RakNet::AddressOrGUID guid;	
	std::vector<std::vector<ALuint>> sources;
};

struct ClientSoundInfo
{	
	std::vector<ALuint> sources;
};

class SoundHandler
{
private:	
	static SoundHandler* m_soundHandlerInstance;
	ALCdevice* m_device;
	ALCcontext* m_context;	
	std::vector<ALuint> m_buffersCommon;
	std::vector<ALuint> m_buffersClient;
	std::vector<PlayerSoundInfo> m_playerSoundInfo;	
	ClientSoundInfo m_clientSoundInfo;	
	ALenum m_error;
	int m_nrOfPlayers; // Including me

public:
	SoundHandler();
	~SoundHandler();

	static SoundHandler* getInstance();

	void destroy();

	void loadAllSound();
	int loadSound(SoundIndexClient whatSound);
	int loadSound(SoundIndexCommon whatSound);

	void setPlayerSourceGains(RakNet::AddressOrGUID guid);
	
	void attachBuffersToClientSources();
	int attachBuffersToPlayerSources(RakNet::AddressOrGUID playerID);

	void playSound(SoundIndexClient whatSound);
	void playSound(SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);

	void pauseSound(SoundIndexClient whatSound);
	void pauseSound(SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);

	void stopSound(SoundIndexClient whatSound);
	void stopSound(SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);

	void setListenerPos(glm::vec3 pos);
	void setListenerVelocity(glm::vec3 vel);
	void setListenerOrientation(glm::vec3 lookAt, glm::vec3 up);

	void setSourcePitch(float pitch, SoundIndexClient whatSound);
	void setSourcePitch(float pitch, SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);

	void setSourceGain(float gain, SoundIndexClient whatSound);
	void setSourceGain(float gain, SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);
	
	void setSourceMaxGain(float gain, SoundIndexClient whatSound);
	void setSourceMaxGain(float gain, SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);
	
	void setSourceMaxDistance(float dist, SoundIndexClient whatSound);
	void setSourceMaxDistance(float dist, SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);
	
	void setSourcePosition(glm::vec3 pos, SoundIndexClient whatSound);
	void setSourcePosition(glm::vec3 pos, SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);
	
	void setSourceVelocity(glm::vec3 vel, SoundIndexClient whatSound);
	void setSourceVelocity(glm::vec3 vel, SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);

	void setSourceDirection(glm::vec3 dir, SoundIndexClient whatSound);
	void setSourceDirection(glm::vec3 dir, SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);

	void setSourceType(ALenum type, SoundIndexClient whatSound); //AL_UNDETERMINED, AL_STATIC or AL_STREAMING
	void setSourceType(ALenum type, SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID); //AL_UNDETERMINED, AL_STATIC or AL_STREAMING

	void setSourceLooping(bool looping, SoundIndexClient whatSound);
	void setSourceLooping(bool looping, SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID);
	
	void setPlayerGUIDs();
	void addPlayer(RakNet::AddressOrGUID guid);
	void removePlayer(RakNet::AddressOrGUID guid);

	const ALint& getSourceState(SoundIndexClient whatSound) const;
	const ALint& getSourceState(SoundIndexCommon whatSound, RakNet::AddressOrGUID playerID, int slot = 0) const;
	//source relative?	
};

#endif
