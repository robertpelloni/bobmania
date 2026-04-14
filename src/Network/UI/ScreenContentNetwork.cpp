#include "global.h"
#include "ScreenContentNetwork.h"
#include "Network/ContentSwarmManager.h"
#include "ScreenManager.h"
#include "RageLog.h"

REGISTER_SCREEN_CLASS( ScreenContentNetwork );

void ScreenContentNetwork::Init()
{
    ScreenWithMenuElements::Init();

    // Start discovery when entering screen
    if( SWARMMAN ) SWARMMAN->StartDiscovery();
}

void ScreenContentNetwork::HandleScreenMessage( const ScreenMessage SM )
{
    ScreenWithMenuElements::HandleScreenMessage( SM );
}

bool ScreenContentNetwork::Input( const InputEventPlus &input )
{
    return ScreenWithMenuElements::Input( input );
}
