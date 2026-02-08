#include "global.h"
#include "NotePath.h"
#include "ArrowEffects.h"
#include "GameState.h"
#include "PlayerState.h"
#include "RageDisplay.h"
#include "Style.h"
#include "LuaBinding.h"
#include "ActorUtil.h"

REGISTER_ACTOR_CLASS( NotePath );

NotePath::NotePath() :
	m_fGrayArrowsYStandard("Player", "ReceptorArrowsYStandard"),
	m_fGrayArrowsYReverse("Player", "ReceptorArrowsYReverse")
{
	m_PlayerNumber = PLAYER_1;
	m_iColumn = 0;
	m_fStartBeat = -1.0f;
	m_fEndBeat = 4.0f;
	m_iResolution = 10;
}

NotePath::~NotePath()
{

}

NotePath *NotePath::Copy() const
{
	return new NotePath(*this);
}

void NotePath::Update( float fDeltaTime )
{
	Actor::Update( fDeltaTime );
}

void NotePath::SetPlayerNumber( PlayerNumber pn )
{
	m_PlayerNumber = pn;
}

void NotePath::SetColumn( int iCol )
{
	m_iColumn = iCol;
}

void NotePath::SetDrawRange( float fStartBeat, float fEndBeat )
{
	m_fStartBeat = fStartBeat;
	m_fEndBeat = fEndBeat;
}

void NotePath::SetResolution( int iResolution )
{
	m_iResolution = max(1, iResolution);
}

void NotePath::DrawPrimitives()
{
	Actor::DrawPrimitives();

	if( m_PlayerNumber >= NUM_PLAYERS || !GAMESTATE->IsPlayerEnabled(m_PlayerNumber) )
		return;

	const Style* pStyle = GAMESTATE->GetCurrentStyle(m_PlayerNumber);
	if( !pStyle ) return;

	if( m_iColumn < 0 || m_iColumn >= pStyle->m_iColsPerPlayer ) return;

	PlayerState *pPlayerState = GAMESTATE->m_pPlayerState[m_PlayerNumber];
	if( !pPlayerState ) return;

	// Use temporary options if needed, but here we just use the current player options.
	// NoteField does this too.
	ArrowEffects::SetCurrentOptions( &pPlayerState->m_PlayerOptions.GetCurrent() );

	float fSongBeat = pPlayerState->m_Position.m_fSongBeatVisible;

	float fStart = fSongBeat + m_fStartBeat;
	float fEnd = fSongBeat + m_fEndBeat;

	if( fStart >= fEnd ) return;

	int iNumSteps = (int)((fEnd - fStart) * m_iResolution);
	if( iNumSteps <= 0 ) return;

	// Limit steps to prevent crash/freeze if range is huge
	if( iNumSteps > 4000 ) iNumSteps = 4000;

	vector<RageSpriteVertex> v;
	v.reserve( iNumSteps + 1 );

	// Calculate Y offsets for reverse
	// NoteField calculates this in Init, we do it here or cache it.
	// Since metrics can change (rarely), doing it here is safe.
	float fYReverseOffsetPixels = m_fGrayArrowsYReverse - m_fGrayArrowsYStandard;

	for( int i=0; i<=iNumSteps; ++i )
	{
		float fBeat = fStart + (float)i / m_iResolution;

		float fPeakYOffset;
		bool bIsPastPeak;
		// GetYOffset returns Y pixel offset relative to receptor.
		float fYOffset = ArrowEffects::GetYOffset( pPlayerState, m_iColumn, fBeat, fPeakYOffset, bIsPastPeak, false );

		RageVector3 pos;
		// GetXYZPos converts Y offset to absolute screen coordinates (applying reverse, mods, etc)
		ArrowEffects::GetXYZPos( pPlayerState, m_iColumn, fYOffset, fYReverseOffsetPixels, pos, true );

		RageSpriteVertex vert;
		vert.p = pos;
		vert.c = this->GetDiffuses(0); // Use actor's diffuse color
		vert.t = RageVector2(0,0);
		v.push_back( vert );
	}

	DISPLAY->SetTexture( nullptr );
	DISPLAY->SetBlendMode( BLEND_NORMAL );
	DISPLAY->SetCullMode( CULL_NONE );
	DISPLAY->SetZWrite( false );
	DISPLAY->SetZTestMode( ZTEST_OFF );

	if( !v.empty() )
	{
		// Draw as a line strip.
		// Use width 2.0f as a default or expose it? NotITG usually relies on basic lines or quads.
		// RageDisplay::DrawLineStrip takes width.
		DISPLAY->DrawLineStrip( &v[0], v.size(), 2.0f );
	}
}

// Lua
#include "LuaBinding.h"

class LunaNotePath : public Luna<NotePath>
{
public:
	static int SetPlayer( T* p, lua_State *L )
	{
		p->SetPlayerNumber( Enum::Check<PlayerNumber>(L, 1) );
		return 0;
	}
	static int SetColumn( T* p, lua_State *L )
	{
		p->SetColumn( IArg(1) );
		return 0;
	}
	static int SetDrawRange( T* p, lua_State *L )
	{
		p->SetDrawRange( FArg(1), FArg(2) );
		return 0;
	}
	static int SetResolution( T* p, lua_State *L )
	{
		p->SetResolution( IArg(1) );
		return 0;
	}

	LunaNotePath()
	{
		ADD_METHOD( SetPlayer );
		ADD_METHOD( SetColumn );
		ADD_METHOD( SetDrawRange );
		ADD_METHOD( SetResolution );
	}
};

LUA_REGISTER_DERIVED_CLASS( NotePath, Actor );
