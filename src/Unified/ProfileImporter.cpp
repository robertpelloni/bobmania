#include "global.h"
#include "ProfileImporter.h"
#include "EconomyManager.h"
#include "RageFile.h"
#include "RageLog.h"
#include "RageUtil.h"

// Basic JSON parser mock since we don't have full JsonCpp headers handy in this context
// In a real integration, we'd use the engine's JSON lib.
std::string ExtractValue(const std::string& json, const std::string& key)
{
	size_t pos = json.find("\"" + key + "\":");
	if (pos == std::string::npos) return "";

	size_t start = json.find_first_of("0123456789", pos);
	if (start == std::string::npos) return "";

	size_t end = json.find_first_not_of("0123456789", start);
	return json.substr(start, end - start);
}

bool ProfileImporter::ImportFromJSON(const std::string& path)
{
	RageFile f;
	if( !f.Open(path, RageFile::READ) )
	{
		LOG->Warn("ProfileImporter: Failed to open %s", path.c_str());
		return false;
	}

	std::string json;
	f.Read( json, f.GetFileSize() );
	f.Close();

	// Parse Balance
	std::string sBal = ExtractValue(json, "balance");
	if( !sBal.empty() )
	{
		long long balance = StringToInt64(sBal);

		// Sync logic: In a real blockchain, we wouldn't just overwrite.
		// We'd check a signature. For MVP, we "Deposit" the difference if positive.

		long long current = EconomyManager::Instance()->GetBalance("WALLET_PLAYER");
		if( balance > current )
		{
			long long diff = balance - current;
			EconomyManager::Instance()->Transfer("WALLET_EXTERNAL", "WALLET_PLAYER", diff, "Import Deposit");
			LOG->Trace("ProfileImporter: Imported %lld coins.", diff);
			return true;
		}
	}

	return false;
}
