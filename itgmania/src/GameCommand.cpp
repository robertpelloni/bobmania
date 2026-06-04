#include "global.h"

#include "GameCommand.h"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

#include "AnnouncerManager.h"
#include "Bookkeeper.h"
#include "Game.h"
#include "GameManager.h"
#include "GameSoundManager.h"
#include "GameState.h"
#include "LocalizedString.h"
#include "PlayerOptions.h"
#include "PlayerState.h"
#include "PrefsManager.h"
#include "Profile.h"
#include "ProfileManager.h"
#include "RageLog.h"
#include "RageUtil.h"
#include "ScreenManager.h"
#include "ScreenPrompt.h"
#include "Song.h"
#include "SongManager.h"
#include "StepMania.h"
#include "Style.h"
#include "UnlockManager.h"
#include "arch/ArchHooks/ArchHooks.h"

static LocalizedString COULD_NOT_LAUNCH_BROWSER(
    "GameCommand", "Could not launch web browser.");

REGISTER_CLASS_TRAITS(GameCommand, new GameCommand(*pCopy));

      }
      return false;
    }
  }


  if (!preferred_modifiers_.empty()) {
    // TODO: Split this and check each modifier individually
    if (UNLOCKMAN->ModifierIsLocked(preferred_modifiers_)) {
      if (invalid_reason) {
        *invalid_reason = "Modifier is locked";
      }
      return false;
    }
  }

  if (!stage_modifiers_.empty()) {
    // TODO: Split this and check each modifier individually
    if (UNLOCKMAN->ModifierIsLocked(stage_modifiers_)) {
      if (invalid_reason) {
        *invalid_reason = "Modifier is locked";
      }
      return false;
    }
  }

  return true;
}

void GameCommand::ApplyToAllPlayers() const {
  std::vector<PlayerNumber> pns;

  FOREACH_PlayerNumber(pn) pns.push_back(pn);

  Apply(pns);
}

void GameCommand::Apply(PlayerNumber pn) const {
  std::vector<PlayerNumber> pns;
  pns.push_back(pn);
  Apply(pns);
}

void GameCommand::Apply(const std::vector<PlayerNumber>& pns) const {
  if (commands_.v.size()) {
    // We were filled using a GameCommand from metrics. Apply the options in
    // order.
    for (const Command& cmd : commands_.v) {
      GameCommand gc;
      gc.is_invalid_ = false;
      gc.apply_commit_screens_ = apply_commit_screens_;
      gc.LoadOne(cmd);
      gc.ApplySelf(pns);
    }
  } else {
    // We were filled by an OptionRowHandler in code. m_Commands isn't filled,
    // so just apply the values that are already set in this.
    this->ApplySelf(pns);
  }
}

