#include "global.h"

#include "GameState.h"

#include <cmath>
#include <cstddef>
#include <ctime>
#include <set>
#include <vector>

#include "Actor.h"
#include "ActorUtil.h"
#include "AdjustSync.h"
#include "Bookkeeper.h"
#include "Character.h"
#include "CharacterManager.h"
#include "Command.h"
#include "CommonMetrics.h"
#include "Course.h"
#include "CryptManager.h"
#include "Difficulty.h"
#include "EnumHelper.h"
#include "Game.h"
#include "GameCommand.h"
#include "GameConstantsAndTypes.h"
#include "GameManager.h"
#include "GamePreferences.h"
#include "Grade.h"
#include "HighScore.h"
#include "InputMapper.h"
#include "LightsManager.h"
#include "LocalizedString.h"
#include "LuaManager.h"
#include "LuaReference.h"
#include "MemoryCardManager.h"
#include "MessageManager.h"
#include "NoteData.h"
#include "NoteSkinManager.h"
#include "PlayerNumber.h"
#include "PlayerStageStats.h"
#include "PlayerState.h"
#include "Preference.h"
#include "PrefsManager.h"
#include "Profile.h"
#include "ProfileManager.h"
#include "RageException.h"
#include "RageFile.h"
#include "RageFileManager.h"
#include "RageLog.h"
#include "RageThreads.h"
#include "RageUtil.h"
#include "Screen.h"
#include "ScreenManager.h"
#include "Song.h"
#include "SongManager.h"
#include "SongOptions.h"
#include "SongUtil.h"
#include "StatsManager.h"
#include "StdString.h"
#include "StepMania.h"
#include "Steps.h"
#include "StepsUtil.h"
#include "Style.h"
#include "ThemeManager.h"
#include "ThemeMetric.h"
#include "Trail.h"
#include "UnlockManager.h"

// Global and accessible from anywhere in our program.
GameState* GAMESTATE = nullptr;

#define NAME_BLACKLIST_FILE "/Data/NamesBlacklist.txt"

class GameStateMessageHandler : public MessageSubscriber {
  void HandleMessage(const Message& msg) {
    if (msg.GetName() == "RefreshCreditText") {
      RString joined;
      FOREACH_HumanPlayer(pn) {
        if (joined != "") {
          joined += ", ";
        }
        joined += ssprintf("P%i", pn + 1);
      }

      if (joined == "") {
        joined = "none";
      }

      LOG->MapLog("JOINED", "Players joined: %s", joined.c_str());
    }
  }
};

struct GameStateImpl {
  GameStateMessageHandler subscriber;
  GameStateImpl() { subscriber.SubscribeToMessage("RefreshCreditText"); }
};
static GameStateImpl* g_pImpl = nullptr;

ThemeMetric<bool> ALLOW_LATE_JOIN("GameState", "AllowLateJoin");
ThemeMetric<bool> USE_NAME_BLACKLIST("GameState", "UseNameBlacklist");

ThemeMetric<RString> DEFAULT_SORT("GameState", "DefaultSort");
SortOrder GetDefaultSort() { return StringToSortOrder(DEFAULT_SORT); }
ThemeMetric<RString> DEFAULT_SONG("GameState", "DefaultSong");
Song* GameState::GetDefaultSong() const {
  SongID song_id;
  song_id.FromString(DEFAULT_SONG);
  return song_id.ToSong();
}

static const ThemeMetric<bool> EDIT_ALLOWED_FOR_EXTRA(
    "GameState", "EditAllowedForExtra");
static const ThemeMetric<Difficulty> MIN_DIFFICULTY_FOR_EXTRA(
    "GameState", "MinDifficultyForExtra");
static const ThemeMetric<Grade> GRADE_TIER_FOR_EXTRA_1(
    "GameState", "GradeTierForExtra1");
static const ThemeMetric<bool> ALLOW_EXTRA_2(
		"GameState", "AllowExtra2");
static const ThemeMetric<Grade> GRADE_TIER_FOR_EXTRA_2(
    "GameState", "GradeTierForExtra2");

static ThemeMetric<bool> ARE_STAGE_PLAYER_MODS_FORCED(
    "GameState", "AreStagePlayerModsForced");
static ThemeMetric<bool> ARE_STAGE_SONG_MODS_FORCED(
    "GameState", "AreStageSongModsForced");

static Preference<Premium> g_Premium("Premium", Premium_DoubleFor1Credit);
Preference<bool> GameState::auto_join_("AutoJoin", false);

GameState::GameState()
    : processed_timing_(nullptr),
      cur_game_(Message_CurrentGameChanged),
      cur_style_(Message_CurrentStyleChanged),
      play_mode_(Message_PlayModeChanged),
      coins_(Message_CoinsChanged),
      preferred_song_group_(Message_PreferredSongGroupChanged),
      preferred_course_group_(Message_PreferredCourseGroupChanged),
      preferred_steps_type_(Message_PreferredStepsTypeChanged),
      preferred_difficulty_(Message_PreferredDifficultyP1Changed),
      preferred_course_difficulty_(Message_PreferredCourseDifficultyP1Changed),
      sort_order_(Message_SortOrderChanged),
      cur_song_(Message_CurrentSongChanged),
      cur_steps_(Message_CurrentStepsP1Changed),
      cur_course_(Message_CurrentCourseChanged),
      cur_trail_(Message_CurrentTrailP1Changed),
      gameplay_lead_in_(Message_GameplayLeadInChanged),
      did_mode_change_noteskin_(false),
      is_using_step_timing_(true),
      in_step_editor_(false),
      steps_type_edit_(Message_EditStepsTypeChanged),
      course_difficulty_edit_(Message_EditCourseDifficultyChanged),
      edit_source_steps_(Message_EditSourceStepsChanged),
      edit_source_steps_type_(Message_EditSourceStepsTypeChanged),
      edit_course_entry_index_(Message_EditCourseEntryIndexChanged),
      edit_local_profile_id_(Message_EditLocalProfileIDChanged) {
  g_pImpl = new GameStateImpl;

  cur_style_.Set(nullptr);
  FOREACH_PlayerNumber(rpn) { separated_styles_[rpn] = nullptr; }

  cur_game_.Set(nullptr);
  coins_.Set(0);
  time_game_started_.SetZero();
  demonstration_or_jukebox_ = false;

	// Initial screen will bump this up to 0
  num_times_through_attract_ = -1;
  stage_seed_ = game_seed_ = 0;

	// Used by IsPlayerEnabled before the first screen.
  play_mode_.Set( PlayMode_Invalid);
	// Used by GetNumSidesJoined before the first screen.
  FOREACH_PlayerNumber(p) side_is_joined_[p] = false;

  FOREACH_PlayerNumber(p) {
    player_state_[p] = new PlayerState;
    player_state_[p]->SetPlayerNumber(p);
  }
  FOREACH_MultiPlayer(p) {
    multiplayer_state_[p] = new PlayerState;
    multiplayer_state_[p]->SetPlayerNumber(PLAYER_1);
    multiplayer_state_[p]->m_mp = p;
  }

  environment_ = new LuaTable;

  dopefish_ = false;

  expanded_section_name_ = "";
=======
Preference<bool> GameState::m_bAutoJoin("AutoJoin", false);

GameState::GameState()
    : processedTiming(nullptr),
      m_pCurGame(Message_CurrentGameChanged),
      m_pCurStyle(Message_CurrentStyleChanged),
      m_PlayMode(Message_PlayModeChanged),
      m_iCoins(Message_CoinsChanged),
      m_sPreferredSongGroup(Message_PreferredSongGroupChanged),
      m_sPreferredCourseGroup(Message_PreferredCourseGroupChanged),
      m_PreferredStepsType(Message_PreferredStepsTypeChanged),
      m_PreferredDifficulty(Message_PreferredDifficultyP1Changed),
      m_PreferredCourseDifficulty(Message_PreferredCourseDifficultyP1Changed),
      m_SortOrder(Message_SortOrderChanged),
      m_pCurSong(Message_CurrentSongChanged),
      m_pCurSteps(Message_CurrentStepsP1Changed),
      m_pCurCourse(Message_CurrentCourseChanged),
      m_pCurTrail(Message_CurrentTrailP1Changed),
      m_bGameplayLeadIn(Message_GameplayLeadInChanged),
      m_bDidModeChangeNoteSkin(false),
      m_bIsUsingStepTiming(true),
      m_bInStepEditor(false),
      m_stEdit(Message_EditStepsTypeChanged),
      m_cdEdit(Message_EditCourseDifficultyChanged),
      m_pEditSourceSteps(Message_EditSourceStepsChanged),
      m_stEditSource(Message_EditSourceStepsTypeChanged),
      m_iEditCourseEntryIndex(Message_EditCourseEntryIndexChanged),
      m_sEditLocalProfileID(Message_EditLocalProfileIDChanged) {
  g_pImpl = new GameStateImpl;

  m_pCurStyle.Set(nullptr);
  FOREACH_PlayerNumber(rpn) { m_SeparatedStyles[rpn] = nullptr; }

  m_pCurGame.Set(nullptr);
  m_iCoins.Set(0);
  m_timeGameStarted.SetZero();
  m_bDemonstrationOrJukebox = false;

  m_iNumTimesThroughAttract = -1;  // initial screen will bump this up to 0
  m_iStageSeed = m_iGameSeed = 0;

  m_PlayMode.Set(
      PlayMode_Invalid);  // used by IsPlayerEnabled before the first screen
  FOREACH_PlayerNumber(p) m_bSideIsJoined[p] =
      false;  // used by GetNumSidesJoined before the first screen

  FOREACH_PlayerNumber(p) {
    m_pPlayerState[p] = new PlayerState;
    m_pPlayerState[p]->SetPlayerNumber(p);
  }
  FOREACH_MultiPlayer(p) {
    m_pMultiPlayerState[p] = new PlayerState;
    m_pMultiPlayerState[p]->SetPlayerNumber(PLAYER_1);
    m_pMultiPlayerState[p]->m_mp = p;
  }

  m_Environment = new LuaTable;

  m_bDopefish = false;
  sLastOpenSection = "";
  sExpandedSectionName = "";
>>>>>>> upstream/release
    return;
  }

  if (pn == PLAYER_INVALID) {
    m.ApplyToAllPlayers();
  } else {
    m.Apply(pn);
  }
}

