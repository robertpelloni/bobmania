#ifndef SCREEN_MANAGE_HIDDEN_SONGS_H
#define SCREEN_MANAGE_HIDDEN_SONGS_H

#include "ScreenWithMenuElements.h"
#include "MusicWheel.h"
#include "Sprite.h"
#include "BitmapText.h"

class ScreenManageHiddenSongs : public ScreenWithMenuElements
{
public:
	ScreenManageHiddenSongs();
	virtual void Init();
	virtual void BeginScreen();
	virtual void EndScreen();

	virtual void Update( float fDeltaTime );
	virtual bool Input( const InputEventPlus &input );
	virtual void HandleScreenMessage( const ScreenMessage SM );

private:
	void UpdateSongList();
	void OnSongSelected();

	MusicWheel m_MusicWheel;
	BitmapText m_textInstructions;
};

#endif
