#include "global.h"
#include "GymCourseWriter.h"
#include "RageFile.h"
#include "RageLog.h"
#include "Song.h"
#include "Course.h"
#include "GameConstantsAndTypes.h"

bool GymCourseWriter::WriteCourse( const Course* pCourse, const std::string& sPath )
{
	if( !pCourse ) return false;

	RageFile f;
	if( !f.Open(sPath, RageFile::WRITE) )
	{
		LOG->Warn("GymCourseWriter: Failed to open %s", sPath.c_str());
		return false;
	}

	f.PutLine( "#COURSE:" + pCourse->m_sMainTitle + ";" );
	f.PutLine( "#SCRIPTER:" + pCourse->m_sScripter + ";" );
	f.PutLine( "#METER:Simple;" ); 

	for( const auto& entry : pCourse->m_vEntries )
	{
		Song* pSong = entry.songID.ToSong();
		if( pSong )
		{
			// Format: #SONG:Group/Title:Difficulty:Modstring;
			std::string songPath = pSong->m_sGroupName + "/" + pSong->m_sMainTitle;
			
			// Get Difficulty
			Difficulty diff = entry.stepsCriteria.m_difficulty;
			std::string sDiff = DifficultyToString( diff );
			
			if( diff == Difficulty_Invalid ) sDiff = "";

			f.PutLine( "#SONG:" + songPath + ":" + sDiff + ":;" );
		}
	}

	f.Close();
	return true;
}
