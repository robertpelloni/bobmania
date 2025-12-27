#include "global.h"
#include "GameClient.h"
#include "RageLog.h"

GameClient* GameClient::s_pInstance = NULL;

GameClient* GameClient::Instance()
{
	if( !s_pInstance )
		s_pInstance = new GameClient;
	return s_pInstance;
}

void GameClient::Destroy()
{
	delete s_pInstance;
	s_pInstance = NULL;
}

GameClient::GameClient()
{
	m_bConnected = false;
	m_Port = 0;
}

GameClient::~GameClient()
{
	Disconnect();
}

void GameClient::Connect( const std::string& host, int port )
{
	LOG->Info( "Connecting to Game Server at %s:%d...", host.c_str(), port );
	m_Host = host;
	m_Port = port;

	// Stub: Simulate successful connection
	m_bConnected = true;
	LOG->Info( "Connected!" );
}

void GameClient::Disconnect()
{
	if( m_bConnected )
	{
		LOG->Info( "Disconnecting from Game Server..." );
		m_bConnected = false;
	}
}

bool GameClient::IsConnected() const
{
	return m_bConnected;
}

void GameClient::Update( float fDeltaTime )
{
	if( !m_bConnected ) return;

	// Stub: Simulate keeping alive / reading packets
}

void GameClient::SendChat( const std::string& msg )
{
	if( !m_bConnected ) return;
	LOG->Info( "TX Chat: %s", msg.c_str() );
}

void GameClient::QueueMatch( const std::string& playlist )
{
	if( !m_bConnected ) return;
	LOG->Info( "TX QueueMatch: %s", playlist.c_str() );
}

void GameClient::SpectateMatch( const std::string& matchID )
{
	if( !m_bConnected ) return;
	LOG->Info( "TX Spectate: %s", matchID.c_str() );
}
