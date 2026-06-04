#include "global.h"

#include "Course.h"

#include <climits>
#include <cstddef>
#include <vector>

#include "CourseLoaderCRS.h"
#include "Game.h"
#include "GameManager.h"
#include "GameState.h"
#include "LuaManager.h"
#include "PlayerNumber.h"
#include "Preference.h"
#include "PrefsManager.h"
#include "ProfileManager.h"
#include "RadarValues.h"
#include "RageLog.h"
#include "RageTypes.h"
#include "RageUtil.h"
#include "RageUtil/RandomNumbers.h"
#include "Song.h"
#include "SongCacheIndex.h"
#include "SongUtil.h"
#include "StdString.h"
#include "Steps.h"
#include "Style.h"
#include "ThemeManager.h"
#include "UnlockManager.h"

static Preference<int> MAX_SONGS_IN_EDIT_COURSE("MaxSongsInEditCourse", -1);

static const char* SongSortNames[] = {
    "Randomize", "MostPlays", "FewestPlays", "TopGrades", "LowestGrades",
};
=======
>>>>>>> Stashed changes
const int MAX_BOTTOM_RANGE = 10;

#define SORT_PREFERRED_COLOR THEME->GetMetricC("Course", "SortPreferredColor")
#define SORT_LEVEL1_COLOR THEME->GetMetricC("Course", "SortLevel1Color")
#define SORT_LEVEL2_COLOR THEME->GetMetricC("Course", "SortLevel2Color")
#define SORT_LEVEL3_COLOR THEME->GetMetricC("Course", "SortLevel3Color")
#define SORT_LEVEL4_COLOR THEME->GetMetricC("Course", "SortLevel4Color")
#define SORT_LEVEL5_COLOR THEME->GetMetricC("Course", "SortLevel5Color")

RString CourseEntry::GetTextDescription() const {
  std::vector<RString> entry_description;
  Song* song = song_id_.ToSong();
  if (song) {
    entry_description.push_back(song->GetTranslitFullTitle());
  } else {
    entry_description.push_back("Random");
  }

  if (!song_criteria_.m_sGroupName.empty()) {
    entry_description.push_back(song_criteria_.m_sGroupName);
  }

  if (song_criteria_.m_bUseSongGenreAllowedList) {
    entry_description.push_back(
        join(",", song_criteria_.m_vsSongGenreAllowedList));
  }

  if (steps_criteria_.m_difficulty != Difficulty_Invalid &&
      steps_criteria_.m_difficulty != Difficulty_Medium) {
    entry_description.push_back(
        CourseDifficultyToLocalizedString(steps_criteria_.m_difficulty));
  }

  if (steps_criteria_.m_iLowMeter != -1) {
    entry_description.push_back(
        ssprintf("Low meter: %d", steps_criteria_.m_iLowMeter));
  }

  if (steps_criteria_.m_iHighMeter != -1) {
    entry_description.push_back(
        ssprintf("High meter: %d", steps_criteria_.m_iHighMeter));
  }

  if (song_sort_ != SongSort_Randomize) {
    entry_description.push_back(
        "Sort: %d" + SongSortToLocalizedString(song_sort_));
  }

  if (song_sort_ != SongSort_Randomize && choose_index_ != 0) {
    entry_description.push_back(
        "Choose " + FormatNumberAndSuffix(choose_index_) + " match");
  }

  int num_mod_changes = GetNumModChanges();
  if (num_mod_changes != 0) {
    entry_description.push_back(ssprintf("%d mod changes", num_mod_changes));
  }

  if (gain_seconds_ != 0) {
    entry_description.push_back(ssprintf("Low meter: %.0f", gain_seconds_));
  }

  RString s = join(",", entry_description);
  return s;
}

int CourseEntry::GetNumModChanges() const {
  int num_mod_changes = 0;
  if (!modifiers_.empty()) {
    num_mod_changes++;
  }
  num_mod_changes += attacks_.size();
  return num_mod_changes;
}