void GameCommand::ApplySelf(const std::vector<PlayerNumber>& pns) const {
  const PlayMode old_play_mode = GAMESTATE->play_mode_;

  if (play_mode_ != PlayMode_Invalid) {
    GAMESTATE->play_mode_.Set(play_mode_);
  }

  if (style_ != nullptr) {
    GAMESTATE->SetCurrentStyle(style_, GAMESTATE->GetMasterPlayerNumber());

    // It's possible to choose a style that didn't have enough players joined.
    // If enough players aren't joined, then  we need to subtract credits
    // for the sides that will be joined as a result of applying this option.
    if (GAMESTATE->GetCoinMode() == CoinMode_Pay) {
      int num_credits_required = GetCreditsRequiredToPlayStyle(style_);
      int num_credits_paid = GetNumCreditsPaid();
      int num_credits_owed = num_credits_required - num_credits_paid;
      GAMESTATE->coins_.Set(
          GAMESTATE->coins_ - num_credits_owed * PREFSMAN->m_iCoinsPerCredit);
      LOG->Trace(
          "Deducted %i coins, %i remaining",
          num_credits_owed * PREFSMAN->m_iCoinsPerCredit,
          GAMESTATE->coins_.Get());

      // Credit Used, make sure to update CoinsFile
      BOOKKEEPER->WriteCoinsFile(GAMESTATE->coins_.Get());
    }

    // If only one side is joined and we picked a style that requires both
    // sides, join the other side.
    switch (style_->m_StyleType) {
      case StyleType_OnePlayerOneSide:
      case StyleType_OnePlayerTwoSides:
        break;
      case StyleType_TwoPlayersTwoSides:
      case StyleType_TwoPlayersSharedSides: {
        FOREACH_PlayerNumber(p) GAMESTATE->JoinPlayer(p);
      } break;
      default:
        LuaHelpers::ReportScriptErrorFmt(
            "Invalid StyleType: %d", style_->m_StyleType);
    }
  }
  if (difficulty_ != Difficulty_Invalid) {
    for (const PlayerNumber& pn : pns) {
      GAMESTATE->preferred_difficulty_[pn].Set(difficulty_);
    }
  }
  if (announcer_ != "") {
    ANNOUNCER->SwitchAnnouncer(announcer_);
  }
  if (preferred_modifiers_ != "") {
    for (const PlayerNumber& pn : pns) {
      GAMESTATE->ApplyPreferredModifiers(pn, preferred_modifiers_);
    }
  }
  if (stage_modifiers_ != "") {
    for (const PlayerNumber& pn : pns) {
      GAMESTATE->ApplyStageModifiers(pn, stage_modifiers_);
    }
  }
  if (lua_function_.IsSet() && !lua_function_.IsNil()) {
    Lua* L = LUA->Get();
    for (const PlayerNumber& pn : pns) {
      lua_function_.PushSelf(L);
      ASSERT(!lua_isnil(L, -1));

      lua_pushnumber(L, pn);  // 1st parameter
      RString error = "Lua GameCommand error: ";
      LuaHelpers::RunScriptOnStack(L, error, 1, 0, true);
    }
    LUA->Release(L);
  }
  if (screen_ != "" && apply_commit_screens_) {
    SCREENMAN->SetNewScreen(screen_);
  }
  if (song_) {
    GAMESTATE->cur_song_.Set(song_);
    GAMESTATE->preferred_song_ = song_;
  }
  if (steps_) {
    for (const PlayerNumber& pn : pns) {
      GAMESTATE->cur_steps_[pn].Set(steps_);
    }
  }
  if (course_) {
    GAMESTATE->cur_course_.Set(course_);
    GAMESTATE->preferred_course_ = course_;
  }
  if (trail_) {
    for (const PlayerNumber& pn : pns) {
      GAMESTATE->cur_trail_[pn].Set(trail_);
    }
  }
  if (course_difficulty_ != Difficulty_Invalid) {
    for (const PlayerNumber& pn : pns) {
      GAMESTATE->ChangePreferredCourseDifficulty(pn, course_difficulty_);
    }
  }
  if (character_) {
    for (const PlayerNumber& pn : pns) {
      GAMESTATE->cur_characters_[pn] = character_;
    }
  }
  for (auto it = set_env_.begin(); it != set_env_.end(); ++it) {
    Lua* L = LUA->Get();
    GAMESTATE->environment_->PushSelf(L);
    lua_pushstring(L, it->first);
    lua_pushstring(L, it->second);
    lua_settable(L, -3);
    lua_pop(L, 1);
    LUA->Release(L);
  }
  for (auto it = set_pref_.begin(); it != set_pref_.end(); ++it) {
    IPreference* preference = IPreference::GetPreferenceByName(it->first);
    if (preference != nullptr) {
      preference->FromString(it->second);
    }
  }
  if (!song_group_.empty()) {
    GAMESTATE->preferred_song_group_.Set(song_group_);
  }
  if (sort_order_ != SortOrder_Invalid) {
    GAMESTATE->preferred_sort_order_ = sort_order_;
  }
  if (sound_path_ != "") {
    SOUND->PlayOnce(THEME->GetPathS("", sound_path_));
  }
  if (weight_pounds_ != -1) {
    for (const PlayerNumber& pn : pns) {
      PROFILEMAN->GetProfile(pn)->m_iWeightPounds = weight_pounds_;
    }
  }
  if (goal_calories_ != -1) {
    for (const PlayerNumber& pn : pns) {
      PROFILEMAN->GetProfile(pn)->m_iGoalCalories = goal_calories_;
    }
  }
  if (goal_type_ != GoalType_Invalid) {
    for (const PlayerNumber& pn : pns) {
      PROFILEMAN->GetProfile(pn)->m_GoalType = goal_type_;
    }
  }
  if (!profile_id_.empty()) {
    for (const PlayerNumber& pn : pns) {
      ProfileManager::m_sDefaultLocalProfileID[pn].Set(profile_id_);
    }
  }
  if (!url_.empty()) {
    if (HOOKS->GoToURL(url_)) {
      if (url_exists_) {
        SCREENMAN->SetNewScreen("ScreenExit");
      }
    } else {
      ScreenPrompt::Prompt(SM_None, COULD_NOT_LAUNCH_BROWSER);
    }
  }

  // If we're going to stop music, do so before preparing new screens, so we
  // don't stop music between preparing screens and loading screens.
  if (stop_music_) {
    SOUND->StopMusic();
  }
  if (fade_music_) {
    SOUND->DimMusic(music_fade_out_volume_, music_fade_out_seconds_);
  }

  for (const RString& s : screens_to_prepare_) {
    SCREENMAN->PrepareScreen(s);
  }

  if (insert_credits_) {
    StepMania::InsertCredit();
  }

  if (clear_credits_) {
    StepMania::ClearCredits();
  }

  if (apply_default_options_) {
    // Applying options affects only the current stage.
    FOREACH_PlayerNumber(p) {
      PlayerOptions player_options;
      GAMESTATE->GetDefaultPlayerOptions(player_options);
      GAMESTATE->player_state_[p]->m_PlayerOptions.Assign(
					ModsLevel_Stage, player_options);
    }

    SongOptions song_options;
    GAMESTATE->GetDefaultSongOptions(song_options);
    GAMESTATE->song_options_.Assign(ModsLevel_Stage, song_options);
  }
  // HACK: Set life type to BATTERY just once here so it happens once and
  // we don't override the user's changes if they back out.
  FOREACH_PlayerNumber(pn) {
    if (GAMESTATE->play_mode_ == PLAY_MODE_ONI &&
        GAMESTATE->play_mode_ != old_play_mode &&
        GAMESTATE->player_state_[pn]->m_PlayerOptions.GetStage().m_LifeType ==
            LifeType_Bar) {
      PO_GROUP_ASSIGN(
          GAMESTATE->player_state_[pn]->m_PlayerOptions, ModsLevel_Stage,
          m_LifeType, LifeType_Battery);
    }
  }
}


  DEFINE_METHOD(GetCourseDifficulty, course_difficulty_)
  DEFINE_METHOD(GetDifficulty, difficulty_)
  DEFINE_METHOD(GetPlayMode, play_mode_)
  DEFINE_METHOD(GetSortOrder, sort_order_)
