#ifndef BLACKJACK_H
#define BLACKJACK_H

// Headers
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <limits>
// string
#include <string>
#include <cstring>
// DST
#include <array>
#include <map>

// Global variables
// card-value pairs
const std::array<std::string, 13> cards = {
	"2", "3", "4", "5", "6", "7", "8", "9",
	"10", "Jack", "Queen", "King", "Ace"
};
const std::array<int, 13> values = {
		2,   3,   4,   5,   6,   7,   8,   9,
		10,   10,     10,      10,     11
};
// Blackjack notifier
static bool BJack = false;

class Blackjack {
private:
	// Players
	class Players {
	protected:
		std::string name{};
		std::string hand{};
		int point{};
		char winCount{};
		Players* next = nullptr;

	public:
		// Big three
		// Constructor
		Players();
		Players(const std::string& name);
		// Destructor
		// ~Players();
		void EmptyLink(Players* head);

		// Methods
		// Edit
		void SetName(const std::string& name);
		void UpdateHand(const std::string& card);
		void UpdatePoint(const int& value);
		void UpdateWin(const char& c);
		void UpdateNext(Players* next);
		// Print
		int GetPoint();
		Players* GetNext();
		virtual void PrintPlayer();
	};

	// Dealer
	class Dealer : public Players {
		std::string hiddenCard = "";
		int hiddenPoint = 0;
	public:
		void PrintPlayer();
		void HideHand();
	};

public:
	// Menu input
	char pick{};
	int round_count = 0;

	// Player data
	static const int playerCount = 5;
	const std::string name[playerCount] = {
		"John", "Kim", "Leo", "Kati", "DEALER"
	};
	// Player pointers
	Players* firP{};
	Dealer* dealer{};

	// Global methods
	// Menu builder
	void Menu();
	void RecordCreate();
	void LeadingBoard();
	void Rule();

	// Game methods
	void RoundStart();	//give out 2 cards for each person
	void PlayerTurn();		//player takes turn one after another
	void DealCard(Players* curP);
	void Result();

	// WaitKey()
	void WaitKey() {
		/*
		 * If your window runs but the consoles flashes and closes immediately
		 * https://www.learncpp.com/cpp-tutorial/compiling-your-first-program/
		 */
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
		system("clear");
	}

};

#endif
