#include "global.h"
#include "ScreenProfileCustomize.h"
#include "ScreenManager.h"
#include "ThemeManager.h"
#include "InputEventPlus.h"
#include "GameSoundManager.h"

REGISTER_SCREEN_CLASS( ScreenProfileCustomize );

void ScreenProfileCustomize::Init()
{
	ScreenWithMenuElements::Init();

	m_textTitle.LoadFromFont( THEME->GetPathF("Common", "header") );
	m_textTitle.SetText( "PROFILE CUSTOMIZATION" );
	m_textTitle.SetXY( 320, 40 );
	this->AddChild( &m_textTitle );

	m_textStatus.LoadFromFont( THEME->GetPathF("Common", "normal") );
	m_textStatus.SetXY( 320, 400 );
	m_textStatus.SetZoom( 0.8f );
	this->AddChild( &m_textStatus );

	m_iSelection = 0;
	RefreshList();
}

void ScreenProfileCustomize::RefreshList()
{
	// Fetch owned items
	m_Inventory = EconomyManager::Instance()->GetInventory();

	// Clear old
	for( auto* p : m_pRowTexts ) {
		this->RemoveChild( p );
		delete p;
	}
	m_pRowTexts.clear();

	float startY = 100;
	if (m_Inventory.empty()) {
		BitmapText* pText = new BitmapText;
		pText->LoadFromFont( THEME->GetPathF("Common", "normal") );
		pText->SetText( "No items owned. Visit Marketplace!" );
		pText->SetXY( 320, 200 );
		this->AddChild( pText );
		m_pRowTexts.push_back( pText );
		return;
	}

	for( size_t i=0; i < m_Inventory.size(); ++i )
	{
		Asset& item = m_Inventory[i];
		BitmapText* pText = new BitmapText;
		pText->LoadFromFont( THEME->GetPathF("Common", "normal") );

		std::string currentlyEquipped = EconomyManager::Instance()->GetEquippedAsset(item.type);
		std::string status = (currentlyEquipped == item.name) ? "[EQUIPPED]" : "";

		std::string sEntry = ssprintf( "%s %s (%s)", status.c_str(), item.name.c_str(), item.type.c_str() );

		pText->SetText( sEntry );
		pText->SetXY( 320, startY + (i * 30) );

		if( (int)i == m_iSelection ) pText->SetDiffuse( RageColor(1,1,0,1) );
		else if( !status.empty() ) pText->SetDiffuse( RageColor(0,1,0,1) );
		else pText->SetDiffuse( RageColor(1,1,1,1) );

		this->AddChild( pText );
		m_pRowTexts.push_back( pText );
	}
}

void ScreenProfileCustomize::EquipItem( int index )
{
	if (index < 0 || index >= (int)m_Inventory.size()) return;

	Asset& item = m_Inventory[index];
	EconomyManager::Instance()->EquipAsset(item.type, item.name);

	SOUND->PlayOnce( THEME->GetPathS("Common", "start") );
	m_textStatus.SetText( "Equipped: " + item.name );
	RefreshList();
}

void ScreenProfileCustomize::Input( const InputEventPlus &input )
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
		if( m_iSelection < (int)m_Inventory.size() - 1 ) m_iSelection++;
		SOUND->PlayOnce( THEME->GetPathS("Common", "change") );
		RefreshList();
	}
	else if( input.MenuI == GAME_BUTTON_START )
	{
		EquipItem( m_iSelection );
	}
	else if( input.MenuI == GAME_BUTTON_BACK )
	{
		SCREENMAN->SetNewScreen( "ScreenUnifiedDashboard" );
	}
}
