#ifndef _NETWORKGLOBALS_H
#define _NETWORKGLOBALS_H

namespace NetGlobals {

	static glm::vec3 PlayerFirstSpawnPoint = glm::vec3(0.0f, 14.5f, 7.0f);

	constexpr unsigned int MaximumConnections = 12;
	constexpr unsigned int MaximumPlayingPlayersOnServer = 4;
	constexpr unsigned short MaximumIncomingConnections = 12;
	constexpr unsigned short ServerPort = 42405;
	constexpr int TickRate = 128;
	constexpr unsigned int NetThreadSleepTime = static_cast<unsigned int>((1.0f / static_cast<float>(TickRate)) * 1000u);
	constexpr unsigned short PlayerTimeoutTimeMS = 5 * 1000;
	constexpr uint32_t WarmupCountdownTimeMS = 11 * 1000;
	constexpr uint32_t TimeBeforeRespawnMS = 5 * 1000;
	constexpr uint32_t GameRoundTimeMS = 2 * 60 * 1000;
	constexpr uint32_t InGameEndStateTimeMS = 10 * 1000;
	constexpr uint32_t PickupSpawnIntervalMS = 15 * 1000;
	constexpr uint32_t PickupNotificationBeforeSpawnMS = 8 * 1000;
	constexpr uint32_t RoutineCleanupTimeIntervalMS = 20 * 1000;
	constexpr uint32_t UpdateClientsWithServerTimeIntervalMS = 250;
	constexpr float InvulnerabilityTime = 1.0f;
	constexpr int PlayerMaxHealth = 100;
	constexpr int PlayerMaxMana = 100;
	 
	enum THREAD_FLAG {
		Remove,
		Add,
		None
	};

	enum THREAD_PLAYER_FLAG {
		SafeToAddNameplate, AlreadyAdded, NotAdded
	};

	enum SERVER_STATE {
		WaitingForPlayers = 0,
		GameIsStarting,
		GameInSession,
		GameFinished
	};

	static std::mutex DynamicTextMutex;
	static std::mutex ClientCleanupMutex;
	static std::mutex UpdatePickupsMutex;
	static std::mutex UpdatePlayersMutex;
	static std::mutex UpdateSpellsMutex;
	static std::mutex UpdatePlayerEventMutex;
	static std::mutex UpdateDeflectSpellMutex;
	static std::mutex UpdateKillFeedMutex;
	static std::mutex PickupNotificationMutex;
	static std::mutex ReadDestructableWallsMutex;

}

#endif
