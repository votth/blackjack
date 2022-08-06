#ifndef BLACKJACK_H
#define BLACKJACK_H

// Headers
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <limits>
// string
#include <string>
// map
#include <map>


class Blackjack {
private:
	class Players {
	protected:
		std::string name = "";
		std::string hand = "";
		int point = 0;
		char winCount = 'X';
	private:
		Players* next = nullptr;

	public:
		// Constructor
		Players();
		Players(const std::string& name);
		// Destructor
		~Players();
		// Edit methods
		void SetName(const std::string& name);
		void UpdateHand(const std::string& card);
		void UpdatePoint(const int& value);
		/* void UpdateWin(char &c) {}; */
		void UpdateNext(Players* next);
		// Print methods
		Players* GetNext();
		virtual void PrintPlayer();

	};
	// Node's member
	Players* firP = new Players();
	// Dealer
	// player* dealer = new player();
	class Dealer : public Players {
		std::string hiddenCard = "";
		int hiddenPoint = 0;
	public:
		void PrintPlayer();
		void HideHand();
	};
	Dealer *dealer = new Dealer();

	// Global variables
	// card-value pairs
	/*
	const std::map<std::string, int> card {
		{"2", 2}, {"3", 3}, {"4", 4},
		{"5", 5}, {"6", 6}, {"7", 7},
		{"8", 8}, {"9", 9}, {"10", 10},
		{"Jack", 10}, {"Queen", 10}, {"King", 10}, {"Ace", 11}
	};
	*/
	const std::string card[13] = {
		"2", "3", "4", "5", "6", "7", "8", "9",
		"10", "Jack", "Queen", "King", "Ace"
	};
	const int value[13] = {
		 2,   3,   4,   5,   6,   7,   8,   9,
		 10,   10,     10,      10,     11
	};
	// For menu option
	char pick{};


public:
	// Menu builder
	void Menu();
	void RecordCreate();
	void LeadingBoard();
	void Rule();

	// Mame methods
	void RoundStart();	//give out 2 cards for each person
	void Round();		//player takes turn one after another
	void DealCard(Players *curr);
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
