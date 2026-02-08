#include "global.h"
#include "ScreenWalletHistory.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "InputEventPlus.h"
#include "RageUtil.h"

REGISTER_SCREEN_CLASS( ScreenWalletHistory );

void ScreenWalletHistory::Init()
{
	ScreenWithMenuElements::Init();

	m_iScrollOffset = 0;
	m_History = EconomyManager::Instance()->GetRecentTransactions();

	m_textTitle.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textTitle.SetText( "WALLET HISTORY" );
	m_textTitle.SetXY( 320, 40 );
	this->AddChild( &m_textTitle );

	// Create reusable rows (e.g. 10 visible)
	for( int i=0; i<10; ++i )
	{
		BitmapText* pText = new BitmapText;
		pText->LoadFromFont( THEME->GetPathF("Common", "normal") );
		pText->SetXY( 320, 100 + (i * 30) );
		pText->SetZoom( 0.6f );
		this->AddChild( pText );
		m_pRowTexts.push_back( pText );
	}

	// Initial Draw
	Input( InputEventPlus() ); // Reuse logic to draw
}

void ScreenWalletHistory::Input( const InputEventPlus &input )
{
	if( input.type == IET_FIRST_PRESS )
	{
		if( input.MenuI == GAME_BUTTON_DOWN )
		{
			if( m_iScrollOffset < (int)m_History.size() - 10 ) m_iScrollOffset++;
		}
		else if( input.MenuI == GAME_BUTTON_UP )
		{
			if( m_iScrollOffset > 0 ) m_iScrollOffset--;
		}
		else if( input.MenuI == GAME_BUTTON_BACK )
		{
			SCREENMAN->SetNewScreen( "ScreenUnifiedDashboard" );
			return;
		}
	}

	// Draw
	for( int i=0; i<10; ++i )
	{
		int dataIndex = m_iScrollOffset + i;
		if( dataIndex < (int)m_History.size() )
		{
			const Transaction& tx = m_History[dataIndex];
			std::string sLine = ssprintf( "[%s] %lld (%s -> %s)",
				tx.reason.c_str(), tx.amount, tx.from.c_str(), tx.to.c_str() );
			m_pRowTexts[i]->SetText( sLine );
		}
		else
		{
			m_pRowTexts[i]->SetText( "" );
		}
	}
}