void GameState::ApplyCmdline() {
  // We need to join players before we can set the style.
  RString player;
  for (int i = 0; GetCommandlineArgument("player", &player, i); ++i) {
    int pn = StringToInt(player) - 1;
    if (!IsAnInt(player) || pn < 0 || pn >= NUM_PLAYERS) {
      RageException::Throw(
          "Invalid argument \"--player=%s\".", player.c_str());
    }

    JoinPlayer((PlayerNumber)pn);
  }

  RString mode;
  for (int i = 0; GetCommandlineArgument("mode", &mode, i); ++i) {
    ApplyGameCommand(mode);
  }
}

void GameState::ResetPlayer(PlayerNumber pn) {
  preferred_steps_type_.Set(StepsType_Invalid);
  preferred_difficulty_[pn].Set(Difficulty_Invalid);
  preferred_course_difficulty_[pn].Set(Difficulty_Medium);
  player_stage_tokens_[pn] = 0;
  awarded_extra_stages_[pn] = 0;
  cur_steps_[pn].Set(nullptr);
  cur_trail_[pn].Set(nullptr);
  player_state_[pn]->Reset();
  PROFILEMAN->UnloadProfile(pn);
  ResetPlayerOptions(pn);
}

void GameState::ResetPlayerOptions(PlayerNumber pn) {
  PlayerOptions player_options;
  GetDefaultPlayerOptions(player_options);
  player_state_[pn]->m_PlayerOptions.Assign(
			ModsLevel_Preferred, player_options);
}

void GameState::Reset() {
	// Must initialize for UnjoinPlayer.
  this->SetMasterPlayerNumber(PLAYER_INVALID);

  FOREACH_PlayerNumber(pn) UnjoinPlayer(pn);

  ASSERT(THEME != nullptr);

  time_game_started_.SetZero();
  SetCurrentStyle(nullptr, PLAYER_INVALID);
  FOREACH_MultiPlayer(p) multiplayer_status_[p] = MultiPlayerStatus_NotJoined;
  FOREACH_PlayerNumber(pn) MEMCARDMAN->UnlockCard(pn);
  multiplayer_ = false;
  num_multiplayer_note_fields_ = 1;
  *environment_ = LuaTable();
  preferred_song_group_.Set(GROUP_ALL);
  preferred_course_group_.Set(GROUP_ALL);
  fail_type_was_explicitly_set_ = false;
  sort_order_.Set(SortOrder_Invalid);
  preferred_sort_order_ = GetDefaultSort();
  play_mode_.Set(PlayMode_Invalid);
  edit_mode_ = EditMode_Invalid;
  demonstration_or_jukebox_ = false;
  jukebox_uses_modifiers_ = false;
  current_stage_index_ = 0;

  gameplay_lead_in_.Set(false);
  num_stages_of_this_song_ = 0;
  loading_next_song_ = false;

  NOTESKIN->RefreshNoteSkinData(cur_game_);

  game_seed_ = rand();
  stage_seed_ = rand();

  adjust_tokens_by_song_cost_for_final_stage_check = true;

  cur_song_.Set(GetDefaultSong());
  preferred_song_ = nullptr;
  cur_course_.Set(nullptr);
  preferred_course_ = nullptr;

  FOREACH_MultiPlayer(p) multiplayer_state_[p]->Reset();

  song_options_.Init();

  is_paused_ = false;
  ResetMusicStatistics();
  ResetStageStatistics();
  AdjustSync::ResetOriginalSyncData();

  SONGMAN->UpdatePopular();
  SONGMAN->UpdateShuffled();

  // We may have cached trails from before everything was loaded (eg. from
  // before SongManager::UpdatePopular could be called). Erase the cache.
  SONGMAN->RegenerateNonFixedCourses();

  STATSMAN->Reset();

  FOREACH_PlayerNumber(p) {
    if (PREFSMAN->m_ShowDancingCharacters == SDC_Random) {
      cur_characters_[p] = CHARMAN->GetRandomCharacter();
    } else {
      cur_characters_[p] = CHARMAN->GetDefaultCharacter();
    }
    ASSERT(cur_characters_[p] != nullptr);
  }

  temporary_event_mode_ = false;

  LIGHTSMAN->SetLightsMode(LIGHTSMODE_ATTRACT);

  steps_type_edit_.Set(StepsType_Invalid);
  edit_source_steps_.Set(nullptr);
  edit_source_steps_type_.Set(StepsType_Invalid);
  edit_course_entry_index_.Set(-1);
  edit_local_profile_id_.Set("");

  backed_out_of_final_stage_ = false;
  earned_extra_stage_ = false;
  expanded_section_name_ = "";

  ApplyCmdline();
}

