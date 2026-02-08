#include "global.h"
#include "ScreenServerNode.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "EconomyManager.h"
#include "InputEventPlus.h"
#include "RageUtil.h"

// Local UUID helper
static std::string MakeUUID()
{
	std::string res = "";
	for(int i=0; i<32; ++i) res += ssprintf("%x", RandomInt(16));
	return res;
}

REGISTER_SCREEN_CLASS( ScreenServerNode );

void ScreenServerNode::Init()
{
	ScreenWithMenuElements::Init();

	m_textStatus.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textStatus.SetText( "SERVER NODE: ACTIVE" );
	m_textStatus.SetXY( 320, 50 );
	m_textStatus.SetDiffuse( RageColor(0, 0.8f, 1, 1) ); // Cyan
	this->AddChild( &m_textStatus );

	m_textConsole.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textConsole.SetXY( 100, 100 );
	m_textConsole.SetHorizAlign( align_left );
	m_textConsole.SetZoom( 0.6f );
	this->AddChild( &m_textConsole );

	m_textEarnings.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textEarnings.SetXY( 320, 400 );
	this->AddChild( &m_textEarnings );

	m_fLogTimer = 0;
	m_fJobTimer = 0;
	m_sCurrentJob = "IDLE";
	m_LogLines.push_back( "Initializing Distributed Compute Protocol..." );
}

void ScreenServerNode::Update( float fDeltaTime )
{
	ScreenWithMenuElements::Update( fDeltaTime );

	// Simulate Job Scheduler
	m_fJobTimer += fDeltaTime;
	if( m_fJobTimer > 3.0f )
	{
		m_fJobTimer = 0;
		int r = RandomInt(3);
		if( r == 0 ) m_sCurrentJob = "AI TRAINING: [FWBer Model v2]";
		else if( r == 1 ) m_sCurrentJob = "RENDER FARM: [bob's game Asset #92]";
		else m_sCurrentJob = "SWARM SEEDING: [DDR 1st Mix]";
	}

	// Simulate Log Activity
	m_fLogTimer += fDeltaTime;
	if( m_fLogTimer > 0.2f ) // Fast scroll
	{
		m_fLogTimer = 0;
		// Generate fake hash
		std::string hash = "0x" + MakeUUID().substr(0, 16);
		m_LogLines.push_back( m_sCurrentJob + " :: " + hash );

		if( m_LogLines.size() > 15 )
			m_LogLines.erase( m_LogLines.begin() );

		std::string sDisplay;
		for( const auto& line : m_LogLines )
			sDisplay += line + "\n";

		m_textConsole.SetText( sDisplay );
	}

	// Update Earnings Display
	long long earnings = EconomyManager::Instance()->GetMiningReward();
	m_textEarnings.SetText( ssprintf("Session Earnings: %lld Coins", earnings) );
}

void ScreenServerNode::Input( const InputEventPlus &input )
{
	if( input.type == IET_FIRST_PRESS )
	{
		char c = input.DeviceI.ToChar();
		if( c >= 32 && c <= 126 )
		{
			m_sInputBuffer += c;
			// Update last log line to show typing if we wanted, but for now just buffer
		}
		else if( input.DeviceI.button == KEY_ENTER || input.DeviceI.button == KEY_KP_ENTER )
		{
			ProcessCommand(m_sInputBuffer);
			m_sInputBuffer = "";
		}
		else if( input.MenuI == GAME_BUTTON_BACK )
		{
			SCREENMAN->SetNewScreen( "ScreenUnifiedDashboard" );
		}
	}
}

void ScreenServerNode::ProcessCommand(const std::string& cmd)
{
	m_LogLines.push_back( "> " + cmd );

	if( cmd == "status" )
	{
		m_LogLines.push_back( "SYSTEM: ONLINE. Hashrate: 45 MH/s. Peers: 12." );
	}
	else if( cmd == "claim" )
	{
		long long reward = EconomyManager::Instance()->GetMiningReward();
		m_LogLines.push_back( ssprintf("SYSTEM: Claimed %lld pending tokens.", reward) );
	}
	else if( cmd == "sync" )
	{
		m_LogLines.push_back( "SYSTEM: Forcing Swarm Sync..." );
		// In real logic, call ContentSwarmManager::ForceSync()
	}
	else if( cmd == "help" )
	{
		m_LogLines.push_back( "COMMANDS: status, claim, sync, help" );
	}
	else if( !cmd.empty() )
	{
		m_LogLines.push_back( "Unknown command." );
	}
}
