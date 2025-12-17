#include "global.h"
#include "ScreenMarketplace.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "EconomyManager.h"
#include "InputEventPlus.h"
#include "GameSoundManager.h"

REGISTER_SCREEN_CLASS( ScreenMarketplace );

void ScreenMarketplace::Init()
{
	ScreenWithMenuElements::Init();

	// Mock Inventory
	m_Items.push_back( { 1, "Song License: Butterfly", "Music", 500, false } );
	m_Items.push_back( { 2, "Song License: B4U", "Music", 450, false } );
	m_Items.push_back( { 3, "Company Share (DAO)", "Stock", 1000, false } );
	m_Items.push_back( { 4, "Gym Skin: Neon", "Cosmetic", 200, false } );
	m_Items.push_back( { 5, "Bob's Sword (Cross-Game)", "Item", 5000, false } );

	m_textTitle.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textTitle.SetText( "ASSET MARKETPLACE" );
	m_textTitle.SetXY( 320, 40 );
	m_textTitle.SetDiffuse( RageColor(1,0,1,1) ); // Purple
	this->AddChild( &m_textTitle );

	m_textBalance.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textBalance.SetXY( 320, 70 );
	m_textBalance.SetDiffuse( RageColor(0,1,0,1) );
	this->AddChild( &m_textBalance );

	m_textStatus.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textStatus.SetXY( 320, 400 );
	m_textStatus.SetZoom( 0.8f );
	this->AddChild( &m_textStatus );

	m_iSelection = 0;
	RefreshList();
}

void ScreenMarketplace::RefreshList()
{
	long long balance = EconomyManager::Instance()->GetBalance("WALLET_PLAYER");
	m_textBalance.SetText( ssprintf("Balance: %lld Tokens", balance) );

	// Clear old
	for( auto* p : m_pRowTexts ) {
		this->RemoveChild( p );
		delete p;
	}
	m_pRowTexts.clear();

	float startY = 120;
	for( size_t i=0; i < m_Items.size(); ++i )
	{
		MarketItem& item = m_Items[i];
		BitmapText* pText = new BitmapText;
		pText->LoadFromFont( THEME->GetPathF("Common", "normal") );

		std::string sPrice = item.owned ? "OWNED" : ssprintf("%lld", item.price);
		std::string sEntry = ssprintf( "%s  [%s]  %s", item.name.c_str(), item.category.c_str(), sPrice.c_str() );

		pText->SetText( sEntry );
		pText->SetXY( 320, startY + (i * 30) );

		if( (int)i == m_iSelection ) pText->SetDiffuse( RageColor(1,1,0,1) );
		else if( item.owned ) pText->SetDiffuse( RageColor(0.5f,0.5f,0.5f,1) );
		else pText->SetDiffuse( RageColor(1,1,1,1) );

		this->AddChild( pText );
		m_pRowTexts.push_back( pText );
	}
}

void ScreenMarketplace::BuyItem( int index )
{
	MarketItem& item = m_Items[index];
	if( item.owned ) return;

	if( EconomyManager::Instance()->Transfer("WALLET_PLAYER", "WALLET_MARKETPLACE", item.price, "Buy: " + item.name) )
	{
		item.owned = true;
		SOUND->PlayOnce( THEME->GetPathS("Common", "coin") );
		m_textStatus.SetText( "Purchase Successful!" );
		m_textStatus.SetDiffuse( RageColor(0,1,0,1) );
	}
	else
	{
		SOUND->PlayOnce( THEME->GetPathS("Common", "cancel") );
		m_textStatus.SetText( "Insufficient Funds." );
		m_textStatus.SetDiffuse( RageColor(1,0,0,1) );
	}
	RefreshList();
}

void ScreenMarketplace::Input( const InputEventPlus &input )
{
	if( input.type != IET_FIRST_PRESS ) return;

	if( input.MenuI == GAME_BUTTON_UP )
	{
		if( m_iSelection > 0 ) m_iSelection--;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
		RefreshList();
	}
	else if( input.MenuI == GAME_BUTTON_DOWN )
	{
		if( m_iSelection < (int)m_Items.size() - 1 ) m_iSelection++;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
		RefreshList();
	}
	else if( input.MenuI == GAME_BUTTON_START )
	{
		BuyItem( m_iSelection );
	}
	else if( input.MenuI == GAME_BUTTON_BACK )
	{
		SCREENMAN->SetNewScreen( "ScreenUnifiedDashboard" );
	}
}
