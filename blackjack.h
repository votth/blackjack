#ifndef BLACKJACK_H
#define BLACKJACK_H

// Headers
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <limits>
// string
#include <string>

class Blackjack {
	private:
		struct player {
			std::string name = "";
			std::string hand = "";
			int point = 0;
			char winCount = 'X';	// X lose by default
			player *next = NULL;

			~player() {
				player *tmp = new player;
				player *head = this;
				while (head->next) {
					tmp = head->next;
					delete(head);
					head = tmp;
				}
			}

			public:
				virtual void PrintPlayer() {
					std::cout << "Hand: " << this->hand
							<< "\nPoint: " << this->point << "\n";
				}
		};
		// These are for the memory chain's build
		player *firP = new player();

		class dealer : public player {
			const int hidIndex = 1;
			const std::string hidCard = "(unkown)";

			void PrintPlayer() {
				std::cout << "This is the DEALER hand: " << this->hand << "\n";
			}
		};

		// Global variables
		// card-value pairs
		const std::string card[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
		const int value[13]     = { 2,   3,   4,   5,   6,   7,   8,   9,   10,   10,     10,      10,     11};
		// For menu option
		char pick;

	public:
		// menu builder
		void Menu();
		void RecordCreate();
		void LeadingBoard();
		void Rule();
		// game methods
		void RoundStart();	//give out 2 cards for each person
		void Round();		//player takes turn one after another
		void DealCard(player *curr);
		void Result();

		//
		void waitKey() {
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
