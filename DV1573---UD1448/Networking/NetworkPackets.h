#ifndef _NET_PACKETS_H
#define _NET_PACKETS_H
#include <GameObject/ObjectTypes.h>

#include <Mesh/MeshFormat.h>
enum {
	INFO_ABOUT_OTHER_PLAYERS = ID_USER_PACKET_ENUM + 1,
	PLAYER_ACCEPTED_TO_SERVER,
	PLAYER_NOT_ACCEPTED_TO_SERVER,
	PLAYER_JOINED,
	PLAYER_DISCONNECTED,
	PLAYER_UPDATE_PACKET,
	SERVER_CURRENT_STATE,
	SERVER_CHANGE_STATE,
	ADMIN_PACKET,
	SPELL_ALL_EXISTING_SPELLS,
	SPELL_CREATED,
	SPELL_UPDATE,
	SPELL_DESTROY,
	SPELL_PLAYER_HIT,
	GAME_START_COUNTDOWN,
	GAME_ROUND_TIMER,
	RESPAWN_TIME,
	RESPAWN_PLAYER_DURING_SESSION,
	RESPAWN_PLAYER_NOT_IN_SESSION,
	GIVE_PLAYER_FULL_HEALTH,
	SCORE_UPDATE,
	SPELL_GOT_DEFLECTED,
	PICKUP_CREATED,
	PICKUP_REMOVED,
	PICKUP_NOTIFICATION,
	HEAL_BUFF,
	DAMAGE_BUFF_ACTIVE,
	DAMAGE_BUFF_INACTIVE,
	KILL_FEED,
	SERVER_TIME,
	SPECTATE_REQUEST,
	PLAY_REQUEST,
	DESTRUCTION,
	READY_PACKET,
	NUMBER_OF_READY_PLAYERS,
	UNREADY_PACKET_FROM_SERVER

};

/* To make sure the compiler aligns the bits */
#pragma pack(push, 1)
struct ServerInfo {
	
	char serverName[16] = { ' ' };
	RakNet::SystemAddress serverAddress;
	unsigned short maxPlayers;
	unsigned short connectedPlayers;
	NetGlobals::SERVER_STATE currentState;
};
#pragma pack(pop)
struct newPlayerInfo {
	RakNet::AddressOrGUID guid;
};


struct PlayerPacket {
	RakNet::AddressOrGUID guid;
	RakNet::AddressOrGUID lastHitByGuid;
	uint32_t timestamp = 0;

	bool Spectator = false;
	int health = NetGlobals::PlayerMaxHealth;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 lookDirection = glm::vec3(0.0f);
	glm::vec3 latestSpawnPosition = glm::vec3(0.0f);
	glm::vec3 meshHalfSize = glm::vec3(0.0f);
	char userName[16] = { ' ' };
	int numberOfKills = 0;
	int numberOfDeaths = 0;
	bool inDeflectState = false;
	bool isReady = false;
	bool hasBeenUpdatedOnce = false;
	bool onGround = true;

	AnimationState animStates;
	bool hasDamageBuff = false;

	void Serialize(bool writeToStream, RakNet::BitStream& stream)
	{
		stream.Serialize(writeToStream, *this);
		/*stream.Serialize(writeToStream, guid);
		stream.Serialize(writeToStream, lastHitByGuid);
		stream.Serialize(writeToStream, timestamp);
		stream.Serialize(writeToStream, health);
		stream.Serialize(writeToStream, position);
		stream.Serialize(writeToStream, rotation);
		stream.Serialize(writeToStream, lookDirection);
		stream.Serialize(writeToStream, latestSpawnPosition);
		stream.Serialize(writeToStream, meshHalfSize);
		stream.Serialize(writeToStream, userName);
		stream.Serialize(writeToStream, numberOfKills);
		stream.Serialize(writeToStream, numberOfDeaths);
		stream.Serialize(writeToStream, inDeflectState);
		stream.Serialize(writeToStream, hasBeenUpdatedOnce);
		stream.Serialize(writeToStream, animStates);
		stream.Serialize(writeToStream, hasDamageBuff);*/
	}
};

struct CountdownPacket {
	uint32_t timeLeft;

	void Serialize(bool writeToStream, RakNet::BitStream& stream)
	{
		stream.Serialize(writeToStream, *this);
	}
};

struct RoundTimePacket {

	uint32_t minutes;
	uint32_t seconds;

	void Serialize(bool writeToStream, RakNet::BitStream& stream)
	{
		stream.Serialize(writeToStream, *this);
	}

};

