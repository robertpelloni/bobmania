#include "global.h"
#include "GameClient.h"
#include "RageLog.h"
#include "RageUtil.h"

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
	m_State = STATE_DISCONNECTED;
	m_Port = 0;
	m_fPingTimer = 0;
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
	m_State = STATE_CONNECTING;

	// Stub: Simulate successful connection
	m_bConnected = true;
	m_State = STATE_LOBBY; // Auto-lobby for MVP
	LOG->Info( "Connected!" );
}

void GameClient::Login( const std::string& user, const std::string& pass )
{
	if( !m_bConnected ) Connect("127.0.0.1", 9000);
	LOG->Info("Logging in as %s...", user.c_str());
	// Stub
}

void GameClient::Disconnect()
{
	if( m_bConnected )
	{
		LOG->Info( "Disconnecting from Game Server..." );
		m_bConnected = false;
		m_State = STATE_DISCONNECTED;
	}
}

bool GameClient::IsConnected() const
{
	return m_bConnected;
}

void GameClient::Update( float fDeltaTime )
{
	if( !m_bConnected ) return;

	// Mock Server Heartbeat / Traffic
	m_fPingTimer += fDeltaTime;
	if( m_fPingTimer > 4.0f )
	{
		m_fPingTimer = 0;
		if( m_OnMessage ) {
			// Mock incoming chat
			int r = RandomInt(3);
			if( r==0 ) m_OnMessage("Server: Live Event starting soon!");
			if( r==1 ) m_OnMessage("Lobby: Player 'Speedy' joined.");
		}
	}
}

void GameClient::SendChat( const std::string& msg )
{
	if( !m_bConnected ) return;
	LOG->Info( "TX Chat: %s", msg.c_str() );

	// Echo back for MVP
	if( m_OnMessage ) m_OnMessage( "YOU: " + msg );
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
