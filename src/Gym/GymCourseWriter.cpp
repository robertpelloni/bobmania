#include "global.h"
#include "GymCourseWriter.h"
#include "RageFile.h"
#include "RageLog.h"
#include "Song.h"

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
	f.PutLine( "#METER:Simple;" ); // Default

	for( const auto& entry : pCourse->m_vEntries )
	{
		if( entry.pSong )
		{
			// Format: #SONG:Group/Title:Difficulty:Modstring;
			// For MVP we just assume song group/title is available.
			// Ideally we use pSong->GetSongDir() relative path logic.

			std::string songPath = entry.pSong->m_sMainTitle; // Placeholder for real path logic
			f.PutLine( "#SONG:" + songPath + ":Medium:;" );
		}
	}

	f.Close();
	return true;
}
