#include "GameManager.h"
#include "config.h"

#include <iostream>
#include <iomanip>


void GameManager::ProcessMainMenu()
{
	char selection{'0'};

	do
	{
		CLEARSCRN;

		std::cout
			<< std::left << std::setw(20) << " " << "~ Welcome to Katko Card Game ~" << std::right << std::setw(20) << " \n\n"
			<< std::left << std::setw(20) << " " << "        1. Start Game" << std::right << std::setw(20) << " \n\n"
			<< std::left << std::setw(20) << " " << "        2. Instructions" << std::right << std::setw(20) << " \n\n"
			<< std::left << std::setw(20) << " " << "        3. Quit        " << std::right << std::setw(20) << " \n\n\n"
			<< std::left << std::setw(20) << " " << "      enter 1-3 to select     " << std::right << std::setw(20) << " \n" << std::endl;

		std::cin >> selection;

		switch (selection)
		{
		case '1':
			ProcessStartGameMenu();
			break;
		case '2':
			selection = '0';
			ProcessInstructionsMenu();
			break;
		case '3':
			selection = '0';
			ProcessQuitGameMenu();

			if (quitGame)
				selection = '3';

			break;
		default:
			selection = '0';
			std::cout << "Invalid input. Please try again." << std::flush;
		}
	} while (selection == '0');

}

void GameManager::ProcessStartGameMenu()
{
	CLEARSCRN;

	int numPlayers{};

	do
	{
		std::cout << std::left << std::setw(20) << " " << "Enter the amount of players(2-4): ";
		std::cin >> numPlayers;

	} while (numPlayers < 2 || numPlayers > 4);

	std::cin.ignore();

	for (unsigned int i = 0; i < numPlayers; ++i)
	{
		players.emplace_back();
		
		std::string playerName{};
		std::cout << std::left << std::setw(20) << " " << "Enter player " << i + 1 << " name: ";
		std::getline(std::cin, playerName);
		players[i].SetName(playerName);
	}

	std::cout << "\n\n";
	std::cout << std::left << std::setw(20) << " " << "Press enter to start the game" << std::flush;
	std::cin.get();

	runGame = true;
}

void GameManager::ProcessInstructionsMenu()
{
	CLEARSCRN;

	std::cout
		<< std::left << std::setw(20) << " " << "~ The Rules of Katko ~" << std::right << std::setw(20) << " \n\n"
		<< std::left << std::setw(20) << " " << "Ace is the highest card(14). 2 is the lowest. Uses a standard 52 card deck.\n"
		<< std::left << std::setw(20) << " " << "Everyone gets 5 cards initially. The player next to the dealer begins.\n"
		<< std::left << std::setw(20) << " " << "The player who begins decides which suit is played. Other players must\n"
		<< std::left << std::setw(20) << " " << "play the same suit if they have any. Otherwise they can play any card.\n"
		<< std::left << std::setw(20) << " " << "After everyone has played a card, whoever played the highest card of the same suit\n"
		<< std::left << std::setw(20) << " " << "will get the next turn and gets to choose the next suit to play.\n"
		<< std::left << std::setw(20) << " " << "When players only have 1 card left, the player that plays the highest card wins and gets a point.\n\n"
		<< std::left << std::setw(20) << " " << "This is a multiplayer game played with a shared screen! When it is a player's turn, everyone\n"
		<< std::left << std::setw(20) << " " << "else should look away from the screen! Fancy isn't it ;) \n"
		<< std::left << std::setw(20) << " " << "https://fi.wikipedia.org/wiki/Katko_(korttipeli) \n\n\n"
		<< std::left << std::setw(20) << " " << "Press enter to return to main menu " << std::flush;

	std::cin.ignore();
	std::cin.get();

}

void GameManager::ProcessQuitGameMenu()
{
	CLEARSCRN;

	char input{};
	std::cout
		<< std::left << std::setw(20) << " " << "Are you sure you want to quit(Y/N): " << std::right << std::setw(20);

	std::cin >> input;
	if (input == 'Y' || input == 'y')
		quitGame = true;
	else
		return;
}

void GameManager::PlayRound()
{
	while (runGame)
	{
		// Loop through the players and set a player as the dealer
		for (unsigned int i = 0; i < players.size(); ++i)
		{
			players[i].isDealer = true;
			
			deck.ArrangeDeck();
			std::cout << players[i].GetName() << " shuffles the cards.\n";
			deck.Shuffle();
			std::cout << players[i].GetName() << " deals the cards.\n";
			deck.DealCardsToAllPlayers(players);

			// The iterator that checks which player's turn it is. Is always the one next to the dealer. If the dealer is the last player in the vector,
			// this iterator gets set back to 0.
			currentPlayerTurnIndex = (i == players.size() - 1 ? 0 : i + 1);

			while (true)
			{
				CLEARSCRN;
				DrawTable();
				PlayCard();
				CheckWhoIsWinning();
				FinishIteration();

				if (RoundFinished())
					break;
			}

			if (GameFinished())
				break;

			// Change dealer
			players[i].isDealer = false;

			if (i == players.size() - 1)
				i = -1;
		}

		++GameCounter;
		ResetPlayerScores();

		char input{};
		do
		{
			std::cout << "Play again?(Y/N): ";
			std::cin >> input;

		} while (input != 'Y' && input != 'N');

		if (input == 'Y')
			runGame = true;
		else if (input == 'N')
			runGame = false;
	}




}