void GameState::JoinPlayer(PlayerNumber pn) {
  // Make sure the join will be successful before doing it. -Kyz
  {
    int players_joined = 0;
    for (int i = 0; i < NUM_PLAYERS; ++i) {
      players_joined += side_is_joined_[i];
    }
    if (players_joined > 0) {
      const Style* cur_style = GetCurrentStyle(PLAYER_INVALID);
      if (cur_style) {
        const Style* new_style = GAMEMAN->GetFirstCompatibleStyle(
            cur_game_, players_joined + 1, cur_style->m_StepsType);
        if (new_style == nullptr) {
          return;
        }
      }
    }
  }
  // If joint premium and we're not taking away a credit for the 2nd join, give
	// the new player the same number of stage tokens that the old player has.
  if (GetCoinMode() == CoinMode_Pay &&
      GetPremium() == Premium_2PlayersFor1Credit && GetNumSidesJoined() == 1) {
    player_stage_tokens_[pn] =
        player_stage_tokens_[this->GetMasterPlayerNumber()];
  } else {
    player_stage_tokens_[pn] = PREFSMAN->m_iSongsPerPlay;
  }

  side_is_joined_[pn] = true;

  if (this->GetMasterPlayerNumber() == PLAYER_INVALID) {
    this->SetMasterPlayerNumber(pn);
  }

  // If first player to join, set start time.
  if (GetNumSidesJoined() == 1) {
    BeginGame();
  }

  // Count each player join as a play.
  {
    Profile* machine_profile = PROFILEMAN->GetMachineProfile();
    machine_profile->m_iTotalSessions++;
  }

  // NOTE(kyz): Set the current style to something appropriate for the new
  // number of joined players. beat gametype's versus styles use a different
  // stepstype from its single styles, so when GameCommand tries to join both
  // players for a versus style, it hits the assert when joining the first
  // player. So if the firstplayer is being joined and the current styletype is
  // for two players, assume that the second player will be joined immediately
  // afterwards and don't try to change the style.
  const Style* cur_style = GetCurrentStyle(PLAYER_INVALID);
  if (cur_style != nullptr &&
      !(pn == PLAYER_1 &&
        (cur_style->m_StyleType == StyleType_TwoPlayersTwoSides ||
         cur_style->m_StyleType == StyleType_TwoPlayersSharedSides))) {
    const Style* pStyle;
    // NOTE(aj): Only use one player for StyleType_OnePlayerTwoSides and
    // StepsTypes that can only be played by one player (e.g. dance-solo,
    // dance-threepanel, popn-nine).
    // NOTE(aj): This still shows joined player as "Insert Card". May not be an
		// issue?
    if (cur_style->m_StyleType == StyleType_OnePlayerTwoSides ||
        cur_style->m_StepsType == StepsType_dance_solo ||
        cur_style->m_StepsType == StepsType_dance_threepanel ||
        cur_style->m_StepsType == StepsType_popn_nine) {
      pStyle = GAMEMAN->GetFirstCompatibleStyle(
          cur_game_, 1, cur_style->m_StepsType);
    } else {
      pStyle = GAMEMAN->GetFirstCompatibleStyle(
          cur_game_, GetNumSidesJoined(), cur_style->m_StepsType);
    }

    // Use SetCurrentStyle in case of StyleType_OnePlayerTwoSides.
    SetCurrentStyle(pStyle, pn);
  }

  Message msg(MessageIDToString(Message_PlayerJoined));
  msg.SetParam("Player", pn);
  MESSAGEMAN->Broadcast(msg);
}

void GameState::UnjoinPlayer(PlayerNumber pn) {
  // Unjoin STATSMAN first, so steps used by this player are released
  // and can be released by PROFILEMAN.
  STATSMAN->UnjoinPlayer(pn);
  side_is_joined_[pn] = false;
  player_stage_tokens_[pn] = 0;

  ResetPlayer(pn);

  if (this->GetMasterPlayerNumber() == pn) {
    // We can't use GetFirstHumanPlayer() because if both players were joined,
    // GetFirstHumanPlayer() will always return PLAYER_1, even when PLAYER_1 is
    // the player we're unjoining.
    FOREACH_HumanPlayer(hp) {
      if (pn != hp) {
        this->SetMasterPlayerNumber(hp);
      }
    }
    if (this->GetMasterPlayerNumber() == pn) {
      this->SetMasterPlayerNumber(PLAYER_INVALID);
    }
  }

  Message msg(MessageIDToString(Message_PlayerUnjoined));
  msg.SetParam("Player", pn);
  MESSAGEMAN->Broadcast(msg);

  // If there are no players left, reset some non-player-specific stuff, too.
  if (this->GetMasterPlayerNumber() == PLAYER_INVALID) {
    SongOptions song_options;
    GetDefaultSongOptions(song_options);
    song_options_.Assign(ModsLevel_Preferred, song_options);
    did_mode_change_noteskin_ = false;
  }
}

// TODO(aj): handle multiplayer join?

namespace {

bool JoinInputInternal(PlayerNumber pn) {
  if (!GAMESTATE->PlayersCanJoin()) {
    return false;
  }

  // If this side is already in, don't re-join.
  if (GAMESTATE->side_is_joined_[pn]) {
    return false;
  }

  // Subtract coins.
  int iCoinsNeededToJoin = GAMESTATE->GetCoinsNeededToJoin();

  if (GAMESTATE->coins_ < iCoinsNeededToJoin) {
		// Not enough coins.
    return false;
  }

  GAMESTATE->coins_.Set(GAMESTATE->coins_ - iCoinsNeededToJoin);

  GAMESTATE->JoinPlayer(pn);

  // On Join, make sure to update Coins File
  BOOKKEEPER->WriteCoinsFile(GAMESTATE->coins_.Get());

  return true;
}

}  // namespace

// Handle an input that can join a player. Return true if the player joined.
bool GameState::JoinInput(PlayerNumber pn) {
  // When AutoJoin is enabled, join all players on a single start press.
  if (GAMESTATE->auto_join_.Get()) {
    return JoinPlayers();
  } else {
    return JoinInputInternal(pn);
  }
}

// Attempt to join all players, as if each player pressed Start.
bool GameState::JoinPlayers() {
  bool joined = false;
  FOREACH_PlayerNumber(pn) {
    if (JoinInputInternal(pn)) {
      joined = true;
    }
  }
  return joined;
}

int GameState::GetCoinsNeededToJoin() const {
  int coins_to_charge = 0;

  if (GetCoinMode() == CoinMode_Pay) {
    coins_to_charge = PREFSMAN->m_iCoinsPerCredit;
  }

  // If joint premium, don't take away a credit for the second join.
  if (GetPremium() == Premium_2PlayersFor1Credit && GetNumSidesJoined() == 1) {
    coins_to_charge = 0;
  }

  return coins_to_charge;
}

// Game flow:
//
// BeginGame() - the first player has joined; the game is starting.
//
// PlayersFinalized() - player memory cards are loaded; later joins won't have
// memory cards this stage
//
// BeginStage() - gameplay is beginning
//
// optional: CancelStage() - gameplay aborted (Back pressed), undo BeginStage
// and back up
//
// CommitStageStats() - gameplay is finished
//   Saves STATSMAN->m_CurStageStats to the profiles, so profile information
//   is up-to-date for Evaluation.
//
// FinishStage() - gameplay and evaluation is finished
//   Clears data which was stored by CommitStageStats.
void GameState::BeginGame() {
  time_game_started_.Touch();

  names_that_were_filled_.clear();
  players_that_were_filled_.clear();

  // Play attract on the ending screen, then on the ranking screen
  // even if attract sounds are set to off.
  num_times_through_attract_ = -1;

  FOREACH_PlayerNumber(pn) MEMCARDMAN->UnlockCard(pn);
}

