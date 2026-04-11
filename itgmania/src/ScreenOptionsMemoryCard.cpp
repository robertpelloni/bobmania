#include "global.h"
#include "ScreenOptionsMemoryCard.h"
#include "RageLog.h"
#include "arch/MemoryCard/MemoryCardDriver.h"
#include "MemoryCardManager.h"
#include "GameState.h"
#include "ScreenManager.h"
#include "ScreenPrompt.h"
#include "LocalizedString.h"
#include "OptionRowHandler.h"

<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
#include <cstddef>
#include <vector>


=======
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
REGISTER_SCREEN_CLASS( ScreenOptionsMemoryCard );

void ScreenOptionsMemoryCard::Init()
{
	ScreenOptions::Init();

	m_textOsMountDir.SetName( "Mount" );
	m_textOsMountDir.LoadFromFont( THEME->GetPathF(m_sName,"mount") );
	ActorUtil::LoadAllCommands( m_textOsMountDir, m_sName );
	this->AddChild( &m_textOsMountDir );

	this->SubscribeToMessage( Message_StorageDevicesChanged );
}

bool ScreenOptionsMemoryCard::UpdateCurrentUsbStorageDevices()
{
<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
	std::vector<UsbStorageDevice> aOldDevices = m_CurrentUsbStorageDevices;

	const std::vector<UsbStorageDevice> &aNewDevices = MEMCARDMAN->GetStorageDevices();
=======
	vector<UsbStorageDevice> aOldDevices = m_CurrentUsbStorageDevices;

	const vector<UsbStorageDevice> &aNewDevices = MEMCARDMAN->GetStorageDevices();
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp

	m_CurrentUsbStorageDevices.clear();
	for( size_t i = 0; i < aNewDevices.size(); ++i )
	{
		const UsbStorageDevice &dev = aNewDevices[i];
		if( dev.m_State == UsbStorageDevice::STATE_CHECKING )
			continue;
		m_CurrentUsbStorageDevices.push_back( dev );
	}

	return aOldDevices != m_CurrentUsbStorageDevices;
}

