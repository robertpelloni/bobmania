#ifndef PROFILE_EXPORTER_H
#define PROFILE_EXPORTER_H

#include <string>

class ProfileExporter
{
public:
	// Exports current user data to a JSON string compatible with external apps
	static std::string ExportToJSON();

	// Helper to write to file
	static bool SaveExport(const std::string& path);
};

#endif
