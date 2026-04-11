#include "global.h"
#include "ScoreSigner.h"
#include "RageUtil.h"
#include "CryptManager.h" // Assuming we can use internal CryptManager or mock it

// Since CryptManager might be heavy to link, we use a simple hash simulation for MVP
std::string ScoreSigner::MockHMAC(const std::string& data, const std::string& key)
{
	// Simulation: XOR with key and simple rotation
	std::string hash = "";
	for(size_t i=0; i<data.length(); ++i) {
		char c = data[i] ^ key[i % key.length()];
		hash += ssprintf("%02X", (unsigned char)c);
	}
	return "SIG_" + hash.substr(0, 32); // Truncate
}

std::string ScoreSigner::SignScore(const HighScoreData& hs)
{
	// Serialize important data
	std::string payload = ssprintf("%s:%d:%.4f:%lld",
		hs.playerID.c_str(), hs.score, hs.percent, hs.timestamp);

	// Secret Key (In reality, this would be the Player's Private Key from EconomyManager)
	std::string privateKey = "PRIVATE_KEY_" + hs.playerID;

	return MockHMAC(payload, privateKey);
}

bool ScoreSigner::VerifyScore(const HighScoreData& hs, const std::string& signature)
{
	std::string expected = SignScore(hs);
	return expected == signature;
}