Course::Course()
    : m_bIsAutogen(false),
      m_sPath(""),
      m_sMainTitle(""),
      m_sMainTitleTranslit(""),
      m_sSubTitle(""),
      m_sSubTitleTranslit(""),
      m_sScripter(""),
      m_sDescription(""),
      m_sBannerPath(""),
      m_sBackgroundPath(""),
      m_sCDTitlePath(""),
      m_sGroupName(""),
      m_bRepeat(false),
      m_fGoalSeconds(0),
      m_bShuffle(false),
      m_iLives(-1),
      m_bSortByMeter(false),
      m_bIncomplete(false),
      m_vEntries(),
      m_SortOrder_TotalDifficulty(0),
      m_SortOrder_Ranking(0),
      m_LoadedFromProfile(ProfileSlot_Invalid),
      m_TrailCache(),
      m_iTrailCacheSeed(0),
      m_RadarCache(),
      m_setStyles() {
  FOREACH_ENUM(Difficulty, dc)
  m_iCustomMeter[dc] = -1;
}

CourseType Course::GetCourseType() const {
  if (m_bRepeat) {
    return COURSE_TYPE_ENDLESS;
  }

  if (m_iLives > 0) {
    return COURSE_TYPE_ONI;
  }

  if (!m_vEntries.empty() && m_vEntries[0].gain_seconds_ > 0) {
    return COURSE_TYPE_SURVIVAL;
  }
  return COURSE_TYPE_NONSTOP;
}

void Course::SetCourseType(CourseType course_type) {
  if (GetCourseType() == course_type) {
    return;
  }

  m_bRepeat = false;
  m_iLives = -1;
  if (!m_vEntries.empty()) {
    m_vEntries[0].gain_seconds_ = 0;
  }

  switch (course_type) {
    default:
      FAIL_M(ssprintf("Invalid course type: %i", course_type));
    case COURSE_TYPE_NONSTOP:
      break;
    case COURSE_TYPE_ONI:
      m_iLives = 4;
      break;
    case COURSE_TYPE_ENDLESS:
      m_bRepeat = true;
      break;
    case COURSE_TYPE_SURVIVAL:
      if (!m_vEntries.empty()) {
        m_vEntries[0].gain_seconds_ = 120;
      }
      break;
  }
}

PlayMode Course::GetPlayMode() const {
  CourseType course_type = GetCourseType();
  switch (course_type) {
    case COURSE_TYPE_ENDLESS:
      return PLAY_MODE_ENDLESS;
    case COURSE_TYPE_ONI:
      return PLAY_MODE_ONI;
    case COURSE_TYPE_SURVIVAL:
      return PLAY_MODE_ONI;
    case COURSE_TYPE_NONSTOP:
      return PLAY_MODE_NONSTOP;
    default:
      FAIL_M(ssprintf("Invalid course type: %i", course_type));
  }
}

void Course::RevertFromDisk() {
  // trying to catch invalid an Course
  ASSERT(!m_sPath.empty());

  CourseLoaderCRS::LoadFromCRSFile(m_sPath, *this);
}

RString Course::GetCacheFilePath() const {
  return SongCacheIndex::GetCacheFilePath("Courses", m_sPath);
}

void Course::Init() {
  m_bIsAutogen = false;
  m_sPath = "";

  m_sMainTitle = "";
  m_sMainTitleTranslit = "";
  m_sSubTitle = "";
  m_sSubTitleTranslit = "";
  m_sScripter = "";
  m_sDescription = "";

  m_sBannerPath = "";
  m_sBackgroundPath = "";
  m_sCDTitlePath = "";
  m_sGroupName = "";

  m_bRepeat = false;
  m_fGoalSeconds = 0;
  m_bShuffle = false;

  m_iLives = -1;
  FOREACH_ENUM(Difficulty, dc)
  m_iCustomMeter[dc] = -1;
  m_bSortByMeter = false;

  m_vEntries.clear();

  m_SortOrder_TotalDifficulty = 0;
  m_SortOrder_Ranking = 0;

  m_LoadedFromProfile = ProfileSlot_Invalid;
  m_bIncomplete = false;
  m_TrailCache.clear();
  m_iTrailCacheSeed = 0;
  m_RadarCache.clear();
}

