#include "global.h"

#include "HighScore.h"

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

#include "GameConstantsAndTypes.h"
#include "PlayerNumber.h"
#include "PrefsManager.h"
#include "RadarValues.h"
#include "ThemeManager.h"
#include "XmlFile.h"

ThemeMetric<RString> EMPTY_NAME("HighScore", "EmptyName");

struct HighScoreImpl {
  RString name;  // Name that shows in the machine's ranking screen.
  Grade grade;
  unsigned int score;
  float percent_dp;
  float survive_seconds;            // Seconds left in Survival mode.
  unsigned int max_combo;           // Maximum combo obtained [SM5 alpha 1a+].
  StageAward stage_award;           // Stage award [SM5 alpha 1a+].
  PeakComboAward peak_combo_award;  // Peak combo award [SM5 alpha 1a+].
  RString modifiers;
  DateTime date_time;    // Return value of time() when screenshot was taken.
  RString player_guid;   // Who made this high score?
  RString machine_guid;  // Where this high score was made.
  int product_id;
  int tap_note_scores[NUM_TapNoteScore];
  int hold_note_scores[NUM_HoldNoteScore];
  RadarValues radar_values;
  float life_remaining_seconds;
  bool disqualified;

  HighScoreImpl();
  XNode* CreateNode() const;
  void LoadFromNode(const XNode* node);

  bool operator==(const HighScoreImpl& other) const;
  bool operator!=(const HighScoreImpl& other) const {
    return !(*this == other);
  }
};

bool HighScoreImpl::operator==(const HighScoreImpl& other) const {
}

void HighScoreList::MergeFromOtherHSL(HighScoreList& other, bool is_machine) {
  num_times_played_ += other.num_times_played_;
  if (other.last_played_ > last_played_) {
    last_played_ = other.last_played_;
  }
  if (other.high_grade_ > high_grade_) {
    high_grade_ = other.high_grade_;
  }
  high_scores_.insert(
      high_scores_.end(), other.high_scores_.begin(), other.high_scores_.end());
  std::sort(high_scores_.begin(), high_scores_.end());
  // NOTE(Kyz): Remove non-unique scores because they probably come from an
  // accidental repeated merge.
  auto unique_end = std::unique(high_scores_.begin(), high_scores_.end());
  high_scores_.erase(unique_end, high_scores_.end());
  // Reverse it because sort moved the lesser scores to the top.
  std::reverse(high_scores_.begin(), high_scores_.end());
  ClampSize(is_machine);
}

XNode* Screenshot::CreateNode() const {
  XNode* node = new XNode("Screenshot");

  // TRICKY:  Don't write "name to fill in" markers.
  node->AppendChild("FileName", file_name);
  node->AppendChild("MD5", md5);
  node->AppendChild(high_score.CreateNode());

  return node;
}

void Screenshot::LoadFromNode(const XNode* node) {
  ASSERT(node->GetName() == "Screenshot");
=======
void HighScoreList::MergeFromOtherHSL(HighScoreList& other, bool is_machine)
{
	iNumTimesPlayed+= other.iNumTimesPlayed;
	if(other.dtLastPlayed > dtLastPlayed) { dtLastPlayed= other.dtLastPlayed; }
	if(other.HighGrade > HighGrade) { HighGrade= other.HighGrade; }
	vHighScores.insert(vHighScores.end(), other.vHighScores.begin(),
		other.vHighScores.end());
	std::sort(vHighScores.begin(), vHighScores.end());
	// Remove non-unique scores because they probably come from an accidental
	// repeated merge. -Kyz
	std::vector<HighScore>::iterator unique_end=
		std::unique(vHighScores.begin(), vHighScores.end());
	vHighScores.erase(unique_end, vHighScores.end());
	// Reverse it because sort moved the lesser scores to the top.
	std::reverse(vHighScores.begin(), vHighScores.end());
	
	if (!PREFSMAN->m_bAllowMultipleHighScoreWithSameName)
	{
		// erase all but the highest score for each name
		RemoveAllButOneOfEachName();
	}
	ClampSize(is_machine);
}
>>>>>>> release
=======
>>>>>>> Stashed changes

  node->GetChildValue("FileName", file_name);
  node->GetChildValue("MD5", md5);
  const XNode* high_score_node = node->GetChild("HighScore");
  if (high_score_node) {
    high_score.LoadFromNode(high_score_node);
  }
}

// lua start
#include "LuaBinding.h"

