#include "global.h"
#include "ScreenGameplayCommercial.h"
#include "ScreenManager.h"
#include "EconomyManager.h"
#include "GameState.h"
#include "Song.h"
#include "RageLog.h"

REGISTER_SCREEN_CLASS( ScreenGameplayCommercial );

void ScreenGameplayCommercial::Init()
{
	ScreenGameplayNormal::Init();

	// Commercial Logic: Pay Royalty
	Song* pSong = GAMESTATE->m_pCurSong;
	if( pSong )
	{
		std::string sTitle = pSong->GetTranslitMainTitle();
		std::string sArtist = pSong->GetTranslitArtist();

		EconomyManager::Instance()->PaySongRoyalty( sTitle, sArtist );

		LOG->Trace( "Commercial Mode: Paid royalty for %s", sTitle.c_str() );
	}
}