void GameState::LoadProfiles(bool load_edits) {
  // Unlock any cards that we might want to load.
  FOREACH_HumanPlayer(pn) if (!PROFILEMAN->IsPersistentProfile(pn))
      MEMCARDMAN->UnlockCard(pn);

  MEMCARDMAN->WaitForCheckingToComplete();

  FOREACH_HumanPlayer(pn) {
    // If a profile is already loaded, this was already called.
    if (PROFILEMAN->IsPersistentProfile(pn)) {
      continue;
    }

    MEMCARDMAN->MountCard(pn);
		// Load full profile.
    bool success = PROFILEMAN->LoadFirstAvailableProfile(pn, load_edits);
    MEMCARDMAN->UnmountCard(pn);

    if (!success) {
      continue;
    }

    // Lock the card on successful load, so we won't allow it to be changed.
    MEMCARDMAN->LockCard(pn);

    LoadCurrentSettingsFromProfile(pn);

    Profile* player_profile = PROFILEMAN->GetProfile(pn);
    if (player_profile) {
      player_profile->m_iTotalSessions++;
    }
  }
}

void GameState::SavePlayerProfiles() {
  FOREACH_HumanPlayer(pn) SavePlayerProfile(pn);
}

void GameState::SavePlayerProfile(PlayerNumber pn) {
  if (!PROFILEMAN->IsPersistentProfile(pn)) {
    return;
  }

  // NOTE(aj): AutoplayCPU should not save scores. this MAY cause issues with
	// Multiplayer. However, without a working Multiplayer build, we'll never
	// know.
  if (player_state_[pn]->m_PlayerController != PC_HUMAN) {
    return;
  }

  bool was_memory_card = PROFILEMAN->ProfileWasLoadedFromMemoryCard(pn);
  if (was_memory_card) {
    MEMCARDMAN->MountCard(pn);
  }
  PROFILEMAN->SaveProfile(pn);
  if (was_memory_card) {
    MEMCARDMAN->UnmountCard(pn);
  }
}

bool GameState::HaveProfileToLoad() {
  FOREACH_HumanPlayer(pn) {
    // We won't load this profile if it's already loaded.
    if (PROFILEMAN->IsPersistentProfile(pn)) {
      continue;
    }

    // If a memory card is inserted, we'l try to load it.
    if (MEMCARDMAN->CardInserted(pn)) {
      return true;
    }
    if (!PROFILEMAN->m_sDefaultLocalProfileID[pn].Get().empty()) {
      return true;
    }
  }

  return false;
}

bool GameState::HaveProfileToSave() {
  FOREACH_HumanPlayer(pn) if (PROFILEMAN->IsPersistentProfile(pn)) return true;
  return false;
}

void GameState::SaveLocalData() {
  BOOKKEEPER->WriteToDisk();
  PROFILEMAN->SaveMachineProfile();
}

int GameState::GetNumStagesMultiplierForSong(const Song* song) {
  int num_stages = 1;

  ASSERT(song != nullptr);
  if (song->IsMarathon()) {
    num_stages *= 3;
  }
  if (song->IsLong()) {
    num_stages *= 2;
  }

  return num_stages;
}

int GameState::GetNumStagesForCurrentSongAndStepsOrCourse() const {
  int num_stages_of_this_song = 1;
  if (cur_song_) {
    // Extra stages need to only count as one stage in case a multi-stage song
		// is chosen.
    if (IsAnExtraStage()) {
      num_stages_of_this_song = 1;
    } else {
      num_stages_of_this_song =
          GameState::GetNumStagesMultiplierForSong(cur_song_);
    }
  } else if (cur_course_) {
    num_stages_of_this_song = PREFSMAN->m_iSongsPerPlay;
  } else {
    return -1;
  }

  num_stages_of_this_song = std::max(num_stages_of_this_song, 1);

  return num_stages_of_this_song;
}

// Called by ScreenGameplay. Set the length of the current song.
void GameState::BeginStage() {
  if (demonstration_or_jukebox_) {
    return;
  }

  // This should only be called once per stage.
  if (num_stages_of_this_song_ != 0) {
    LOG->Warn("XXX: num_stages_of_this_song_ == %i?", num_stages_of_this_song_);
  }

  ResetStageStatistics();
  AdjustSync::ResetOriginalSyncData();

  if (!ARE_STAGE_PLAYER_MODS_FORCED) {
    FOREACH_PlayerNumber(pn) {
      ModsGroup<PlayerOptions>& player_options =
					player_state_[pn]->m_PlayerOptions;
      player_options.Assign(
          ModsLevel_Stage, player_state_[pn]->m_PlayerOptions.GetPreferred());
    }
  }
  if (!ARE_STAGE_SONG_MODS_FORCED) {
    song_options_.Assign(ModsLevel_Stage, song_options_.GetPreferred());
  }

  STATSMAN->m_CurStageStats.m_fMusicRate = song_options_.GetSong().m_fMusicRate;
  num_stages_of_this_song_ = GetNumStagesForCurrentSongAndStepsOrCourse();
  ASSERT(num_stages_of_this_song_ != -1);
  FOREACH_EnabledPlayer(p) {
    // NOTE(aj): Only do this check with human players. Assume that CPU players
    // (Rave) always have tokens (this could probably be moved below, even).
    if (!IsEventMode() && !IsCpuPlayer(p)) {
      if (player_stage_tokens_[p] < num_stages_of_this_song_) {
        LuaHelpers::ReportScriptErrorFmt(
            "Player %d only has %d stage tokens, but needs %d.", p,
            player_stage_tokens_[p], num_stages_of_this_song_);
      }
    }
    player_stage_tokens_[p] -= num_stages_of_this_song_;
  }
  FOREACH_HumanPlayer(pn) if (CurrentOptionsDisqualifyPlayer(pn))
      STATSMAN->m_CurStageStats.m_player[pn]
          .m_bDisqualified = true;
  earned_extra_stage_ = false;
  stage_guid_ = CryptManager::GenerateRandomUUID();
}

void GameState::CancelStage() {
  FOREACH_CpuPlayer(p) {
    switch (play_mode_) {
      case PLAY_MODE_BATTLE:
      case PLAY_MODE_RAVE:
        player_stage_tokens_[p] = PREFSMAN->m_iSongsPerPlay;
      default:
        break;
    }
  }

  FOREACH_EnabledPlayer(p) player_stage_tokens_[p] += num_stages_of_this_song_;
  num_stages_of_this_song_ = 0;
  ResetStageStatistics();
}

void GameState::CommitStageStats() {
  if (demonstration_or_jukebox_) {
    return;
  }

  STATSMAN->CommitStatsToProfiles(&STATSMAN->m_CurStageStats);

  // Update TotalPlaySeconds.
  int play_seconds = std::max(0, (int)time_game_started_.GetDeltaTime());

  Profile* machine_profile = PROFILEMAN->GetMachineProfile();
  machine_profile->m_iTotalSessionSeconds += play_seconds;

  FOREACH_HumanPlayer(p) {
    Profile* player_profile = PROFILEMAN->GetProfile(p);
    if (player_profile) {
      player_profile->m_iTotalSessionSeconds += play_seconds;
    }
  }
}

