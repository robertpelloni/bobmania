#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <string>
#include <vector>
#include "RageThreads.h"

// Represents a remote game server connection
class GameClient
{
public:
	static GameClient* Instance();
	static void Destroy();

	GameClient();
	~GameClient();

	void Connect( const std::string& host, int port );
	void Disconnect();
	bool IsConnected() const;

	void Update( float fDeltaTime ); // Poll socket

	// Game Actions
	void SendChat( const std::string& msg );
	void QueueMatch( const std::string& playlist );
	void SpectateMatch( const std::string& matchID );

private:
	bool m_bConnected;
	std::string m_Host;
	int m_Port;

	static GameClient* s_pInstance;
};

#endif