static LocalizedString NO_LABEL ("ScreenOptionsMemoryCard", "(no label)" );
static LocalizedString SIZE_UNKNOWN ("ScreenOptionsMemoryCard", "size ???" );
static LocalizedString VOLUME_SIZE ("ScreenOptionsMemoryCard", "%dMB" );
void ScreenOptionsMemoryCard::CreateMenu()
{
<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
	std::vector<OptionRowHandler*> vHands;

	for (UsbStorageDevice const &iter : m_CurrentUsbStorageDevices)
	{
		std::vector<RString> vs;
=======
	vector<OptionRowHandler*> vHands;

	for (UsbStorageDevice const &iter : m_CurrentUsbStorageDevices)
	{
		vector<RString> vs;
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
		if( iter.sVolumeLabel.empty() )
			vs.push_back( NO_LABEL );
		else
			vs.push_back( iter.sVolumeLabel );
		if( iter.iVolumeSizeMB == 0 )
			vs.push_back( SIZE_UNKNOWN );
		else
			vs.push_back( ssprintf(RString(VOLUME_SIZE).c_str(),iter.iVolumeSizeMB) );

		vHands.push_back( OptionRowHandlerUtil::MakeNull() );

		OptionRowDefinition &def = vHands.back()->m_Def;
		RString sDescription = join(", ", vs);
		def.m_sName = sDescription;
		def.m_vsChoices.push_back( "" );
		def.m_sExplanationName = "Memory Card";
		def.m_bAllowThemeTitle = false;
		def.m_bOneChoiceForAllPlayers = true;
	}

	if( m_CurrentUsbStorageDevices.empty() )
	{
		vHands.push_back( OptionRowHandlerUtil::MakeNull() );
		OptionRowDefinition &def = vHands.back()->m_Def;
		def.m_sName = "No USB memory cards found";
		def.m_sExplanationName = "No Memory Card";
		def.m_vsChoices.push_back( "" );
		def.m_bAllowThemeTitle = true;
		def.m_bOneChoiceForAllPlayers = true;
	}
<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp

=======
	
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
	InitMenu( vHands );
}

void ScreenOptionsMemoryCard::BeginScreen()
{
	UpdateCurrentUsbStorageDevices();
	CreateMenu();

	ScreenOptions::BeginScreen();

	// select the last chosen memory card (if present)
	SelectRowWithMemoryCard( MEMCARDMAN->m_sEditorMemoryCardOsMountPoint.Get() );
}

void ScreenOptionsMemoryCard::HandleScreenMessage( const ScreenMessage SM )
{
	ScreenOptions::HandleScreenMessage( SM );
}

void ScreenOptionsMemoryCard::AfterChangeRow( PlayerNumber pn )
{
	ScreenOptions::AfterChangeRow( pn );

	if( m_CurrentUsbStorageDevices.empty() )
	{
		m_textOsMountDir.SetText( "" );
	}
	else
	{
		int iRow = m_iCurrentRow[GAMESTATE->GetMasterPlayerNumber()];
		m_textOsMountDir.SetText( m_CurrentUsbStorageDevices[iRow].sOsMountDir );
	}
}

void ScreenOptionsMemoryCard::HandleMessage( const Message &msg )
{
	if( msg == Message_StorageDevicesChanged )
	{
		if( !m_Out.IsTransitioning() )
		{
			/* Remember the old mountpoint. */
<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
			const std::vector<UsbStorageDevice> &v = m_CurrentUsbStorageDevices;
=======
			const vector<UsbStorageDevice> &v = m_CurrentUsbStorageDevices;
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
			int iRow = m_iCurrentRow[GAMESTATE->GetMasterPlayerNumber()];
			RString sOldMountPoint;
			if( iRow < int(v.size()) )
			{
				const UsbStorageDevice &dev = v[iRow];
				sOldMountPoint = dev.sOsMountDir;
			}

			/* If the devices that we'd actually show haven't changed, don't recreate the menu. */
			if( !UpdateCurrentUsbStorageDevices() )
				return;

			CreateMenu();
			RestartOptions();

			/* If the memory card that was selected previously is still there, select it. */
			SelectRowWithMemoryCard( sOldMountPoint );
		}
	}

	ScreenOptions::HandleMessage( msg );
}

<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
void ScreenOptionsMemoryCard::ImportOptions( int iRow, const std::vector<PlayerNumber> &vpns )
=======
void ScreenOptionsMemoryCard::ImportOptions( int iRow, const vector<PlayerNumber> &vpns )
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
{

}

<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
void ScreenOptionsMemoryCard::ExportOptions( int iRow, const std::vector<PlayerNumber> &vpns )
=======
void ScreenOptionsMemoryCard::ExportOptions( int iRow, const vector<PlayerNumber> &vpns )
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
{
	OptionRow &row = *m_pRows[iRow];
	if( row.GetRowType() == OptionRow::RowType_Exit )
		return;

	PlayerNumber pn = GAMESTATE->GetMasterPlayerNumber();
	if( m_iCurrentRow[pn] == iRow )
	{
<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
		const std::vector<UsbStorageDevice> &v = m_CurrentUsbStorageDevices;
=======
		const vector<UsbStorageDevice> &v = m_CurrentUsbStorageDevices;
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
		if( iRow < int(v.size()) )
		{
			const UsbStorageDevice &dev = v[iRow];
			MEMCARDMAN->m_sEditorMemoryCardOsMountPoint.Set( dev.sOsMountDir );
		}
	}
}

void ScreenOptionsMemoryCard::SelectRowWithMemoryCard( const RString &sOsMountPoint )
{
	if( sOsMountPoint.empty() )
		return;

<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
	const std::vector<UsbStorageDevice> &v = m_CurrentUsbStorageDevices;
=======
	const vector<UsbStorageDevice> &v = m_CurrentUsbStorageDevices;
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
	for( unsigned i=0; i<v.size(); i++ )
	{
		if( v[i].sOsMountDir == sOsMountPoint )
		{
			this->MoveRowAbsolute( PLAYER_1, i );
			return;
		}
	}
}

static LocalizedString ERROR_MOUNTING_CARD ("ScreenOptionsMemoryCard", "Error mounting card: %s" );
void ScreenOptionsMemoryCard::ProcessMenuStart( const InputEventPlus & )
{
	if( IsTransitioning() )
		return;

	int iCurRow = m_iCurrentRow[GAMESTATE->GetMasterPlayerNumber()];

<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
	const std::vector<UsbStorageDevice> &v = m_CurrentUsbStorageDevices;
	if( iCurRow < int(v.size()) )	// a card
	{
		// Why is this statement in twice? Doubt it would change right after the if. -Wolfman2000
		const std::vector<UsbStorageDevice> &vUSB = m_CurrentUsbStorageDevices;
=======
	const vector<UsbStorageDevice> &v = m_CurrentUsbStorageDevices;
	if( iCurRow < int(v.size()) )	// a card
	{
		// Why is this statement in twice? Doubt it would change right after the if. -Wolfman2000
		const vector<UsbStorageDevice> &vUSB = m_CurrentUsbStorageDevices;
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
		const UsbStorageDevice &dev = vUSB[iCurRow];
		MEMCARDMAN->m_sEditorMemoryCardOsMountPoint.Set( dev.sOsMountDir );

		/* The destination screen must UnmountCard.  XXX: brittle */
		bool bSuccess = MEMCARDMAN->MountCard( PLAYER_1, dev );
		if( bSuccess )
		{
			SCREENMAN->PlayStartSound();
			this->BeginFadingOut();
		}
		else
		{
<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
			RString s = ssprintf(ERROR_MOUNTING_CARD.GetValue().c_str(), MEMCARDMAN->GetCardError(PLAYER_1).c_str() );
=======
			RString s = ssprintf(ERROR_MOUNTING_CARD.GetValue(), MEMCARDMAN->GetCardError(PLAYER_1).c_str() );
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
			ScreenPrompt::Prompt( SM_None, s );
		}
	}
	else
	{
		SCREENMAN->PlayInvalidSound();
	}
}

/*
 * (c) 2005 Chris Danford
 * All rights reserved.
<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
 *
=======
 * 
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
<<<<<<< HEAD:itgmania/src/ScreenOptionsMemoryCard.cpp
 *
=======
 * 
>>>>>>> origin/c++11:src/ScreenOptionsMemoryCard.cpp
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
