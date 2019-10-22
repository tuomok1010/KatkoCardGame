#include "Deck.h"
#include "Player.h"

#include <random>
#include <algorithm>

Deck::Deck()
{
	ArrangeDeck();
}


Deck::~Deck()
{
}

void Deck::ArrangeDeck()
{
	Cards.clear();

	// Init the deck with spades
	for (int i = 2; i <= 14; ++i)
	{
		Cards.emplace_back(Card(i, "Spades"));
	}

	// Init the deck with hearts
	for (int i = 2; i <= 14; ++i)
	{
		Cards.emplace_back(Card(i, "Hearts"));
	}

	// Init the deck with diamonds
	for (int i = 2; i <= 14; ++i)
	{
		Cards.emplace_back(Card(i, "Diamonds"));
	}

	// Init the deck with clubs
	for (int i = 2; i <= 14; ++i)
	{
		Cards.emplace_back(Card(i, "Clubs"));
	}
}

void Deck::Shuffle()
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::shuffle(Cards.begin(), Cards.end(), rng);
}

void Deck::PrintDeck()
{
	for (const auto& card : Cards)
	{
		std::cout << card << "\n";
	}

	std::cout << std::endl;
}

Deck::Card Deck::GetCard()
{
	Card card = Cards.back();
	Cards.pop_back();
	return card;
}

void Deck::DealCardToPlayer(Player& playerToDeal)
{
	playerToDeal.Hand.emplace_back(GetCard());
}

void Deck::DealCardsToAllPlayers(std::vector<Player>& players, int numOfCardsToDealPerPlayer)
{
	for (size_t i = 0; i < players.size(); ++i)
	{
		for (int j = 0; j < numOfCardsToDealPerPlayer; ++j)
		{
			DealCardToPlayer(players[i]);
		}
	}
}
