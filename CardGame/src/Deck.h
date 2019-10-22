#pragma once

#include <iostream>
#include <string>
#include <vector>

// TODO Consider refactoring if having a nested private class gets too tricky

// This forward declaration may not be necessary. However, it does remove some red error lines. Code seems to work without this though...
class Player;

class Deck
{
	// Making player friend because it needs access to the Card class
	friend class Player;
	friend class GameManager;

private:
	class Card
	{
	public:
		Card() {}

		Card(const Card &c2) { value = c2.value; suit = c2.suit; }

		Card(int value, std::string suit)
			: value(value), suit(suit) {}

		friend std::ostream& operator<<(std::ostream& os, const Card& card)
		{
			os << "[" << card.suit[0] << card.value << "]";
			return os;
		}

		inline std::string GetSuit() const { return suit; }
		inline int GetValue() const { return value; }

		inline void SetValue(int value) { this->value = value; }
		inline void SetSuit(std::string suit) { this->suit = suit; }

	private:
		int value{};
		std::string suit{};
	};

public:
	// Emplaces all cards into the vector of cards starting from: Spades(2-14), Hearts(2-14), Diamonds(2-14), Clubs(2-14)
	Deck();

	~Deck();

	void ArrangeDeck();

	// Uses std::shuffle and a random device to shuffle the vector of cards
	void Shuffle();

	// Prints all cards into the console
	void PrintDeck();

	// Pops one card from the end of the deck and returns it
	Deck::Card GetCard();

	// Deals a card to a player. Remember to shuffle the deck before dealing!
	void DealCardToPlayer(Player& playerToDeal);

	// Deals cards to all players. Remember to shuffle the deck before dealing!
	void DealCardsToAllPlayers(std::vector<Player>& players, int numOfCardsToDealPerPlayer = 5);

private:
	// This is the main deck
	std::vector<Card> Cards;
};
