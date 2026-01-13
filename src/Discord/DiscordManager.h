#ifndef DISCORD_MANAGER_H
#define DISCORD_MANAGER_H

#include "RageTimer.h"

struct DiscordRichPresence;

class DiscordManager
{
public:
	DiscordManager();
	~DiscordManager();

	void Init();
	void Shutdown();
	void Update( float fDeltaTime );

	// Lua bindings
	void SetPresence( const RString &sDetails, const RString &sState, const RString &sLargeImageKey, const RString &sLargeImageText );

private:
	bool m_bInitialized;
	RageTimer m_LastUpdate;
};

extern DiscordManager *DISCORD;

#endif