// Called by ScreenSelectMusic (etc). Increment the stage counter if we just
// played a song. Might be called more than once.
void GameState::FinishStage() {
  // Increment the stage counter.
  const int old_stage_index = current_stage_index_;
  ++current_stage_index_;

  num_stages_of_this_song_ = 0;

  EarnedExtraStage earned_extra_stage = CalculateEarnedExtraStage();
  STATSMAN->m_CurStageStats.m_EarnedExtraStage = earned_extra_stage;
  if (earned_extra_stage != EarnedExtraStage_No) {
    LOG->Trace("awarded extra stage");
    FOREACH_HumanPlayer(p) {
      // TODO(aj): Unhardcode the extra stage limit?
      if (awarded_extra_stages_[p] < 2) {
        ++awarded_extra_stages_[p];
        ++player_stage_tokens_[p];
        earned_extra_stage_ = true;
      }
    }
  }

  // Save the current combo to the profiles so it can be used for
  // ComboContinuesBetweenSongs.
  FOREACH_HumanPlayer(p) {
    Profile* profile = PROFILEMAN->GetProfile(p);
    profile->m_iCurrentCombo =
        STATSMAN->m_CurStageStats.m_player[p].m_iCurCombo;
    // If the sort order is Recent, move the profile to the top of the list.
    if (PREFSMAN->m_ProfileSortOrder == ProfileSortOrder_Recent &&
        PROFILEMAN->IsPersistentProfile(p)) {
      int num_local_profiles = PROFILEMAN->GetNumLocalProfiles();
      for (int i = 0; i < num_local_profiles; ++i) {
        Profile* local_profile = PROFILEMAN->GetLocalProfileFromIndex(i);
        if (local_profile->m_sGuid == profile->m_sGuid) {
          PROFILEMAN->MoveProfileTopBottom(
              i, PREFSMAN->m_bProfileSortOrderAscending);
          break;
        }
      }
    }
  }

  if (demonstration_or_jukebox_) {
    return;
  }

  // TODO(aj): Simplify. Profile saving is accomplished in ScreenProfileSave
  // now; all this code does differently is save machine profile as well.
  if (IsEventMode()) {
    const int save_profile_every = 3;
    if (old_stage_index / save_profile_every <
        current_stage_index_ / save_profile_every) {
      LOG->Trace("Played %i stages; saving profiles ...", save_profile_every);
      PROFILEMAN->SaveMachineProfile();
      this->SavePlayerProfiles();
    }
  }
}

void GameState::LoadCurrentSettingsFromProfile(PlayerNumber pn) {
  if (!PROFILEMAN->IsPersistentProfile(pn)) {
    return;
  }

  const Profile* profile = PROFILEMAN->GetProfile(pn);

  // apply saved default modifiers if any
  RString modifiers;
  if (profile->GetDefaultModifiers(cur_game_, modifiers)) {
    // We don't save negative preferences (eg. "no reverse"). If the theme
    // sets a default of "reverse", and the player turns it off, we should
    // set it off. However, don't reset modifiers that aren't saved by the
    // profile, so we don't ignore unsaved modifiers when a profile is in use.
    PO_GROUP_CALL(
        player_state_[pn]->m_PlayerOptions, ModsLevel_Preferred,
        ResetSavedPrefs);
    ApplyPreferredModifiers(pn, modifiers);
  }
  // Only set the sort order if it wasn't already set by a GameCommand (or by an
  // earlier profile)
  if (preferred_sort_order_ == SortOrder_Invalid &&
      profile->m_SortOrder != SortOrder_Invalid) {
    preferred_sort_order_ = profile->m_SortOrder;
  }
  if (profile->m_LastDifficulty != Difficulty_Invalid) {
    preferred_difficulty_[pn].Set(profile->m_LastDifficulty);
  }
  if (profile->m_LastCourseDifficulty != Difficulty_Invalid) {
    preferred_course_difficulty_[pn].Set(profile->m_LastCourseDifficulty);
  }
  // Only set the PreferredStepsType if it wasn't already set by a GameCommand
  // (or by an earlier profile)
  if (preferred_steps_type_ == StepsType_Invalid &&
      profile->m_LastStepsType != StepsType_Invalid) {
    preferred_steps_type_.Set(profile->m_LastStepsType);
  }
  if (preferred_song_ == nullptr) {
    preferred_song_ = profile->m_lastSong.ToSong();
  }
  if (preferred_course_ == nullptr) {
    preferred_course_ = profile->m_lastCourse.ToCourse();
  }
}

void GameState::SaveCurrentSettingsToProfile(PlayerNumber pn) {
  if (!PROFILEMAN->IsPersistentProfile(pn)) {
    return;
  }
  if (demonstration_or_jukebox_) {
    return;
  }

  Profile* pProfile = PROFILEMAN->GetProfile(pn);

  pProfile->SetDefaultModifiers(
      cur_game_,
      player_state_[pn]->m_PlayerOptions.GetPreferred().GetSavedPrefsString());
  if (IsSongSort(preferred_sort_order_)) {
    pProfile->m_SortOrder = preferred_sort_order_;
  }
  if (preferred_difficulty_[pn] != Difficulty_Invalid) {
    pProfile->m_LastDifficulty = preferred_difficulty_[pn];
  }
  if (preferred_course_difficulty_[pn] != Difficulty_Invalid) {
    pProfile->m_LastCourseDifficulty = preferred_course_difficulty_[pn];
  }
  if (preferred_steps_type_ != StepsType_Invalid) {
    pProfile->m_LastStepsType = preferred_steps_type_;
  }
  if (preferred_song_) {
    pProfile->m_lastSong.FromSong(preferred_song_);
  }
  if (preferred_course_) {
    pProfile->m_lastCourse.FromCourse(preferred_course_);
  }
}

bool GameState::CanSafelyEnterGameplay(RString& reason) {
  if (!IsCourseMode()) {
    const Song* song = cur_song_;
    if (song == nullptr) {
      reason = "Current song is null.";
      return false;
    }
  } else {
    const Course* song = cur_course_;
    if (song == nullptr) {
      reason = "Current course is null.";
      return false;
    }
  }
  FOREACH_EnabledPlayer(pn) {
    const Style* style = GetCurrentStyle(pn);
    if (style == nullptr) {
      reason = ssprintf("Style for player %d is null.", pn + 1);
      return false;
    }
    if (!IsCourseMode()) {
      const Steps* steps = cur_steps_[pn];
      if (steps == nullptr) {
        reason = ssprintf("Steps for player %d is null.", pn + 1);
        return false;
      }
      if (steps->m_StepsType != style->m_StepsType) {
        reason = ssprintf(
            "Player %d StepsType %s for steps does not equal "
            "StepsType %s for style.",
            pn + 1, GAMEMAN->GetStepsTypeInfo(steps->m_StepsType).name,
            GAMEMAN->GetStepsTypeInfo(style->m_StepsType).name);
        return false;
      }
      if (steps->m_pSong != cur_song_) {
        reason = ssprintf(
            "Steps for player %d are not for the current song.", pn + 1);
        return false;
      }
      NoteData ndtemp;
      steps->GetNoteData(ndtemp);
      if (ndtemp.GetNumTracks() != style->m_iColsPerPlayer) {
        reason = ssprintf(
            "Steps for player %d have %d columns, style has %d "
            "columns.",
            pn + 1, ndtemp.GetNumTracks(), style->m_iColsPerPlayer);
        return false;
      }
    } else {
      const Trail* steps = cur_trail_[pn];
      if (steps == nullptr) {
        reason = ssprintf("Steps for player %d is null.", pn + 1);
        return false;
      }
      if (steps->m_StepsType != style->m_StepsType) {
        reason = ssprintf(
            "Player %d StepsType %s for steps does not equal "
            "StepsType %s for style.",
            pn + 1, GAMEMAN->GetStepsTypeInfo(steps->m_StepsType).name,
            GAMEMAN->GetStepsTypeInfo(style->m_StepsType).name);
        return false;
      }
    }
  }
  return true;
}