=======
class LunaGameCommand : public Luna<GameCommand> {
 public:
  static int GetName(T* p, lua_State* L) {
    lua_pushstring(L, p->m_sName.c_str());
    return 1;
  }
  static int GetText(T* p, lua_State* L) {
    lua_pushstring(L, p->m_sText.c_str());
    return 1;
  }
  static int GetIndex(T* p, lua_State* L) {
    lua_pushnumber(L, p->m_iIndex);
    return 1;
  }
  static int GetMultiPlayer(T* p, lua_State* L) {
    lua_pushnumber(L, p->m_MultiPlayer);
    return 1;
  }
  static int GetStyle(T* p, lua_State* L) {
    if (p->m_pStyle == nullptr) {
      lua_pushnil(L);
    } else {
      Style* pStyle = (Style*)p->m_pStyle;
      pStyle->PushSelf(L);
    }
    return 1;
  }
  static int GetScreen(T* p, lua_State* L) {
    lua_pushstring(L, p->m_sScreen.c_str());
    return 1;
  }
  static int GetProfileID(T* p, lua_State* L) {
    lua_pushstring(L, p->m_sProfileID.c_str());
    return 1;
  }
  static int GetSong(T* p, lua_State* L) {
    if (p->m_pSong == nullptr) {
      lua_pushnil(L);
    } else {
      p->m_pSong->PushSelf(L);
    }
    return 1;
  }
  static int GetSteps(T* p, lua_State* L) {
    if (p->m_pSteps == nullptr) {
      lua_pushnil(L);
    } else {
      p->m_pSteps->PushSelf(L);
    }
    return 1;
  }
  static int GetCourse(T* p, lua_State* L) {
    if (p->m_pCourse == nullptr) {
      lua_pushnil(L);
    } else {
      p->m_pCourse->PushSelf(L);
    }
    return 1;
  }
  static int GetTrail(T* p, lua_State* L) {
    if (p->m_pTrail == nullptr) {
      lua_pushnil(L);
    } else {
      p->m_pTrail->PushSelf(L);
    }
    return 1;
  }
  static int GetCharacter(T* p, lua_State* L) {
    if (p->m_pCharacter == nullptr) {
      lua_pushnil(L);
    } else {
      p->m_pCharacter->PushSelf(L);
    }
    return 1;
  }
  static int GetSongGroup(T* p, lua_State* L) {
    lua_pushstring(L, p->m_sSongGroup.c_str());
    return 1;
  }
  static int GetUrl(T* p, lua_State* L) {
    LOG->Warn("GetUrl usage has been deprecated.");
    return 1;
  }
  static int GetAnnouncer(T* p, lua_State* L) {
    lua_pushstring(L, p->m_sAnnouncer.c_str());
    return 1;
  }
  static int GetPreferredModifiers(T* p, lua_State* L) {
    lua_pushstring(L, p->m_sPreferredModifiers.c_str());
    return 1;
  }
  static int GetStageModifiers(T* p, lua_State* L) {
    lua_pushstring(L, p->m_sStageModifiers.c_str());
    return 1;
  }

