#include "global.h"
#include "ProfileExporter.h"
#include "EconomyManager.h"
#include "ContentSwarmManager.h"
#include "RageFile.h"
#include "RageLog.h"

std::string ProfileExporter::ExportToJSON()
{
	// Gather Data
	long long balance = EconomyManager::Instance()->GetBalance("WALLET_PLAYER");
	long long earnings = EconomyManager::Instance()->GetMiningReward();
	long long bandwidth = ContentSwarmManager::Instance()->GetSessionBandwidthEarnings();

	// Manual JSON construction (for MVP simplicity vs pulling in JsonCpp headers here)
	std::string json = "{";
	json += "\"app\": \"StepMania Network\",";
	json += "\"version\": \"1.0\",";
	json += "\"user\": {";
	json += "  \"wallet_address\": \"WALLET_PLAYER\",";
	json += ssprintf("  \"balance\": %lld,", balance);
	json += ssprintf("  \"mining_rewards\": %lld,", earnings);
	json += ssprintf("  \"bandwidth_contribution\": %lld", bandwidth);
	json += "},";
	json += "\"stats\": {";
	json += "  \"rank_division\": \"Gold\",";
	json += "  \"tournament_lp\": 1200";
	json += "}";
	json += "}";

	return json;
}

bool ProfileExporter::SaveExport(const std::string& path)
{
	RageFile f;
	if( !f.Open(path, RageFile::WRITE) )
	{
		LOG->Warn("ProfileExporter: Failed to open %s", path.c_str());
		return false;
	}

	std::string data = ExportToJSON();
	f.Write( data );
	f.Close();
	return true;
}
