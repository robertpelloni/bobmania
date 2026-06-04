#include "global.h"

#include "CombinedLifeMeterTug.h"

#include <algorithm>
#include <cstddef>
#include <string>

#include "ActorUtil.h"
#include "GameState.h"
#include "PrefsManager.h"
#include "ThemeManager.h"
#include "ThemeMetric.h"

ThemeMetric<float> METER_WIDTH("CombinedLifeMeterTug", "MeterWidth");

static void TugMeterPercentChangeInit(
    size_t score_event, RString& name_out, float& default_value_out) {
  name_out =
      "TugMeterPercentChange" + ScoreEventToString((ScoreEvent)score_event);
  switch (score_event) {
    default:
      FAIL_M(ssprintf("Invalid ScoreEvent: %i", static_cast<int>(score_event)));
    case SE_W1:
      default_value_out = +0.010f;
      break;
    case SE_W2:
      default_value_out = +0.008f;
      break;
    case SE_W3:
      default_value_out = +0.004f;
      break;
    case SE_W4:
      default_value_out = +0.000f;
      break;
    case SE_W5:
      default_value_out = -0.010f;
      break;
    case SE_Miss:
      default_value_out = -0.020f;
      break;
    case SE_HitMine:
      default_value_out = -0.040f;
      break;
    case SE_CheckpointHit:
      default_value_out = +0.002f;
      break;
    case SE_CheckpointMiss:
      default_value_out = -0.002f;
      break;
    case SE_Held:
      default_value_out = +0.008f;
      break;
    case SE_LetGo:
      default_value_out = -0.020f;
      break;
    case SE_Missed:
      default_value_out = +0.000f;
      break;
  }
=======
#include "ActorFrame.h"
#include "ActorUtil.h"
#include "GameConstantsAndTypes.h"
#include "GameState.h"
#include "PlayerNumber.h"
#include "Preference.h"
#include "PrefsManager.h"
#include "RageUtil.h"
#include "ThemeManager.h"
#include "ThemeMetric.h"
#include "global.h"

ThemeMetric<float> METER_WIDTH("CombinedLifeMeterTug", "MeterWidth");

static void TugMeterPercentChangeInit(
    size_t /*ScoreEvent*/ i, std::string& sNameOut, float& defaultValueOut) {
  sNameOut = "TugMeterPercentChange" + ScoreEventToString((ScoreEvent)i);
  switch (i) {
    default:
      FAIL_M(ssprintf("Invalid ScoreEvent: %i", static_cast<int>(i)));
    case SE_W1:
      defaultValueOut = +0.010f;
      break;
    case SE_W2:
      defaultValueOut = +0.008f;
      break;
    case SE_W3:
      defaultValueOut = +0.004f;
      break;
    case SE_W4:
      defaultValueOut = +0.000f;
      break;
    case SE_W5:
      defaultValueOut = -0.010f;
      break;
    case SE_Miss:
      defaultValueOut = -0.020f;
      break;
    case SE_HitMine:
      defaultValueOut = -0.040f;
      break;
    case SE_CheckpointHit:
      defaultValueOut = +0.002f;
      break;
    case SE_CheckpointMiss:
      defaultValueOut = -0.002f;
      break;
    case SE_Held:
      defaultValueOut = +0.008f;
      break;
    case SE_LetGo:
      defaultValueOut = -0.020f;
      break;
    case SE_Missed:
      defaultValueOut = +0.000f;
      break;
  }
>>>>>>> upstream/release
      break;
    default:
      FAIL_M(ssprintf("Invalid player number: %i", pn));
  }
=======
>>>>>>> Stashed changes
}

/*
 * (c) 2003-2004 Chris Danford
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
