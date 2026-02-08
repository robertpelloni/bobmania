#include "global.h"
#include "ScreenEvaluationBetting.h"
#include "EconomyManager.h"
#include "ThemeManager.h"
#include "GameState.h"
#include "StageStats.h"
#include "RageLog.h"
#include "../Security/ScoreSigner.h"

REGISTER_SCREEN_CLASS( ScreenEvaluationBetting );

void ScreenEvaluationBetting::Init()
{
	// Call parent Init first to set up standard evaluation UI
	ScreenEvaluation::Init();

	// Custom Betting Logic
	if( EconomyManager::Instance()->IsBetActive() )
	{
		// Determine Win Condition
		// For MVP: Passing the song with > 80% accuracy is a "Win" against the House.
		// In a real PvP match, we would compare m_pStageStats->m_player[PLAYER_1].m_iScore vs Rival.

		bool bPlayerWon = false;
		PlayerNumber pn = GAMESTATE->GetMasterPlayerNumber();

		if( !m_pStageStats->m_player[pn].m_bFailed )
		{
			float fPercent = m_pStageStats->m_player[pn].GetPercentDancePoints();
			if( fPercent > 0.80f )
			{
				bPlayerWon = true;
			}
		}

		// Resolve the bet

		// Anti-Cheat Check
		HighScoreData hsData;
		hsData.score = m_pStageStats->m_player[pn].m_iScore;
		hsData.percent = m_pStageStats->m_player[pn].GetPercentDancePoints();
		hsData.playerID = "PLAYER_ID"; // Mock
		hsData.timestamp = time(NULL);

		std::string sig = ScoreSigner::SignScore(hsData);
		if( !ScoreSigner::VerifyScore(hsData, sig) )
		{
			LOG->Warn("Anti-Cheat: Invalid Score Signature! Bet Voided.");
			m_textBetResult.SetText( "SECURITY ALERT: INVALID SCORE" );
			return;
		}

		EconomyManager::Instance()->ResolveMatchBet( bPlayerWon );

		// Update Elo (Assuming a standard "Gold" opponent of 1200 for now, or match it to a selected rival)
		EconomyManager::Instance()->UpdateElo( bPlayerWon, 1300 ); // Beating the "House" is like beating a slightly stronger opponent

		// Display Result
		m_textBetResult.LoadFromFont( THEME->GetPathF("Common", "header") );
		m_textBetResult.SetXY( 320, 400 );

		if( bPlayerWon )
		{
			m_textBetResult.SetText( "BET WON! Payout Received." );
			m_textBetResult.SetDiffuse( RageColor(0,1,0,1) ); // Green
			SOUND->PlayOnce( THEME->GetPathS("Common", "coin") );
		}
		else
		{
			m_textBetResult.SetText( "BET LOST. House Wins." );
			m_textBetResult.SetDiffuse( RageColor(1,0,0,1) ); // Red
		}

		this->AddChild( &m_textBetResult );

		LOG->Trace("ScreenEvaluationBetting: Resolved bet. Won: %d", bPlayerWon);
	}
}
