#ifndef NOTE_PATH_H
#define NOTE_PATH_H

#include "Actor.h"
#include "PlayerNumber.h"
#include "ThemeMetric.h"

class NotePath : public Actor
{
public:
	NotePath();
	virtual ~NotePath();

	virtual void DrawPrimitives();
	virtual void Update( float fDeltaTime );

	void SetPlayerNumber( PlayerNumber pn );
	void SetColumn( int iCol );
	void SetDrawRange( float fStartBeat, float fEndBeat ); // relative to current beat
	void SetResolution( int iResolution ); // steps per beat

	virtual NotePath *Copy() const;

	// Lua
	virtual void PushSelf( lua_State *L );

private:
	PlayerNumber m_PlayerNumber;
	int m_iColumn;
	float m_fStartBeat;
	float m_fEndBeat;
	int m_iResolution;

	ThemeMetric<float> m_fGrayArrowsYStandard;
	ThemeMetric<float> m_fGrayArrowsYReverse;
};

#endif