bool Course::IsPlayableIn(StepsType steps_type) const {
  Trail trail;
  FOREACH_ShownCourseDifficulty(course_difficulty) {
    if (GetTrailUnsorted(steps_type, course_difficulty, trail)) {
      return true;
    }
  }

  // No valid trail for this StepsType.
  return false;
}

struct SortTrailEntry {
  TrailEntry entry;
  int sort_meter;

  SortTrailEntry() : entry(), sort_meter(0) {}

  bool operator<(const SortTrailEntry& rhs) const {
    return sort_meter < rhs.sort_meter;
  }
};

RString Course::GetDisplayMainTitle() const {
  if (!PREFSMAN->m_bShowNativeLanguage) {
    return GetTranslitMainTitle();
  }
  return m_sMainTitle;
}

RString Course::GetDisplaySubTitle() const {
  if (!PREFSMAN->m_bShowNativeLanguage) {
    return GetTranslitSubTitle();
  }
  return m_sSubTitle;
}

RString Course::GetDisplayFullTitle() const {
  RString sTitle = GetDisplayMainTitle();
  RString sSubTitle = GetDisplaySubTitle();

  if (!sSubTitle.empty()) {
    sTitle += " " + sSubTitle;
  }
  return sTitle;
}

RString Course::GetTranslitFullTitle() const {
  RString sTitle = GetTranslitMainTitle();
  RString sSubTitle = GetTranslitSubTitle();

  if (!sSubTitle.empty()) {
    sTitle += " " + sSubTitle;
  }
  return sTitle;
}

// This is called by many simple functions, like Course::GetTotalSeconds, and
// may be called on all songs to sort. It can take time to execute, so we cache
// the results. Returned pointers remain valid for the lifetime of the Course.
// If the course difficulty doesn't exist, nullptr is returned.
Trail* Course::GetTrail(
    StepsType steps_type, CourseDifficulty course_difficulty) const {
  ASSERT(course_difficulty != Difficulty_Invalid);

  // Check to see if the Trail cache is out of date
  if (m_iTrailCacheSeed != GAMESTATE->stage_seed_) {
    RegenerateNonFixedTrails();
    m_iTrailCacheSeed = GAMESTATE->stage_seed_;
  }

  // Look in the Trail cache
  {
    auto it = m_TrailCache.find(CacheEntry(steps_type, course_difficulty));
    if (it != m_TrailCache.end()) {
      CacheData& cache = it->second;
      if (cache.null) {
        return nullptr;
      }
      return &cache.trail;
    }
  }

  return GetTrailForceRegenCache(steps_type, course_difficulty);
}

Trail* Course::GetTrailForceRegenCache(
    StepsType steps_type, CourseDifficulty course_difficulty) const {
  // Construct a new Trail, add it to the cache, then return it.
  CacheData& cache = m_TrailCache[CacheEntry(steps_type, course_difficulty)];
  Trail& trail = cache.trail;
  trail.Init();
  if (!GetTrailSorted(steps_type, course_difficulty, trail)) {
    // This course difficulty doesn't exist.
    cache.null = true;
    return nullptr;
  }

  // If we have cached RadarValues for this trail, insert them.
  {
    auto it = m_RadarCache.find(CacheEntry(steps_type, course_difficulty));
    if (it != m_RadarCache.end()) {
      const RadarValues& radar_values = it->second;
      trail.SetRadarValues(radar_values);
    }
  }

  cache.null = false;
  return &cache.trail;
}

