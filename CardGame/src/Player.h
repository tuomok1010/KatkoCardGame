#pragma once

#include <list>

#include "Deck.h"

class Player
{
public:
	Player();
	~Player();

	void ClearHandAndTable();

	inline std::string GetName() const { return name; }
	inline int GetScore() const { return score; }
	inline void SetName(std::string name) { this->name = name; }

	std::list<Deck::Card> Hand;
	std::list<Deck::Card> CardsOnTable;

	bool isDealer{ false };
	bool isMyTurn{ false };
	bool isOnLead{ false };

	int score{};


private:
	std::string name{};
};

