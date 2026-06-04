#include "global.h"

#include "CourseWriterCRS.h"

#include <vector>

#include "Course.h"
#include "RageFile.h"
#include "RageFileDriverMemory.h"
#include "RageLog.h"
#include "RageUtil.h"
#include "Song.h"

// Load the difficulty names from CourseLoaderCRS.
extern const char* g_CRSDifficultyNames[];  // in CourseLoaderCRS

// @brief Get the string of the course difficulty.
static RString DifficultyToCRSString(CourseDifficulty course_difficulty) {
  return g_CRSDifficultyNames[course_difficulty];
}

bool CourseWriterCRS::Write(
    const Course& course, const RString& path, bool saving_cache) {
  RageFile f;
  if (!f.Open(path, RageFile::WRITE)) {
    LOG->UserLog(
        "Course file", path, "couldn't be written: %s", f.GetError().c_str());
    return false;
  }

  return CourseWriterCRS::Write(course, f, saving_cache);
}

void CourseWriterCRS::GetEditFileContents(const Course* course, RString& out) {
  RageFileObjMem mem;
  CourseWriterCRS::Write(*course, mem, true);
  out = mem.GetString();
}

bool CourseWriterCRS::Write(
    const Course& course, RageFileBasic& f, bool saving_cache) {
  ASSERT(!course.m_bIsAutogen);

  f.PutLine(ssprintf("#COURSE:%s;", course.m_sMainTitle.c_str()));
  if (course.m_sMainTitleTranslit != "") {
    f.PutLine(
        ssprintf("#COURSETRANSLIT:%s;", course.m_sMainTitleTranslit.c_str()));
  }
  if (course.m_sScripter != "") {
    f.PutLine(ssprintf("#SCRIPTER:%s;", course.m_sScripter.c_str()));
  }
  if (course.m_sDescription != "") {
    f.PutLine(ssprintf("#DESCRIPTION:%s;", course.m_sDescription.c_str()));
  }
  if (course.m_bRepeat) {
    f.PutLine("#REPEAT:YES;");
  }
  if (course.m_iLives != -1) {
    f.PutLine(ssprintf("#LIVES:%i;", course.m_iLives));
  }
  if (!course.m_sBannerPath.empty()) {
    f.PutLine(ssprintf("#BANNER:%s;", course.m_sBannerPath.c_str()));
  }

  if (!course.m_setStyles.empty()) {
    std::vector<RString> styles;
    styles.insert(
        styles.begin(), course.m_setStyles.begin(), course.m_setStyles.end());
    f.PutLine(ssprintf("#STYLE:%s;", join(",", styles).c_str()));
  }

  FOREACH_ENUM(CourseDifficulty, cd) {
    if (course.m_iCustomMeter[cd] == -1) {
      continue;
    }
    f.PutLine(ssprintf(
        "#METER:%s:%i;", DifficultyToCRSString(cd).c_str(),
        course.m_iCustomMeter[cd]));
  }

  if (saving_cache) {
    f.PutLine("// cache tags:");

    for (auto it = course.m_RadarCache.begin(); it != course.m_RadarCache.end();
         ++it) {
      // #RADAR:type:difficulty:value,value,value...;
      const Course::CacheEntry& entry = it->first;
      StepsType steps_type = entry.first;
      CourseDifficulty course_difficulty = entry.second;

      std::vector<RString> all_radar_values;
      const RadarValues& radar_values = it->second;
      for (int r = 0; r < NUM_RadarCategory; ++r) {
        all_radar_values.push_back(ssprintf("%.3f", radar_values[r]));
      }
      RString line = ssprintf("#RADAR:%i:%i:", steps_type, course_difficulty);
      line += join(",", all_radar_values) + ";";
      f.PutLine(line);
    }
    f.PutLine("// end cache tags");
  }

=======
>>>>>>> Stashed changes

  return true;
}

=======
>>>>>>> Stashed changes
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
