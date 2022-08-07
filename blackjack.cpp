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
	   << "\nRound";
	for (int index = 0; index < playerCount - 1; ++index) {
		fp << "\t\t" << name[index];
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
	// Create record.txt if not existed yet
	std::string line;
	std::ifstream f;
	f.open("record.txt");
	if (f.is_open()) {
		while (std::getline(f, line)) {
			round_count++;
		}
	} else RecordCreate();
	f.close();
	// Count record log
	round_count -= 9;
	if (round_count < 0) round_count = 0;

	while (pick != '4') {
		std::cout << "----------------------------------------\n"
				  << "-----   Blackjack  Simulator  1.2  -----\n"
				  << "----------------------------------------\n"
				  << "Please choose from the below functions:\n";
		//if 1st start out or have reset (chosen New) then don't print out option [0]
		if (round_count != 0)
			std::cout << "\t[0]\tContinue\n\t[1]\tNew\n\t[2]\tRecord\n\t[3]\tRules\n\t[4]\tExit\n";
		else
			std::cout << "\t[1]\tNew\n\t[2]\tRecord\n\t[3]\tRules\n\t[4]\tExit\n";
		std::cin >> pick;
		switch (pick) {
			case '0': {
				if (round_count == 0) {
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
				round_count = 0;
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
				system("clear");
				break;
			}
		}
	}
}

// Start of the game
void Blackjack::RoundStart() {
	Players *curP{};
	// Random card value generator
	srand(time(0));

	// Deal cards to DEALER
	dealer = new Dealer();
	dealer->SetName(name[playerCount - 1]);
	DealCard(dealer);
	DealCard(dealer);
	if (dealer->GetPoint() == 21) {
		BJack = true;
	}

	// Deal cards to players
	for (int index = 0; index < playerCount - 1; ++index) {
		Players *newP = new Players(name[index]);
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

	if (BJack) {
		// Result();
	}

	std::cout << "----  Initial Round  ----\n";
	// Print out all players' hand
	// Hide DEALER hand
	dealer->HideHand();
	// Players' hand
	curP = firP;
	while (curP != nullptr) {
		curP->PrintPlayer();
		curP = curP->GetNext();
	}
	curP->EmptyLink(curP);
	// DEALER's hand
	dealer->PrintPlayer();

	WaitKey();
	PlayerTurn();
}

// Players' turn
void Blackjack::PlayerTurn() {
	return;
	do {
		round_count++;
		std::cout << "----  Round " << std::setw(2) << round_count << "  ----\n";

		/*
		int draw{};	// for draw card decision
		curP = firP;
		// will stop right at dealer's data
		while (curP->next) {
			if (curP->winCount == 'O') { // If already got Blackjack then skip to the next player
				curP = curP->next;
				continue;
			}
			std::cout << "---- Round " << round_count << "----\n";
			std::cout << "---- " << curP->name <<"'s turn----\n"
					  << "Hand: " << curP->hand
					  << "\nPoint: " << curP->point << "\n";
			do {
				std::cout << "------------\n";
				std::cout << "What do you want to do?"
						  << "\n[1] Stand\n[2] Hit";
				std::cin >> draw;
				tmp_point = curP->point;
				while (draw != 1 && draw != 2) {
					std::cout << "Error!"
							  << "\n[1] Stand\n[2] Hit";
					std::cin >> draw;
				}
				if (draw==1) {
					std::cout << "Hand: " << curP->hand
							  << "\nPoint: " << curP->point << "\n";
					std::cout << "------------\n";
				} else {
					DealCard(curP);
					curP->point = tmp_point;
					if (curP->point > 21) {
						std::cout << "Busted!\n";
						curP->winCount = 'X';
						std::cout << "Hand: " << curP->hand
								  << "\nPoint: " << curP->point
								  <<"\t(x)\n";
						std::cout << "------------\n";
						break;
					}
					else
						std::cout << "Hand: " << curP->hand
								  << "\nPoint: " << curP->point <<"\n";
				}
			} while(draw!=1);
			std::getchar();
			curP = curP->next;
			system("clear");
		}

		Result();
		std::cout << "\n\n"
				  << "Do you want to keep playing?"
				  << "\n[1] Yes\n[2] No\n";
		std::cin >> pick;
		system("clear");
		*/
	} while(pick == '1');

	// Exit Round() loop, ending game before back to Menu()
	firP->EmptyLink(firP);
	dealer->EmptyLink(dealer);
}

// DealCard
void Blackjack::DealCard(Players *curP) {
	int rando = rand() % 13;
	curP->UpdateHand(cards[rando]);
	curP->UpdateHand(" ");
	curP->UpdatePoint(values[rando]);
}

/* Result
void Blackjack::Result() {
	// FILE *f;
	// f = fopen("record.txt", "a");
	// fprintf(f, "%3d", round_count);
	std::ofstream f;
	f.open("record.txt");
	f << std::setw(3) << round_count;

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
}
*/

