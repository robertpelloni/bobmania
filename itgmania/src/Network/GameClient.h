#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <string>
#include <vector>
#include <functional>
#include "RageThreads.h"

enum ClientState {
	STATE_DISCONNECTED,
	STATE_CONNECTING,
	STATE_LOBBY,
	STATE_GAME
};

// Represents a remote game server connection
class GameClient
{
public:
	static GameClient* Instance();
	static void Destroy();

	GameClient();
	~GameClient();

	void Connect( const std::string& host, int port );
	void Login( const std::string& user, const std::string& pass );
	void Disconnect();
	bool IsConnected() const;

	void Update( float fDeltaTime ); // Poll socket

	// Callbacks
	void SetMessageCallback( std::function<void(const std::string&)> cb ) { m_OnMessage = cb; }

	// Game Actions
	void SendChat( const std::string& msg );
	void QueueMatch( const std::string& playlist );
	void SpectateMatch( const std::string& matchID );

private:
	bool m_bConnected;
	ClientState m_State;
	std::string m_Host;
	int m_Port;
	float m_fPingTimer;

	std::function<void(const std::string&)> m_OnMessage;

	static GameClient* s_pInstance;
};

#endif
