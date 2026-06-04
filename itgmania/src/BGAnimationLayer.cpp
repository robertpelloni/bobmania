#include "BGAnimationLayer.h"
#include "GameState.h"
#include "XmlFile.h"
#include "RageMath.h"
#include "RageLog.h"
#include "Song.h"
#include "ScreenDimensions.h"
#include "Sprite.h"
#include "RageDisplay.h"
#include "ActorUtil.h"
#include "arch/ArchHooks/ArchHooks.h"
#include "LuaManager.h"
#include "AutoActor.h"
#include "ThemeManager.h"
=======
					float fX = m_fTilesStartX + m_fTilesSpacingX * x + fSecs * m_fTileVelocityX;
					float fY = m_fTilesStartY + m_fTilesSpacingY * y + fSecs * m_fTileVelocityY;
>>>>>>> Stashed changes

  // particle and tile stuff
  float fZoomMin = 1;
  float fZoomMax = 1;
  pNode->GetAttrValue("ZoomMin", fZoomMin);
  pNode->GetAttrValue("ZoomMax", fZoomMax);

  float fVelocityXMin = 10, fVelocityXMax = 10;
  float fVelocityYMin = 0, fVelocityYMax = 0;
  float fVelocityZMin = 0, fVelocityZMax = 0;
  float fOverrideSpeed = 0;  // 0 means don't override speed
  pNode->GetAttrValue("VelocityXMin", fVelocityXMin);
  pNode->GetAttrValue("VelocityXMax", fVelocityXMax);
  pNode->GetAttrValue("VelocityYMin", fVelocityYMin);
  pNode->GetAttrValue("VelocityYMax", fVelocityYMax);
  pNode->GetAttrValue("VelocityZMin", fVelocityZMin);
  pNode->GetAttrValue("VelocityZMax", fVelocityZMax);
  pNode->GetAttrValue("OverrideSpeed", fOverrideSpeed);

  int iNumParticles = 10;
  pNode->GetAttrValue("NumParticles", iNumParticles);

  pNode->GetAttrValue("ParticlesBounce", m_bParticlesBounce);
  pNode->GetAttrValue("TilesStartX", m_fTilesStartX);
  pNode->GetAttrValue("TilesStartY", m_fTilesStartY);
  pNode->GetAttrValue("TilesSpacingX", m_fTilesSpacingX);
  pNode->GetAttrValue("TilesSpacingY", m_fTilesSpacingY);
  pNode->GetAttrValue("TileVelocityX", m_fTileVelocityX);
  pNode->GetAttrValue("TileVelocityY", m_fTileVelocityY);

<<<<<<< HEAD
					m_SubActors[i]->SetX( fX );
					m_SubActors[i]->SetY( fY );
				}
			}
		}
		break;
	default:
		FAIL_M(ssprintf("Unrecognized layer type: %i", m_Type));
	}
}

/*
 * (c) 2001-2004 Ben Nordstrom, Chris Danford, Glenn Maynard
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
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
