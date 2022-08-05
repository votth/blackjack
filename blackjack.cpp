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

// Variables
// ???
int i, r, tmp, round_count;
int tmp_point = 0;

// Players
const int playerCount = 5;
const std::string name[playerCount] = {"John", "Kim", "Leo", "Kati", "DEALER"};

// Creat record.txt
void Blackjack::RecordCreate() {
	std::ofstream fp;
	fp.open("record.txt");
	fp << "----------------------------------------\n"
	   << "-----         Record table         -----\n"
	   << "----------------------------------------\n"
	   << "\nExplication: O means win, X means lost, - means tied\n\n"
	   << "\nRound";
	for (i = 0; i < playerCount - 1; ++i) {
		fp << "\t\t" << name[i];
	}
	fp << "\n\n";
	fp.close();
};

// Open rule.txt to read
void Blackjack::Rule() {
	/*
	FILE *f;
	char s;
	f = fopen("rule.txt", "r");
	if (f) {
		while( (s = getc(f)) != EOF )
			putchar(s);
		fclose(f);
	} else
		std::cout << "Error opening Rules!\n";
	*/
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

// Menu
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

	// Blackjack b;
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
					waitKey();
					break;
				}
				system("clear");
				Round();
				break;
			}
			case '1': {
				system("clear");
				RecordCreate();
				round_count = 0;
				Round();
				waitKey();
				break;
			}
			case '2': {
				system("clear");
				LeadingBoard();
				waitKey();
				break;
			}
			case '3': {
				system("clear");
				Rule();
				waitKey();
				break;
			}
			case '4': {
				std::cout << "Thank you for playing!\tGoodbye!!!\n";
				break;
			}
			default: {
				std::cout << "Error! Please try again!\n";
				waitKey();
				system("clear");
				break;
			}
		}
	}
}

void Blackjack::RoundStart() {
	std::cout << "----  Round " << std::setw(2) << round_count << "  ----\n";
	player *curP = new player();
	firP = NULL;
	// Random card value generator
	srand(time(0));

	for (i = 0; i < playerCount - 1; ++i) {
		player *newP = new player();
		// Fill in new player node
		newP->name = name[i];
		std::cout << "Player "<< newP->name << "\n";
		// Add first 2 cards to player's hand
		DealCard(newP);
		DealCard(newP);
		// Print their current hand
		// newP->PrintPlayer();
		std::cout << "--------\n";

		// Add node to chain
		if (firP) {
			curP->next = newP;
			curP = curP->next;
		} else {
			firP = newP;
			curP = firP;
		}
		newP = NULL;
		delete newP;
	}
	curP = NULL;
	delete curP;

	dealer->name = name[playerCount];
	DealCard(dealer);
	DealCard(dealer);
}

void Blackjack::Round() {
	player *curP = new player();
	do {
		round_count++;
		RoundStart();

		std::cout << "End of RoundStart(), continue from Round()\n";
		player *tmpP = new player();
		tmpP = firP;
		while (tmpP->next) {
			tmpP->PrintPlayer();
			tmpP = tmpP->next;
		}
		tmpP = NULL;
		delete tmpP;
		delete firP;

		waitKey();
		Menu();

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
				std::cin >> tmp;
				tmp_point = curP->point;
				while (tmp != 1 && tmp != 2) {
					std::cout << "Error!"
							  << "\n[1] Stand\n[2] Hit";
					std::cin >> tmp;
				}
				if (tmp==1) {
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
			} while(tmp!=1);
			std::getchar();
			curP = curP->next;
			system("clear");
		}

		//Result();
		std::cout << "\n\n"
				  << "Do you want to keep playing?"
				  << "\n[1] Yes\n[2] No\n";
		std::cin >> pick;
		system("clear");
	} while(pick == '1');
	delete(curP);
}

// Deal card to curPent player's pointer
void Blackjack::DealCard(player *curP) {
	r = rand() % 13;
	curP->hand += card[r];
	curP->hand += " ";
	curP->point += value[r];
}

/*
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