// Allow Lua to have access to the HighScore.
class LunaHighScore : public Luna<HighScore> {
 public:
  static int GetName(T* p, lua_State* L) {
    lua_pushstring(L, p->GetName());
    return 1;
  }
  static int GetScore(T* p, lua_State* L) {
    lua_pushnumber(L, p->GetScore());
    return 1;
  }
  static int GetPercentDP(T* p, lua_State* L) {
    lua_pushnumber(L, p->GetPercentDP());
    return 1;
  }
  static int GetDate(T* p, lua_State* L) {
    lua_pushstring(L, p->GetDateTime().GetString());
    return 1;
  }
  static int GetSurvivalSeconds(T* p, lua_State* L) {
    lua_pushnumber(L, p->GetSurvivalSeconds());
    return 1;
  }
  static int IsFillInMarker(T* p, lua_State* L) {
    bool is_fill_in_marker = false;
    FOREACH_PlayerNumber(pn)
			is_fill_in_marker |= p->GetName() == RANKING_TO_FILL_IN_MARKER[pn];
    lua_pushboolean(L, is_fill_in_marker);
    return 1;
  }
  static int GetMaxCombo(T* p, lua_State* L) {
    lua_pushnumber(L, p->GetMaxCombo());
    return 1;
  }
  static int GetModifiers(T* p, lua_State* L) {
    lua_pushstring(L, p->GetModifiers());
    return 1;
  }
  static int GetTapNoteScore(T* p, lua_State* L) {
    lua_pushnumber(L, p->GetTapNoteScore(Enum::Check<TapNoteScore>(L, 1)));
    return 1;
  }
  static int GetHoldNoteScore(T* p, lua_State* L) {
    lua_pushnumber(L, p->GetHoldNoteScore(Enum::Check<HoldNoteScore>(L, 1)));
    return 1;
  }
  static int GetRadarValues(T* p, lua_State* L) {
    RadarValues& rv = const_cast<RadarValues&>(p->GetRadarValues());
    rv.PushSelf(L);
    return 1;
  }
  DEFINE_METHOD(GetGrade, GetGrade())
  DEFINE_METHOD(GetStageAward, GetStageAward())
  DEFINE_METHOD(GetPeakComboAward, GetPeakComboAward())
=======
/** @brief Allow Lua to have access to the HighScore. */
class LunaHighScore: public Luna<HighScore>
{
public:
	static int GetName( T* p, lua_State *L )			{ lua_pushstring(L, p->GetName().c_str() ); return 1; }
	static int GetScore( T* p, lua_State *L )			{ lua_pushnumber(L, p->GetScore() ); return 1; }
	static int GetPercentDP( T* p, lua_State *L )			{ lua_pushnumber(L, p->GetPercentDP() ); return 1; }
	static int GetDate( T* p, lua_State *L )			{ lua_pushstring(L, p->GetDateTime().GetString().c_str() ); return 1; }
	static int GetSurvivalSeconds( T* p, lua_State *L )			{ lua_pushnumber(L, p->GetSurvivalSeconds() ); return 1; }
	static int IsFillInMarker( T* p, lua_State *L )
	{
		bool bIsFillInMarker = false;
		FOREACH_PlayerNumber( pn )
			bIsFillInMarker |= p->GetName() == RANKING_TO_FILL_IN_MARKER[pn];
		lua_pushboolean( L, bIsFillInMarker );
		return 1;
	}
	static int GetMaxCombo( T* p, lua_State *L )			{ lua_pushnumber(L, p->GetMaxCombo() ); return 1; }
	static int GetModifiers( T* p, lua_State *L )			{ lua_pushstring(L, p->GetModifiers().c_str() ); return 1; }
	static int GetTapNoteScore( T* p, lua_State *L )			{ lua_pushnumber(L, p->GetTapNoteScore( Enum::Check<TapNoteScore>(L, 1) ) ); return 1; }
	static int GetHoldNoteScore( T* p, lua_State *L )			{ lua_pushnumber(L, p->GetHoldNoteScore( Enum::Check<HoldNoteScore>(L, 1) ) ); return 1; }
	static int GetRadarValues( T* p, lua_State *L )
	{
		RadarValues &rv = const_cast<RadarValues &>(p->GetRadarValues());
		rv.PushSelf(L);
		return 1;
	}
	DEFINE_METHOD( GetGrade, GetGrade() )
	DEFINE_METHOD( GetStageAward, GetStageAward() )
	DEFINE_METHOD( GetPeakComboAward, GetPeakComboAward() )
>>>>>>> release
        return 1;
      }
    }
    lua_pushnil(L);
    return 1;
  }

  static int GetRankOfName(T* p, lua_State* L) {
    RString name = SArg(1);
    std::size_t rank = 0;
    for (std::size_t i = 0; i < p->high_scores_.size(); ++i) {
      if (name == p->high_scores_[i].GetName()) {
        // Indices from Lua are one-indexed.  +1 to adjust.
        rank = i + 1;
        break;
      }
    }
    // The themer is expected to check for validity before using.
    lua_pushnumber(L, rank);
    return 1;
  }
=======
>>>>>>> Stashed changes

  LunaHighScoreList() {
    ADD_METHOD(GetHighScores);
    ADD_METHOD(GetHighestScoreOfName);
    ADD_METHOD(GetRankOfName);
  }
};

LUA_REGISTER_CLASS(HighScoreList)
// lua end

/*
 * (c) 2004 Chris Danford
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
