#ifndef SCREEN_TOURNAMENT_LADDER_H
#define SCREEN_TOURNAMENT_LADDER_H

#include "ScreenWithMenuElements.h"
#include "BitmapText.h"
#include "Quad.h"
#include "TournamentBracket.h"

struct LadderEntry {
	std::string rank;
	std::string name;
	int points;
	std::string division;
};

class ScreenTournamentLadder : public ScreenWithMenuElements
{
public:
	virtual void Init();
	virtual void Input( const InputEventPlus &input );
	virtual void HandleScreenMessage( const ScreenMessage SM );

private:
	void RefreshView();

	BitmapText m_textTitle;
	BitmapText m_textDivision;
	BitmapText m_textViewMode;

	bool m_bShowBracket;
	TournamentBracket m_Bracket;

	// Simple list display for MVP
	std::vector<LadderEntry> m_LadderEntries;
	std::vector<BitmapText*> m_pRowTexts;

	int m_iSelection;
};

#endif
