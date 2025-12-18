#ifndef SCORE_SIGNER_H
#define SCORE_SIGNER_H

#include <string>

// Mock HighScore struct if standard one isn't easily linkable
struct HighScoreData {
	int score;
	float percent;
	std::string playerID;
	long long timestamp;
};

class ScoreSigner
{
public:
	// Generates a simulated HMAC signature for the score
	static std::string SignScore(const HighScoreData& hs);

	// Verifies the signature matches the data
	static bool VerifyScore(const HighScoreData& hs, const std::string& signature);

private:
	static std::string MockHMAC(const std::string& data, const std::string& key);
};

#endif