  DEFINE_METHOD(GetCourseDifficulty, m_CourseDifficulty)
  DEFINE_METHOD(GetDifficulty, m_dc)
  DEFINE_METHOD(GetPlayMode, m_pm)
  DEFINE_METHOD(GetSortOrder, m_SortOrder)
>>>>>>> upstream/release

  LunaGameCommand() {
    ADD_METHOD(GetName);
    ADD_METHOD(GetText);
    ADD_METHOD(GetIndex);
    ADD_METHOD(GetMultiPlayer);
    ADD_METHOD(GetStyle);
    ADD_METHOD(GetDifficulty);
    ADD_METHOD(GetCourseDifficulty);
    ADD_METHOD(GetScreen);
    ADD_METHOD(GetPlayMode);
    ADD_METHOD(GetProfileID);
    ADD_METHOD(GetSong);
    ADD_METHOD(GetSteps);
    ADD_METHOD(GetCourse);
    ADD_METHOD(GetTrail);
    ADD_METHOD(GetCharacter);
    ADD_METHOD(GetSongGroup);
    ADD_METHOD(GetSortOrder);
    ADD_METHOD(GetUrl);
    ADD_METHOD(GetAnnouncer);
    ADD_METHOD(GetPreferredModifiers);
    ADD_METHOD(GetStageModifiers);
  }
};

LUA_REGISTER_CLASS(GameCommand)
// lua end

/*
 * (c) 2001-2004 Chris Danford, Glenn Maynard
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
