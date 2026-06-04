#include "global.h"
#include "ScreenManageHiddenSongs.h"
#include "ScreenManager.h"
#include "SongManager.h"
#include "ProfileManager.h"
#include "Profile.h"
#include "GameState.h"
#include "ThemeManager.h"
#include "ActorUtil.h"
#include "ScreenPrompt.h"
#include "InputEventPlus.h"

static const char* HIDDEN_SONG_ACTION_NAMES[] = {
	"Unhide",
	"Exit",
};
XToString( HiddenSongAction );
#define FOREACH_HiddenSongAction( hsa ) FOREACH_ENUM_N( HiddenSongAction, NUM_HiddenSongAction, hsa )

REGISTER_SCREEN_CLASS( ScreenManageHiddenSongs );

ScreenManageHiddenSongs::ScreenManageHiddenSongs()
{
}

void ScreenManageHiddenSongs::Init()
{
	ScreenWithMenuElements::Init();

	m_MusicWheel.SetName( "MusicWheel" );
	m_MusicWheel.Load( "MusicWheel" );
	LOAD_ALL_COMMANDS_AND_SET_XY( m_MusicWheel );
	this->AddChild( &m_MusicWheel );

	m_textInstructions.LoadFromFont( THEME->GetPathF(m_sName,"instructions") );
	m_textInstructions.SetName( "Instructions" );
	LOAD_ALL_COMMANDS_AND_SET_XY( m_textInstructions );
	this->AddChild( &m_textInstructions );
}

void ScreenManageHiddenSongs::BeginScreen()
{
	ScreenWithMenuElements::BeginScreen();
	UpdateSongList();
}

void ScreenManageHiddenSongs::EndScreen()
{
	ScreenWithMenuElements::EndScreen();
}

void ScreenManageHiddenSongs::Update( float fDeltaTime )
{
	ScreenWithMenuElements::Update( fDeltaTime );
}

bool ScreenManageHiddenSongs::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS )
		return false;

	if( input.MenuI == GAME_BUTTON_START )
	{
		OnSongSelected();
		return true;
	}

	return ScreenWithMenuElements::Input( input );
}

void ScreenManageHiddenSongs::HandleScreenMessage( const ScreenMessage SM )
{
	if( SM == SM_GoToPrevScreen || SM == SM_GoToNextScreen )
	{
		SCREENMAN->SetNewScreen( THEME->GetMetric (m_sName,"NextScreen" ) );
	}
}

void ScreenManageHiddenSongs::UpdateSongList()
{
	std::vector<Song*> vpSongs;
	Profile* pProfile = PROFILEMAN->GetProfile(GAMESTATE->m_MasterPlayerNumber);
	if (pProfile)
	{
		const std::set<SongID>& hiddenSongs = pProfile->GetHiddenSongIDs();
		for (const SongID& songID : hiddenSongs)
		{
			Song* pSong = songID.ToSong();
			if (pSong)
			{
				vpSongs.push_back(pSong);
			}
		}
	}
	m_MusicWheel.SetSongs( vpSongs );
}

void ScreenManageHiddenSongs::OnSongSelected()
{
	if (m_MusicWheel.GetSelectedSong() == nullptr)
	{
		SCREENMAN->SetNewScreen( THEME->GetMetric (m_sName,"NextScreen" ) );
		return;
	}

	Song* pSong = m_MusicWheel.GetSelectedSong();
	Profile* pProfile = PROFILEMAN->GetProfile(GAMESTATE->m_MasterPlayerNumber);
	if (pProfile)
	{
		pProfile->RemoveFromHidden(pSong->GetSongID());
		PROFILEMAN->SaveProfile(GAMESTATE->m_MasterPlayerNumber);
		UpdateSongList();
	}
}