void GameManager::PlayCard()
{
	players[currentPlayerTurnIndex].isMyTurn = true;

	int numOfCard{ 1 };
	int cardSelection{};

	std::cout << "\nIt is player " << players[currentPlayerTurnIndex].GetName() << " turn. Press enter when you are ready!" << std::endl;
	if (CurrentHighestCard.GetSuit() != "")
		std::cout << "The current highest card in the table is " << CurrentHighestCard << std::endl;
	else
		std::cout << "You get to choose which suit to play!" << std::endl;

	std::cin.get();

	std::cout << "Select a card to play: ";
	for (auto it = players[currentPlayerTurnIndex].Hand.begin(); it != players[currentPlayerTurnIndex].Hand.end(); ++it)
	{
		std::cout << numOfCard << ". " << *it << " ";
		++numOfCard;
	}

	std::cin >> cardSelection;

	auto it = players[currentPlayerTurnIndex].CardsOnTable.end();

	auto it2 = players[currentPlayerTurnIndex].Hand.begin();
	std::advance(it2, cardSelection - 1);

	// Repetition of code here...this will sound like a lot of work but consider refactoring?
	while (!ProcessPlayedCard(*it2))
	{
		std::cout << "Select a card to play: ";
		std::cin >> cardSelection;
		it2 = players[currentPlayerTurnIndex].Hand.begin();
		std::advance(it2, cardSelection - 1);
	}

	players[currentPlayerTurnIndex].CardsOnTable.splice(it, players[currentPlayerTurnIndex].Hand, it2);

	std::cin.ignore();

	players[currentPlayerTurnIndex].isMyTurn = false;

	if (currentPlayerTurnIndex == players.size() - 1)
		currentPlayerTurnIndex = 0;
	else
		++currentPlayerTurnIndex;


}

bool GameManager::ProcessPlayedCard(const Deck::Card& CardToBeValidated)
{
	// If no card has been played in this iteration of the round, set the card as highest
	if (CurrentHighestCard.GetSuit() == "")
	{
		CurrentHighestCard = CardToBeValidated;
		return true;
	}
	// If the new card is of the same suit and is larger value than the current highest card, make it the new highest card
	// If it is not higher value, just return true
	else if (CurrentHighestCard.GetSuit() == CardToBeValidated.GetSuit())
	{
		if (CardToBeValidated.GetValue() > CurrentHighestCard.GetValue())
		{
			CurrentHighestCard = CardToBeValidated;
		}
		return true;
	}
	// If the played card is not same suit as the current highest card, make sure the player doesnt have any same suit cards in their hand,
	// because if they do, they have to play those!
	else
	{
		for (auto card : players[currentPlayerTurnIndex].Hand)
		{
			if (card.GetSuit() == CurrentHighestCard.GetSuit())
			{
				std::cout << "You must play the same suit!(" << CurrentHighestCard.GetSuit() << ")" << std::endl;
				return false;
			}
		}
	}

	// Return true by default
	return true;
}

// Deprecated now since CheckWhoIsWinning() was implemented. TODO remove whenever you feel like it :D 

//void GameManager::SetLeadingPlayer(Player& player)
//{
//	player.isOnLead = true;
//	for (int i = 0; i < players.size(); ++i)
//	{
//		if (i == currentPlayerTurnIndex)
//			continue;
//		players[i].isOnLead = false;
//	}
//}

void GameManager::CheckWhoIsWinning()
{
	for (int i = 0; i < players.size(); ++i)
	{
		for (const auto& card : players[i].CardsOnTable)
		{
			if (card.GetSuit() == CurrentHighestCard.GetSuit() && card.GetValue() == CurrentHighestCard.GetValue())
			{
				players[i].isOnLead = true;
			}
			else
			{
				players[i].isOnLead = false;
			}
		}
	}
}

void GameManager::FinishIteration()
{
	bool allHavePlayed{ false };

	for (int j = 0; j < players.size() - 1; ++j)
	{
		if (players[j].CardsOnTable.size() == players[j + 1].CardsOnTable.size())
		{
			allHavePlayed = true;
		}
		else
		{
			allHavePlayed = false;
			break;
		}
	}

	if (allHavePlayed)
	{
		CurrentHighestCard.SetSuit("");
		CurrentHighestCard.SetValue(-1);

		for (int j = 0; j < players.size(); ++j)
		{
			if (players[j].isOnLead)
			{
				currentPlayerTurnIndex = j;
			}
		}
	}
}

bool GameManager::RoundFinished()
{
	bool allCardsPlayed{ false };
	for (const auto& player : players)
	{
		if (player.CardsOnTable.size() == 5)
			allCardsPlayed = true;
		else
		{
			allCardsPlayed = false;
			break;
		}
	}
	if (allCardsPlayed)
	{
		for (auto& player : players)
		{
			if (player.isOnLead)
			{
				player.score++;
				RoundCounter++;
				return true;
			}
		}
	}

	return false;
}

bool GameManager::GameFinished()
{
	for (auto& player : players)
		player.ClearHandAndTable();

	for (auto& player : players)
	{
		if (player.score == 3)
		{
			std::cout << player.GetName() << " won the game! Congrats!!" << std::endl;
			return true;
		}
	}

	return false;
}

void GameManager::DrawTable()
{
	// Prints the table in a nice formatted way...or at least that's the plan

	std::cout << "======= ~~ GAME\t\t" << GameCounter << " ~~ =======\n";
	std::cout << "-------  ~ round\t" << RoundCounter << " ~ -------\n\n";

	for (size_t i = 0; i < players.size(); ++i)
	{
		std::cout << std::left << std::setw(20) << players[i].GetName() << "(Score: " << players[i].score << ")" << "\n";

		for (auto it = players[i].CardsOnTable.begin(); it != players[i].CardsOnTable.end(); ++it)
		{
			std::cout << *it << "\n";
		}
		std::cout << "=================================\n" << std::flush;
	}
}

void GameManager::ResetPlayerScores()
{
	for (auto& player : players)
		player.score = 0;
}