struct KillFeedPacket {
	RakNet::AddressOrGUID killerGuid;
	RakNet::AddressOrGUID deadGuid;

	void Serialize(bool writeToStream, RakNet::BitStream& stream)
	{
		stream.Serialize(writeToStream, *this);
	}
};

struct SpellPacket{
	RakNet::MessageID packetType;
	uint64_t SpellID = 0;
	uint32_t timestamp = 0;
	RakNet::RakNetGUID CreatorGUID = RakNet::UNASSIGNED_RAKNET_GUID;
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 LastPosition = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::vec3 Direction = glm::vec3(0.0f);
	
	uint64_t SoundSlot = 0;
	OBJECT_TYPE SpellType = OBJECT_TYPE::UNKNOWN;



	void Serialize(bool writeToStream, RakNet::BitStream& stream) {
		stream.Serialize(writeToStream, *this);
		/*stream.Serialize(writeToStream, packetType);
		stream.Serialize(writeToStream, SpellID);
		stream.Serialize(writeToStream, timestamp);
		stream.Serialize(writeToStream, CreatorGUID);
		stream.Serialize(writeToStream, Position);
		stream.Serialize(writeToStream, LastPosition);
		stream.Serialize(writeToStream, Rotation);
		stream.Serialize(writeToStream, Scale);
		stream.Serialize(writeToStream, Direction);
		stream.Serialize(writeToStream, SpellType);*/
	}


};

struct HitPacket {

	uint64_t SpellID = 0;
	RakNet::RakNetGUID CreatorGUID = RakNet::UNASSIGNED_RAKNET_GUID;
	RakNet::RakNetGUID playerHitGUID = RakNet::UNASSIGNED_RAKNET_GUID;
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::vec3 SpellDirection = glm::vec3(0.0f);
	glm::quat Rotation = glm::quat();
	float damage = 0.0f;
	OBJECT_TYPE SpellType = OBJECT_TYPE::UNKNOWN;

	void Serialize(bool writeToStream, RakNet::BitStream& stream) {
		stream.Serialize(writeToStream, *this);
		/*stream.Serialize(writeToStream, SpellID);
		stream.Serialize(writeToStream, CreatorGUID);
		stream.Serialize(writeToStream, playerHitGUID);
		stream.Serialize(writeToStream, Position);
		stream.Serialize(writeToStream, Scale);
		stream.Serialize(writeToStream, SpellDirection);
		stream.Serialize(writeToStream, Rotation);
		stream.Serialize(writeToStream, damage);
		stream.Serialize(writeToStream, SpellType);*/
	}


};

struct PickupPacket {
	char locationName[16] = { ' ' };
	uint64_t uniqueID;
	glm::vec3 position;
	PickupType type;

	void Serialize(bool writeToStream, RakNet::BitStream& stream) {
		stream.Serialize(writeToStream, *this);
		/*stream.Serialize(writeToStream, locationName);
		stream.Serialize(writeToStream, uniqueID);
		stream.Serialize(writeToStream, position);
		stream.Serialize(writeToStream, type);*/
	}
};

struct ServerStateChange {
	NetGlobals::SERVER_STATE currentState;

	void Serialize(bool writeToStream, RakNet::BitStream& stream) {
		stream.Serialize(writeToStream, *this);
		/*stream.Serialize(writeToStream, currentState);*/
	}
};

struct ServerTimePacket {
	uint32_t serverTimestamp;

	void Serialize(bool writeToStream, RakNet::BitStream& stream) {
		stream.Serialize(writeToStream, *this);
	}
};


struct DestructionPacket {

	int randomSeed;
	int index;
	glm::vec2 hitPoint;
	glm::vec3 hitDir;

	void Serialize(bool writeToStream, RakNet::BitStream& stream) {
		stream.Serialize(writeToStream, *this);
		/*stream.Serialize(writeToStream, randomSeed);
		stream.Serialize(writeToStream, index);
		stream.Serialize(writeToStream, hitPoint); 
		stream.Serialize(writeToStream, hitDir);*/
	}


};

struct ReadyPacket {
	RakNet::RakNetGUID guid = RakNet::UNASSIGNED_RAKNET_GUID;
	
	void Serialize(bool writeToStream, RakNet::BitStream& stream) {
		stream.Serialize(writeToStream, *this);
	}
};

struct ReadyPlayersCount {
	int numberOfReadyPlayers = 0;

	void Serialize(bool writeToStream, RakNet::BitStream& stream) {
		stream.Serialize(writeToStream, *this);
	}
};


#endif