bool Course::GetTrailSorted(
    StepsType steps_type, CourseDifficulty course_difficulty,
    Trail& trail) const {
  if (!GetTrailUnsorted(steps_type, course_difficulty, trail)) {
    return false;
  }

  if (m_bSortByMeter) {
    // Sort according to Difficulty_Medium, since the order of songs
    // must not change across difficulties.
    Trail sort_trail;
    if (course_difficulty == Difficulty_Medium) {
      sort_trail = trail;
    } else {
      bool ok = GetTrailUnsorted(steps_type, Difficulty_Medium, sort_trail);

      // If we have any other difficulty, we must have Difficulty_Medium.
      ASSERT(ok);
    }
    ASSERT_M(
        trail.m_vEntries.size() == sort_trail.m_vEntries.size(),
        ssprintf(
            "%i %i", int(trail.m_vEntries.size()),
            int(sort_trail.m_vEntries.size())));

    std::vector<SortTrailEntry> entries;
    for (unsigned i = 0; i < trail.m_vEntries.size(); ++i) {
      SortTrailEntry sort_trail_entry;
      sort_trail_entry.entry = trail.m_vEntries[i];
      sort_trail_entry.sort_meter = sort_trail.m_vEntries[i].pSteps->GetMeter();
      entries.push_back(sort_trail_entry);
    }

    stable_sort(entries.begin(), entries.end());
    for (unsigned i = 0; i < trail.m_vEntries.size(); ++i) {
      trail.m_vEntries[i] = entries[i].entry;
    }
  }

  if (trail.m_vEntries.empty()) {
    return false;
  }
  return true;
}

// TODO: Move Course initialization after PROFILEMAN is created
}

void Course::GetAllTrails(std::vector<Trail*>& add_to) const {
  std::vector<StepsType> steps_type_to_show;
  GAMEMAN->GetStepsTypesForGame(GAMESTATE->cur_game_, steps_type_to_show);
  for (const StepsType& steps_type : steps_type_to_show) {
    GetTrails(add_to, steps_type);
  }
}

int Course::GetMeter(
    StepsType steps_type, CourseDifficulty course_difficulty) const {
  if (m_iCustomMeter[course_difficulty] != -1) {
    return m_iCustomMeter[course_difficulty];
  }
  const Trail* trail = GetTrail(steps_type);
  if (trail != nullptr) {
    return trail->GetMeter();
  }
  return 0;
}

bool Course::HasMods() const {
  return std::any_of(
      m_vEntries.begin(), m_vEntries.end(),
      [](const CourseEntry& e) { return !e.attacks_.empty(); });
}

bool Course::HasTimedMods() const {
  // NOTE(aj): What makes this different from the SM4 implementation is that
  // HasTimedMods now searches for bGlobal in the attacks; if one of
  // them is false, it has timed mods. Also returning false will probably
  // take longer than expected.
  for (const CourseEntry& course_entry : m_vEntries) {
    if (course_entry.attacks_.empty()) {
      continue;
    }
    if (std::any_of(
            course_entry.attacks_.begin(), course_entry.attacks_.end(),
            [](const Attack& a) { return !a.bGlobal; })) {
      return true;
    }
  }
  return false;
}

bool Course::AllSongsAreFixed() const {
  return std::all_of(
      m_vEntries.begin(), m_vEntries.end(),
      [](const CourseEntry& course_entry) {
        return course_entry.IsFixedSong();
      });
}

const Style* Course::GetCourseStyle(const Game* game, int num_players) const {
  std::vector<const Style*> styles;
  GAMEMAN->GetCompatibleStyles(game, num_players, styles);

  for (const Style* style : styles) {
    for (const RString& style_str : m_setStyles) {
      if (!style_str.CompareNoCase(style->m_szName)) {
        return style;
      }
    }
  }
  return nullptr;
}

void Course::InvalidateTrailCache() { m_TrailCache.clear(); }

=======
void Course::GetAllCachedTrails(std::vector<Trail*>& out) {
  TrailCache_t::iterator it;
  for (it = m_TrailCache.begin(); it != m_TrailCache.end(); ++it) {
    CacheData& cd = it->second;
    if (!cd.null) {
      out.push_back(&cd.trail);
    }
  }
>>>>>>> upstream/release
=======
>>>>>>> Stashed changes
}

bool Course::ShowInDemonstrationAndRanking() const {
  // Don't show endless courses in Ranking.
  // todo: make this a metric of course types not to show? -aj
  return !IsEndless();
}

