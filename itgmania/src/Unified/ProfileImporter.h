#ifndef PROFILE_IMPORTER_H
#define PROFILE_IMPORTER_H

#include <string>

class ProfileImporter
{
public:
	// Imports user data from an external JSON file (Bridge from FWBer)
	// Returns true if successful and balance was updated.
	static bool ImportFromJSON(const std::string& path);
};

#endif
