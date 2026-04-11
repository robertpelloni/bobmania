#ifndef MSD_CALCULATOR_H
#define MSD_CALCULATOR_H

class Steps;

class MsdCalculator
{
public:
	// Calculate the "Mine Sweeper Density" (Technical Difficulty) of a chart
	// This mirrors the logic found in Etterna (MinaCalc) but simplified for MVP.
	static float CalculateMSD( const Steps* pSteps );

	// Helper to get a string representation for the UI
	static std::string GetMsdString( const Steps* pSteps );

	// Returns tags like "Stream", "Jumpstream", "Jack" based on note data
	static std::vector<std::string> GetTags( const Steps* pSteps );
};

#endif
