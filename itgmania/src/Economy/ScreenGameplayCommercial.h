#ifndef SCREEN_GAMEPLAY_COMMERCIAL_H
#define SCREEN_GAMEPLAY_COMMERCIAL_H

#include "ScreenGameplayNormal.h"

// A variant of ScreenGameplayNormal that triggers commercial logic (Royalties)
class ScreenGameplayCommercial : public ScreenGameplayNormal
{
public:
	virtual void Init();
};

#endif
