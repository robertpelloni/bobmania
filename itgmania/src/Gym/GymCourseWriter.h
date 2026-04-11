#ifndef GYM_COURSE_WRITER_H
#define GYM_COURSE_WRITER_H

#include "Course.h"
#include <string>

class GymCourseWriter
{
public:
	// Writes a Course to a standard .crs file
	static bool WriteCourse( const Course* pCourse, const std::string& sPath );
};

#endif
