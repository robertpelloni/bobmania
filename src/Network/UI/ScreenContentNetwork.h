#ifndef SCREEN_CONTENT_NETWORK_H
#define SCREEN_CONTENT_NETWORK_H

#include "ScreenWithMenuElements.h"

class ScreenContentNetwork : public ScreenWithMenuElements
{
public:
    virtual void Init();
    virtual void HandleScreenMessage( const ScreenMessage SM );
    virtual bool Input( const InputEventPlus &input );

private:
    void StartDiscovery();
};

#endif
