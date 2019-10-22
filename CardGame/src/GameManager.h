#pragma once

#include "Deck.h"
#include "Player.h"

class GameManager
{
public:
	void ProcessStartGameMenu();
	void ProcessInstructionsMenu();
	void ProcessQuitGameMenu();
	void ProcessMainMenu();

	void PlayRound();
	void PlayCard();
	bool ProcessPlayedCard(const Deck::Card& CardToBeValidated);
	// Deprecated now since CheckWhoIsWinning() was implemented
	//void SetLeadingPlayer(Player& player);
	void CheckWhoIsWinning();
	void FinishIteration();
	bool RoundFinished();
	bool GameFinished();
	void DrawTable();
	void ResetPlayerScores();

private:


private:
	Deck deck;
	Deck::Card CurrentHighestCard;
	std::vector<Player> players;
	unsigned int currentPlayerTurnIndex;
	unsigned int GameCounter{ 1 };
	unsigned int RoundCounter{ 1 };

	bool runGame{ false };
	bool quitGame{ false };
};