void Course::CalculateRadarValues() {
  FOREACH_ENUM(StepsType, steps_type) {
    FOREACH_ENUM(CourseDifficulty, course_difficulty) {
      // For courses that aren't fixed, the radar values are meaningless.
      // Makes non-fixed courses have unknown radar values.
      if (AllSongsAreFixed()) {
        Trail* trail = GetTrail(steps_type, course_difficulty);
        if (trail == nullptr) {
          continue;
        }
        RadarValues radar_values = trail->GetRadarValues();
        m_RadarCache[CacheEntry(steps_type, course_difficulty)] = radar_values;
      } else {
        m_RadarCache[CacheEntry(steps_type, course_difficulty)] = RadarValues();
      }
    }
  }
}

bool Course::Matches(RString group, RString course) const {
  if (group.size() && group.CompareNoCase(this->m_sGroupName) != 0) {
    return false;
  }

  RString file = m_sPath;
  if (!file.empty()) {
    file.Replace("\\", "/");
    std::vector<RString> bits;
    split(file, "/", bits);
    const RString& last_bit = bits[bits.size() - 1];
    if (course.EqualsNoCase(last_bit)) {
      return true;
    }
  }

=======
>>>>>>> Stashed changes
}

// lua start
#include "LuaBinding.h"

/** @brief Allow Lua to have access to the CourseEntry. */
class LunaCourseEntry : public Luna<CourseEntry> {
 public:
  static int GetSong(T* p, lua_State* L) {
    if (p->song_id_.ToSong()) {
      p->song_id_.ToSong()->PushSelf(L);
    } else {
      lua_pushnil(L);
    }
    return 1;
  }
  DEFINE_METHOD(IsSecret, is_secret_);
  DEFINE_METHOD(IsFixedSong, IsFixedSong());
  DEFINE_METHOD(GetGainSeconds, gain_seconds_);
  DEFINE_METHOD(GetGainLives, gain_lives_);
  DEFINE_METHOD(GetNormalModifiers, modifiers_);
  // GetTimedModifiers - table
  DEFINE_METHOD(GetNumModChanges, GetNumModChanges());
  DEFINE_METHOD(GetTextDescription, GetTextDescription());

  LunaCourseEntry() {
    ADD_METHOD(GetSong);
    // sm-ssc additions:
    ADD_METHOD(IsSecret);
    ADD_METHOD(IsFixedSong);
    ADD_METHOD(GetGainSeconds);
    ADD_METHOD(GetGainLives);
    ADD_METHOD(GetNormalModifiers);
    // ADD_METHOD( GetTimedModifiers );
    ADD_METHOD(GetNumModChanges);
    ADD_METHOD(GetTextDescription);
  }
};

LUA_REGISTER_CLASS(CourseEntry)

// Now for the Course bindings:
/** @brief Allow Lua to have access to the Course. */

  LunaCourse() {
    ADD_METHOD(GetPlayMode);
    ADD_METHOD(GetDisplayFullTitle);
    ADD_METHOD(GetTranslitFullTitle);
    ADD_METHOD(HasMods);
    ADD_METHOD(HasTimedMods);
    ADD_METHOD(GetCourseType);
    ADD_METHOD(GetCourseEntry);
    ADD_METHOD(GetCourseEntries);
    ADD_METHOD(GetNumCourseEntries);
    ADD_METHOD(GetAllTrails);
    ADD_METHOD(GetBannerPath);
    ADD_METHOD(GetBackgroundPath);
    ADD_METHOD(GetCourseDir);
    ADD_METHOD(GetGroupName);
    ADD_METHOD(IsAutogen);
    ADD_METHOD(GetEstimatedNumStages);
    ADD_METHOD(GetScripter);
    ADD_METHOD(GetDescription);
    ADD_METHOD(GetTotalSeconds);
    ADD_METHOD(IsEndless);
    ADD_METHOD(IsNonstop);
    ADD_METHOD(IsOni);
    ADD_METHOD(GetGoalSeconds);
    ADD_METHOD(HasBanner);
    ADD_METHOD(HasBackground);
    ADD_METHOD(IsAnEdit);
    ADD_METHOD(IsPlayableIn);
    ADD_METHOD(IsRanking);
    ADD_METHOD(AllSongsAreFixed);
  }
};

LUA_REGISTER_CLASS(Course)
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