void GameState::SetCompatibleStylesForPlayers() {
  bool style_set = false;
  if (IsCourseMode()) {
    if (cur_course_ != nullptr) {
      const Style* style =
          cur_course_->GetCourseStyle(cur_game_, GetNumSidesJoined());
      if (style != nullptr) {
        style_set = true;
        SetCurrentStyle(style, PLAYER_INVALID);
      }
    } else if (GetCurrentStyle(PLAYER_INVALID) == nullptr) {
      std::vector<StepsType> steps_types;
      GAMEMAN->GetStepsTypesForGame(cur_game_, steps_types);
      const Style* style = GAMEMAN->GetFirstCompatibleStyle(
          cur_game_, GetNumSidesJoined(), steps_types[0]);
      SetCurrentStyle(style, PLAYER_INVALID);
    }
  }
  if (!style_set) {
    FOREACH_EnabledPlayer(pn) {
      StepsType steps_type = StepsType_Invalid;
      if (cur_steps_[pn] != nullptr) {
        steps_type = cur_steps_[pn]->m_StepsType;
      } else if (cur_trail_[pn] != nullptr) {
        steps_type = cur_trail_[pn]->m_StepsType;
      } else {
        std::vector<StepsType> steps_types;
        GAMEMAN->GetStepsTypesForGame(cur_game_, steps_types);
        steps_type = steps_types[0];
      }
      const Style* style = GAMEMAN->GetFirstCompatibleStyle(
					cur_game_, GetNumSidesJoined(), steps_type);
      SetCurrentStyle(style, pn);
    }
  }
}

void GameState::ForceSharedSidesMatch() {
  PlayerNumber pn_with_shared = PLAYER_INVALID;
  const Style* shared_style = nullptr;
  FOREACH_EnabledPlayer(pn) {
    const Style* style = GetCurrentStyle(pn);
    ASSERT_M(style != nullptr, "Style being null should not be possible.");
    if (style->m_StyleType == StyleType_TwoPlayersSharedSides) {
      pn_with_shared = pn;
      shared_style = style;
    }
  }
  if (pn_with_shared != PLAYER_INVALID) {
    ASSERT_M(
        GetNumPlayersEnabled() == 2,
        "2 players must be enabled for shared sides.");
    PlayerNumber other_pn = OPPOSITE_PLAYER[pn_with_shared];
    const Style* other_style = GetCurrentStyle(other_pn);
    ASSERT_M(
        other_style != nullptr,
        "Other player's style being null should not be possible.");
    if (other_style->m_StyleType != StyleType_TwoPlayersSharedSides) {
      SetCurrentStyle(shared_style, other_pn);
      if (IsCourseMode()) {
        cur_trail_[other_pn].Set(cur_trail_[pn_with_shared]);
      } else {
        cur_steps_[other_pn].Set(cur_steps_[pn_with_shared]);
      }
    }
  }
}

void GameState::ForceOtherPlayersToCompatibleSteps(PlayerNumber main) {
  if (IsCourseMode()) {
    Trail* steps_to_match = cur_trail_[main].Get();
    if (steps_to_match == nullptr) {
      return;
    }
    int num_players = GAMESTATE->GetNumPlayersEnabled();
    StyleType styletype_to_match =
        GAMEMAN
            ->GetFirstCompatibleStyle(
                GAMESTATE->GetCurrentGame(), num_players,
                steps_to_match->m_StepsType)
            ->m_StyleType;
    FOREACH_EnabledPlayer(pn) {
      Trail* pn_steps = cur_trail_[pn].Get();
      bool match_failed = pn_steps == nullptr;
      if (steps_to_match != pn_steps && pn_steps != nullptr) {
        StyleType pn_styletype = GAMEMAN
                                     ->GetFirstCompatibleStyle(
                                         GAMESTATE->GetCurrentGame(),
                                         num_players, pn_steps->m_StepsType)
                                     ->m_StyleType;
        if (styletype_to_match == StyleType_TwoPlayersSharedSides ||
            pn_styletype == StyleType_TwoPlayersSharedSides) {
          match_failed = true;
        }
      }
      if (match_failed) {
        cur_trail_[pn].Set(steps_to_match);
      }
    }
  } else {
    Steps* steps_to_match = cur_steps_[main].Get();
    if (steps_to_match == nullptr) {
      return;
    }
    int num_players = GAMESTATE->GetNumPlayersEnabled();
    StyleType styletype_to_match =
        GAMEMAN
            ->GetFirstCompatibleStyle(
                GAMESTATE->GetCurrentGame(), num_players,
                steps_to_match->m_StepsType)
            ->m_StyleType;
    RString music_to_match = steps_to_match->GetMusicFile();
    FOREACH_EnabledPlayer(pn) {
      Steps* pn_steps = cur_steps_[pn].Get();
      bool match_failed = pn_steps == nullptr;
      if (steps_to_match != pn_steps && pn_steps != nullptr) {
        StyleType pn_styletype = GAMEMAN
                                     ->GetFirstCompatibleStyle(
                                         GAMESTATE->GetCurrentGame(),
                                         num_players, pn_steps->m_StepsType)
                                     ->m_StyleType;
        if (styletype_to_match == StyleType_TwoPlayersSharedSides ||
            pn_styletype == StyleType_TwoPlayersSharedSides) {
          match_failed = true;
        }
        if (music_to_match != pn_steps->GetMusicFile()) {
          match_failed = true;
        }
      }
      if (match_failed) {
        cur_steps_[pn].Set(steps_to_match);
      }
    }
  }
}

void GameState::Update(float delta) {
  song_options_.Update(delta);

  FOREACH_PlayerNumber(p) {
    player_state_[p]->Update(delta);

    if (!goal_complete_[p] && IsGoalComplete(p)) {
      goal_complete_[p] = true;
      MESSAGEMAN->Broadcast(
          (MessageID)(Message_GoalCompleteP1 + Enum::to_integral(p)));
    }
  }

  if (GAMESTATE->cur_course_) {
    if (GAMESTATE->cur_course_->m_fGoalSeconds > 0 &&
        !workout_goal_complete_) {
      const StageStats& cur_stage_stats = STATSMAN->m_CurStageStats;
      bool goal_complete = cur_stage_stats.m_fGameplaySeconds >
                           GAMESTATE->cur_course_->m_fGoalSeconds;
      if (goal_complete) {
        MESSAGEMAN->Broadcast("WorkoutGoalComplete");
        workout_goal_complete_ = true;
      }
    }
  }
}

void GameState::SetCurGame(const Game* game) {
  cur_game_.Set(game);
  RString game_str = game ? RString(game->name) : RString();
  PREFSMAN->SetCurrentGame(game_str);
}

const float GameState::MUSIC_SECONDS_INVALID = -5000.0f;

void GameState::ResetMusicStatistics() {
  position_.Reset();
  last_position_timer_.Touch();
  last_position_seconds_ = 0.0f;

  Actor::SetBGMTime(0, 0, 0, 0);

  FOREACH_PlayerNumber(p) { player_state_[p]->m_Position.Reset(); }
}

