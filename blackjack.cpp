// HEADERS
// Custom headers
#include "blackjack.h"
// STD headers
#include <ctime>
#include <cstring>
// for file access
#include <fstream>
#include <string>
// for setw()
#include <iomanip>

// Game options
// Creat record.txt
void Blackjack::RecordCreate() {
	std::ofstream fp;
	fp.open("record.txt");
	fp << "----------------------------------------\n"
	   << "-----         Record table         -----\n"
	   << "----------------------------------------\n"
	   << "\nExplication: O means win, X means lost, - means tied\n\n"
	   << "\nGame";
	for (int index = 0; index < playerCount - 1; ++index) {
		fp << "\t\t" << names[index];
	}
	fp << "\n\n";
	fp.close();
};

// Open rule.txt to read
void Blackjack::Rule() {
	std::string line;
	std::ifstream f;
	f.open("./rule.txt");
	if (f.is_open()) {
		while (std::getline(f, line)) {
			std::cout << line << "\n";
		}
	} else {
		std::cout << "error opening Rules!\n";
	}
	f.close();
}

// Open record.txt to read
void Blackjack::LeadingBoard() {
	std::string line;
	std::ifstream f;
	f.open("./record.txt");
	if (f.is_open()) {
		while (std::getline(f, line)) {
			std::cout << line << "\n";
		}
	} else {
		std::cout << "error opening Record!\n";
	}
	f.close();
}

// Game control
// Print Menu() and wait for pick option
void Blackjack::Menu() {
	system("clear");
	// Create record.txt if not existed yet
	std::string line;
	std::ifstream f;
	f.open("record.txt");
	if (f.is_open()) {
		while (std::getline(f, line)) {
			GameCounter++;
		}
	} else RecordCreate();
	f.close();
	// Count record log
	if (GameCounter <= 9) GameCounter = 0;

	while (pick != '4') {
		std::cout << "----------------------------------------\n"
				  << "-----   Blackjack  Simulator  1.2  -----\n"
				  << "----------------------------------------\n"
				  << "Please choose from the below functions:\n";
		std::cout << "\t[1]\tNew\n\t[2]\tRecord\n\t[3]\tRules\n\t[4]\tExit\n";
		std::cin >> pick;
		switch (pick) {
			case '0': {
				if (GameCounter == 0) {
					std::cout << "Error! Please try again!\n";
					WaitKey();
					break;
				}
				system("clear");
				// PlayerTurn();
				break;
			}
			case '1': {
				// system("clear");
				GameCounter = 0;
				RoundStart();
				break;
			}
			case '2': {
				system("clear");
				LeadingBoard();
				WaitKey();
				break;
			}
			case '3': {
				system("clear");
				Rule();
				WaitKey();
				break;
			}
			case '4': {
				std::cout << "\nThe game is exiting...\n\n";
				// ExitGame();
				break;
			}
			default: {
				std::cout << "Error! Please try again!\n";
				WaitKey();
				break;
			}
		}
	}
}

// DealCard
void Blackjack::DealCard(Players *curP) {
	int rando = rand() % 13;
	curP->UpdateHand(cards[rando]);
	curP->UpdateHand(" ");
	curP->UpdatePoint(values[rando]);
}

// Start of the game
void Blackjack::RoundStart() {
	system("clear");
	GameCounter++;

	// For when starting [1] again
	firP = nullptr;

	Players *curP{};
	firP = nullptr;
	// Random card value generator
	srand(time(0));

	// Deal cards to DEALER
	dealer = new Dealer();
	dealer->SetName(names[playerCount - 1]);
	DealCard(dealer);
	DealCard(dealer);
	if (dealer->GetPoint() == 21) {
		BJack = true;
	}

	// Deal cards to players
	for (int index = 0; index < playerCount - 1; ++index) {
		Players *newP = new Players(names[index]);
		DealCard(newP);
		DealCard(newP);
		if (newP->GetPoint() == 21) {
			newP->UpdateWin('O');
			BJack = true;
		}

		// Add node to chain
		if (firP) {
			curP->UpdateNext(newP);
			curP = curP->GetNext();
		} else {
			firP = newP;
			curP = firP;
		}
		// Free
		newP = nullptr;
		newP->EmptyLink(newP);
	}

	// Natural: win by default
	if (BJack) {
		// EndGame();
	}

	std::cout << "----  Initial Round  ----\n\n";
	// Print out all players' hand
	// DEALER's hand
	dealer->HideHand();
	dealer->PrintPlayer();
	// Players' hand
	curP = firP;
	while (curP != nullptr) {
		curP->PrintPlayer();
		curP = curP->GetNext();
	}
	curP->EmptyLink(curP);

	std::cout << "[Enter] Players' turn... ";
	WaitKey();
	PlayerTurn();
}