void GameState::ResetStageStatistics() {
  StageStats old_stats = STATSMAN->m_CurStageStats;
  STATSMAN->m_CurStageStats = StageStats();
  if (PREFSMAN->m_bComboContinuesBetweenSongs) {
    FOREACH_PlayerNumber(p) {
      bool first_song = current_stage_index_ == 0;
      if (first_song) {
        Profile* profile = PROFILEMAN->GetProfile(p);
        STATSMAN->m_CurStageStats.m_player[p].m_iCurCombo =
            profile->m_iCurrentCombo;
      } else {
        STATSMAN->m_CurStageStats.m_player[p].m_iCurCombo =
            old_stats.m_player[p].m_iCurCombo;
      }
    }
  }

  RemoveAllActiveAttacks();
  FOREACH_PlayerNumber(p) player_state_[p]->RemoveAllInventory();
  opponent_health_percent_ = 1;
  haste_rate_ = 0;
  last_haste_update_music_seconds_ = 0;
  accumulated_haste_seconds_ = 0;
  tug_life_percent_p1_ = 0.5f;
  FOREACH_PlayerNumber(pn) {
    player_state_[pn]->m_fSuperMeter = 0;
    player_state_[pn]->m_HealthState = HealthState_Alive;

    player_state_[pn]->m_iLastPositiveSumOfAttackLevels = 0;
		// PlayerAI not affected.
    player_state_[pn]->m_fSecondsUntilAttacksPhasedOut = 0;

    goal_complete_[pn] = false;
  }
  workout_goal_complete_ = false;

  FOREACH_PlayerNumber(pn) {
    last_stage_awards_[pn].clear();
    last_peak_combo_awards_[pn].clear();
  }

  // Reset the round seed. Do this here and not in FinishStage so that players
  // get new shuffle patterns if they Back out of gameplay and play again.
  stage_seed_ = rand();
}

void GameState::UpdateSongPosition(
    float position_seconds, const TimingData& timing,
    const RageTimer& timestamp) {
  // It's not uncommon to get a lot of duplicated positions from the sound
  // driver, like so: 13.120953,13.130975,13.130975,13.130975,13.140998,...
  // This causes visual stuttering of the arrows. To compensate, keep a
  // RageTimer since the last change.
  if (position_seconds == last_position_seconds_ && !is_paused_) {
    position_seconds += last_position_timer_.Ago();
  } else {
    last_position_timer_.Touch();
    last_position_seconds_ = position_seconds;
  }

  position_.UpdateSongPosition(position_seconds, timing, timestamp);

  FOREACH_EnabledPlayer(pn) {
    if (cur_steps_[pn]) {
      float additional_visual_delay =
          player_state_[pn]->m_PlayerOptions.GetPreferred().m_fVisualDelay;
      player_state_[pn]->m_Position.UpdateSongPosition(
          position_seconds, *cur_steps_[pn]->GetTimingData(), timestamp,
          additional_visual_delay);
      Actor::SetPlayerBGMBeat(
          pn, player_state_[pn]->m_Position.m_fSongBeatVisible,
          player_state_[pn]->m_Position.m_fSongBeatNoOffset);
    }
  }
  Actor::SetBGMTime(
      GAMESTATE->position_.m_fMusicSecondsVisible,
      GAMESTATE->position_.m_fSongBeatVisible, position_seconds,
      GAMESTATE->position_.m_fSongBeatNoOffset);
}

float GameState::GetSongPercent(float beat) const {
  // 0 = first step; 1 = last step
  float curTime = this->cur_song_->m_SongTiming.GetElapsedTimeFromBeat(beat);
  return (curTime - cur_song_->GetFirstSecond()) / cur_song_->GetLastSecond();
}

int GameState::GetNumStagesLeft(PlayerNumber pn) const {
  return player_stage_tokens_[pn];
}

int GameState::GetSmallestNumStagesLeftForAnyHumanPlayer() const {
  if (IsEventMode()) {
    return 999;
  }
  int smallest = INT_MAX;
  FOREACH_HumanPlayer(p) smallest = std::min(smallest, player_stage_tokens_[p]);
  return smallest;
}

bool GameState::IsFinalStageForAnyHumanPlayer() const {
  return GetSmallestNumStagesLeftForAnyHumanPlayer() == 1;
}

bool GameState::IsFinalStageForEveryHumanPlayer() const {
  int song_cost = 1;
  if (cur_song_ != nullptr) {
    if (cur_song_->IsLong()) {
      song_cost = 2;
    } else if (cur_song_->IsMarathon()) {
      song_cost = 3;
    }
  }
  // If we're on gameplay or evaluation, they set this to false because those
  // screens have already had the stage tokens subtracted.
  song_cost *= adjust_tokens_by_song_cost_for_final_stage_check;
  int num_on_final = 0;
  int num_humans = 0;
  FOREACH_HumanPlayer(p) {
    if (player_stage_tokens_[p] - song_cost <= 0) {
      ++num_on_final;
    }
    ++num_humans;
  }
  return num_on_final >= num_humans;
}

bool GameState::IsAnExtraStage() const {
  if (this->GetMasterPlayerNumber() == PlayerNumber_Invalid) {
    return false;
  }
  return !IsEventMode() && !IsCourseMode() &&
         awarded_extra_stages_[this->GetMasterPlayerNumber()] > 0;
}

static ThemeMetric<bool> LOCK_EXTRA_STAGE_SELECTION(
    "GameState", "LockExtraStageSelection");
bool GameState::IsAnExtraStageAndSelectionLocked() const {
  return IsAnExtraStage() && LOCK_EXTRA_STAGE_SELECTION;
}

bool GameState::IsExtraStage() const {
  if (this->GetMasterPlayerNumber() == PlayerNumber_Invalid) {
    return false;
  }
  return !IsEventMode() && !IsCourseMode() &&
         awarded_extra_stages_[this->GetMasterPlayerNumber()] == 1;
}

bool GameState::IsExtraStage2() const {
  if (this->GetMasterPlayerNumber() == PlayerNumber_Invalid) {
    return false;
  }
  return !IsEventMode() && !IsCourseMode() &&
         awarded_extra_stages_[this->GetMasterPlayerNumber()] == 2;
}

Stage GameState::GetCurrentStage() const {
  if (demonstration_or_jukebox_) {
    return Stage_Demo;
  }
  // "event" has precedence
  else if (IsEventMode()) {
    return Stage_Event;
  } else if (play_mode_ == PLAY_MODE_ONI) {
    return Stage_Oni;
  } else if (play_mode_ == PLAY_MODE_NONSTOP) {
    return Stage_Nonstop;
  } else if (play_mode_ == PLAY_MODE_ENDLESS) {
    return Stage_Endless;
  } else if (IsExtraStage()) {
    return Stage_Extra1;
  } else if (IsExtraStage2()) {
    return Stage_Extra2;
  }
  // NOTE(kyz): Previous logic did not factor in current song length, or the
  // fact that players aren't allowed to start a song with 0 tokens. This new
  // function also has logic for handling the Gameplay and Evaluation cases
  // which used to require workarounds on the theme side.
  else if (IsFinalStageForEveryHumanPlayer()) {
    return Stage_Final;
  } else {
    switch (this->current_stage_index_) {
      case 0:
        return Stage_1st;
      case 1:
        return Stage_2nd;
      case 2:
        return Stage_3rd;
      case 3:
        return Stage_4th;
      case 4:
        return Stage_5th;
      case 5:
        return Stage_6th;
      default:
        return Stage_Next;
    }
  }
}

int GameState::GetCourseSongIndex() const {
  // iSongsPlayed includes the current song, so it's 1-based; subtract one.
  if (GAMESTATE->multiplayer_) {
    FOREACH_EnabledMultiPlayer(mp) return STATSMAN->m_CurStageStats
            .m_multiPlayer[mp]
            .m_iSongsPlayed -
        1;
    FAIL_M("At least one MultiPlayer must be joined.");
  } else {
    return STATSMAN->m_CurStageStats.m_player[this->GetMasterPlayerNumber()]
               .m_iSongsPlayed -
           1;
  }
}

// HACK: when we're loading a new course song, we want to display the new song
// number, even though we haven't started that song yet.
int GameState::GetLoadingCourseSongIndex() const {
  int iIndex = GetCourseSongIndex();
  if (loading_next_song_) {
    ++iIndex;
  }
  return iIndex;
}

static const char* prepare_song_failures[] = {
    "success",
    "no_current_song",
    "card_mount_failed",
    "load_interrupted",
};

}

=======
>>>>>>> Stashed changes

  static int ResetPlayerOptions(T* p, lua_State* L) {
    p->ResetPlayerOptions(Enum::Check<PlayerNumber>(L, 1));
    COMMON_RETURN_SELF;
  }

  static int RefreshNoteSkinData(T* p, lua_State* L) {
    NOTESKIN->RefreshNoteSkinData(p->cur_game_);
    COMMON_RETURN_SELF;
  }

  static int Dopefish(T* p, lua_State* L) {
    lua_pushboolean(L, p->dopefish_);
    return 1;
  }

  static int LoadProfiles(T* p, lua_State* L) {
    bool load_edits = true;
    if (lua_isboolean(L, 1)) {
      load_edits = BArg(1);
    }
    p->LoadProfiles(load_edits);
    SCREENMAN->ZeroNextUpdate();
    COMMON_RETURN_SELF;
  }

  static int SaveProfiles(T* p, lua_State* L) {
    p->SavePlayerProfiles();
    SCREENMAN->ZeroNextUpdate();
    COMMON_RETURN_SELF;
  }

  static int SetFailTypeExplicitlySet(T* p, lua_State* L) {
    p->fail_type_was_explicitly_set_ = true;
    COMMON_RETURN_SELF;
  }

  static int StoreRankingName(T* p, lua_State* L) {
    p->StoreRankingName(Enum::Check<PlayerNumber>(L, 1), SArg(2));
    COMMON_RETURN_SELF;
  }

  DEFINE_METHOD(HaveProfileToLoad, HaveProfileToLoad())
  DEFINE_METHOD(HaveProfileToSave, HaveProfileToSave())

  static bool AreStyleAndPlayModeCompatible(
      T* p, lua_State* L, const Style* style, PlayMode play_mode) {
    if (play_mode != PLAY_MODE_BATTLE && play_mode != PLAY_MODE_RAVE) {
      return true;
    }

    // Do not allow styles with StepsTypes with shared sides or that are one
    // player only with Battle or Rave.
    if (style->m_StyleType != StyleType_TwoPlayersSharedSides) {
      std::vector<const Style*> styles;
      GAMEMAN->GetCompatibleStyles(p->cur_game_, 2, styles);
      for (const Style* s : styles) {
        if (s->m_StepsType == style->m_StepsType) {
          return true;
        }
      }
    }
    luaL_error(
        L, "Style %s is incompatible with PlayMode %s", style->m_szName,
        PlayModeToString(play_mode).c_str());
    return false;
  }

  static void ClearIncompatibleStepsAndTrails(T* p, lua_State* L) {
    FOREACH_HumanPlayer(pn) {
      const Style* style = p->GetCurrentStyle(pn);
      if (p->cur_steps_[pn] &&
          (!style || style->m_StepsType != p->cur_steps_[pn]->m_StepsType)) {
        p->cur_steps_[pn].Set(nullptr);
      }
      if (p->cur_trail_[pn] &&
          (!style || style->m_StepsType != p->cur_trail_[pn]->m_StepsType)) {
        p->cur_trail_[pn].Set(nullptr);
      }
    }
  }

  static int SetCurrentStyle(T* p, lua_State* L) {
    const Style* style = nullptr;
    if (lua_isstring(L, 1)) {
      RString style_str = SArg(1);
      style = GAMEMAN->GameAndStringToStyle(GAMESTATE->cur_game_, style_str);
      if (!style) {
        luaL_error(
            L, "SetCurrentStyle: %s is not a valid style.", style_str.c_str());
      }
    } else {
      style = Luna<Style>::check(L, 1);
    }

    StyleType style_type = style->m_StyleType;
    if (p->GetNumSidesJoined() == 2 &&
				(style_type == StyleType_OnePlayerOneSide ||
         style_type == StyleType_OnePlayerTwoSides)) {
      luaL_error(L, "Too many sides joined for style %s", style->m_szName);
    } else if (
        p->GetNumSidesJoined() == 1 &&
        (style_type == StyleType_TwoPlayersTwoSides ||
         style_type == StyleType_TwoPlayersSharedSides)) {
      luaL_error(L, "Too few sides joined for style %s", style->m_szName);
    }

    if (!AreStyleAndPlayModeCompatible(p, L, style, p->play_mode_)) {
      COMMON_RETURN_SELF;
    }
    PlayerNumber pn = Enum::Check<PlayerNumber>(L, 2, true, true);

    p->SetCurrentStyle(style, pn);
    ClearIncompatibleStepsAndTrails(p, L);

    COMMON_RETURN_SELF;
  }

  static int SetCurrentPlayMode(T* p, lua_State* L) {
    PlayMode play_mode = Enum::Check<PlayMode>(L, 1);
    if (AreStyleAndPlayModeCompatible(
            p, L, p->GetCurrentStyle(PLAYER_INVALID), play_mode)) {
      p->play_mode_.Set(play_mode);
    }
    COMMON_RETURN_SELF;
  }

  static int SetStepsForEditMode(T* p, lua_State* L) {
    // Arg forms:
    // 1.  Edit existing steps:
    //    song, steps
    // 2.  Create new steps to edit:
    //    song, nil, stepstype, difficulty
    // 3.  Copy steps to new difficulty to edit:
    //    song, steps, stepstype, difficulty
    Song* song = Luna<Song>::check(L, 1);
    Steps* steps = nullptr;
    if (!lua_isnil(L, 2)) {
      steps = Luna<Steps>::check(L, 2);
    }
    // Form 1.
    if (steps != nullptr && lua_gettop(L) == 2) {
      p->cur_song_.Set(song);
      p->cur_steps_[PLAYER_1].Set(steps);
      p->SetCurrentStyle(
          GAMEMAN->GetEditorStyleForStepsType(steps->m_StepsType),
          PLAYER_INVALID);
      p->cur_course_.Set(nullptr);
      return 0;
    }
    StepsType steps_type = Enum::Check<StepsType>(L, 3);
    Difficulty difficulty = Enum::Check<Difficulty>(L, 4);
    Steps* new_steps = song->CreateSteps();
    RString edit_name;
    // Form 2.
    if (steps == nullptr) {
      new_steps->CreateBlank(steps_type);
      new_steps->SetMeter(1);
      edit_name = "";
    }
    // Form 3.
    else {
      new_steps->CopyFrom(steps, steps_type, song->m_fMusicLengthSeconds);
      edit_name = steps->GetDescription();
    }
    SongUtil::MakeUniqueEditDescription(song, steps_type, edit_name);
    steps->SetDescription(edit_name);
    song->AddSteps(new_steps);
    p->cur_song_.Set(song);
    p->cur_steps_[PLAYER_1].Set(steps);
    p->SetCurrentStyle(
        GAMEMAN->GetEditorStyleForStepsType(steps->m_StepsType),
        PLAYER_INVALID);
    p->cur_course_.Set(nullptr);
    return 0;
  }

};

LUA_REGISTER_CLASS(GameState)
// lua end

/*
 * (c) 2001-2004 Chris Danford, Glenn Maynard, Chris Gomez
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