// Players' turn
void Blackjack::PlayerTurn() {
	Players* curP{};
	curP = firP;
	std::string curName{};
	int draw{};		// for draw card decision

	// Move through players
	while (curP) {
		curName = curP->GetName();
		// Draw decision, looping for multiple cards
		do {
			system("clear");
			std::cout << "----  Players' turn  ----\n\n";
			dealer->PrintPlayer();
			std::cout << '\n';
			curP->PrintPlayer();
			std::cout << curName << ", what is your choice?\n"
					  << "\t[1] Stand\t\t[2] Hit\n";
			std::cin >> draw;
			// Error: fail when input a string
			// Input checker
			while (draw != 1 && draw != 2) {
				std::cout << "Please pick again:\n"
						  << "\t[1] Stand\t\t[2] Hit\n";
				std::cin >> draw;
			}

			// Outcomes
			if (draw == 1) {
				std::cout << curName << " chose to Stand!\n\n";
				system("sleep 0.8 && clear");
				std::cout << "----  Players' turn  ----\n\n";
				dealer->PrintPlayer();
				std::cout << "---- Final hand ----\n";
				curP->PrintPlayer();
			} else {
				std::cout << curName << " chose to Hit...\n\n";
				system("sleep 0.5 && clear");
				DealCard(curP);
				if (curP->GetPoint() > 21) {
					std::cout << "----  Players' turn  ----\n\n";
					dealer->PrintPlayer();
					std::cout << "---- Eliminated! ----\n";
					curP->PrintPlayer();
					draw = 1;
				} else {
					system("clear");
					std::cout << "----  Players' turn  ----\n\n";
					dealer->PrintPlayer();
					std::cout << "----  New hand ----\n";
					curP->PrintPlayer();
				}
			}
		} while(draw != 1);

		// Move to next player
		std::cout << "[Enter] Next player... ";
		WaitKey();
		curP = curP->GetNext();
	}

	// Move to DEALER's turn
	std::cout << "[Enter] DEALER's turn... ";
	std::getchar();
	DealerTurn();

	// DEBUG
	// To avoid memory leak error when debugging
	firP->EmptyLink(firP);
	dealer->EmptyLink(dealer);

	// Jump to Result(); ?
}

// DEALER's turn
void Blackjack::DealerTurn() {
	system("clear");
	// reveal hand slowly by flipping (unknown) to true value??
	std::cout << "---- DEALER's turn ----\n\n";
	std::cout << "Hand reveal...\n";
	dealer->PrintPlayer();
	system("sleep 1");
	system("clear");
	// Hand reveal func
	dealer->RevealHand();
	std::cout << "---- DEALER's turn ----\n\n";
	std::cout << "Hand reveal...\n";
	dealer->PrintPlayer();
	system("sleep 0.5");
	system("clear");

	// Deal more card
	if (dealer->GetPoint() < 17) {
		std::cout << "---- DEALER's turn ----\n\n";
		std::cout << "Drawing more cards...\n";
		dealer->PrintPlayer();
		system("sleep 1");
		while (dealer->GetPoint() < 17) {
			system("clear");
			std::cout << "---- DEALER's turn ----\n\n";
			std::cout << "Drawing more cards...\n";
			DealCard(dealer);
			dealer->PrintPlayer();
			system("sleep 1.2");
		}
	}

	// End
	system("clear");
	std::cout << "---- DEALER's turn ----\n\n";
	std::cout << "DEALER's final hand:\n";
	dealer->PrintPlayer();

	std::cout << "[Enter] Announcing result... ";
	std::cout << "\n\t WIP \n\n";
	WaitKey();
}

/*
void Blackjack::Result() {
	// FILE *f;
	// f = fopen("record.txt", "a");
	// fprintf(f, "%3d", GameCounter);
	std::ofstream f;
	f.open("record.txt");
	f << std::setw(3) << GameCounter;

	tmp_point = dealer->point;
	while (dealer->point < 17) {
		DealCard(dealer);
		dealer->point = tmp_point;
	}
	std::cout << dealer->name << ":\n"
			  << "Hand: " << dealer->hand
			  << "\nPoint: " << dealer->point << "\n\n\n";
	curP = firP;
	if (dealer->point>21) {
		while (curP->next) {
			if (curP->winCount != 'X')
				curP->winCount = 'O';
			// fprintf(f, "\t\t%c", curP->winCount);
			f << "\t\t" << curP->winCount;
			curP = curP->next;
		}
	} else {
		while(curP->next){
			if(curP->winCount != 'X')
				if(curP->point > dealer->point)
					curP->winCount = 'O';
				else {
					if(curP->point < dealer->point)
						curP->winCount = 'X';
					else
						curP->winCount = '-';
				}
			// fprintf(f, "\t\t%c", curP->winCount);
			f << "\t\t" << curP->winCount;
			curP = curP->next;
		}
	}
	f << "\n";
	f.close();
	// fprintf(f, "\n");
	// fclose(f);

	curP = firP;
	std::cout << "------------\n";
	std::cout << "The following people won:\n\n";
	while (curP->next) {
		if(curP->winCount == 'O')
			std::cout << curP->name
					  << "\nHand: " << curP->hand
					  << "\nPoint: " << curP->point << "\n";
		curP = curP->next;
	}

	curP = firP;
	std::cout << "------------\n";
	std::cout << "The following people tied:\n\n";
	while (curP->next) {
		if (curP->winCount == '-')
			std::cout << curP->name
					  << "\nHand: " << curP->hand
					  << "\nPoint: " << curP->point << "\n";
		curP = curP->next;
	}

	std::cout << "\n\n"
			  << "Do you want to keep playing?"
			  << "\n[1] Yes\n[2] No\n";
	std::cin >> pick;
	system("clear");
}
*/

