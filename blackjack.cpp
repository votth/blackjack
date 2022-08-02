#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cstring>
// for file access
#include <fstream>
#include <string>
// for setw()
#include <iomanip>
// Headers
#include "blackjack.h"

// card-value pairs
const char card[13][10] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
const int value[13]     = { 2,   3,   4,   5,   6,   7,   8,   9,   10,   10,     10,      10,     11};
/**
 * for an infinite decks of card, count[i] = 4 then no more of that card
 * int count[13]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 */
// Players
char name[5][10] = {"John", "Kim", "Leo", "Kati", "DEALER"};
// ???
int i, r, tmp, round_count;
int tmp_point = 0;
// for menu option
char pick;

void Blackjack::RecordCreate() {
	std::ofstream fp;
	fp.open("record.txt");
	fp << "----------------------------------------\n"
	   << "-----         Record table         -----\n"
	   << "----------------------------------------\n"
	   << "\nExplication: O means win, X means lost, - means tied\n\n"
	   << "\nRound\t\tJohn\t\tKim\t\tLeo\t\tKati\n\n";
	fp.close();
};

void Blackjack::Menu() {
	/*
	FILE *f;	//run through the record file by counting lines
	char line[256];
	f = fopen("record.txt", "r");
	if (f == NULL) RecordCreate();
	else
		while (fgets(line, sizeof(line), f)) round_count++;
	fclose(f);
	*/
	std::string line;
	std::ifstream f;
	f.open("record.txt");
	if (f.is_open()) {
		while (std::getline(f, line)) {
			round_count++;
		}
	} else RecordCreate();
	f.close();

	// Check if there're already a play record
	// why 9??
	round_count -= 9;
	if (round_count < 0) round_count = 0;

	Blackjack b;
	dealer = NULL;
	add = NULL;
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
					/*
					 * If your window runs but the consoles flashes and closes immediately
					 * https://www.learncpp.com/cpp-tutorial/compiling-your-first-program/
					 */
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cin.get();
					system("clear");
					break;
				}
				system("clear");
				b.Round();
				break;
			}
			case '1': {
				system("echo '' > record.txt");
				RecordCreate();
				round_count = 0;
				/*
				 * for(tmp=0;tmp<13;tmp++)	count[tmp]=0;
				 * reset the count of cards also, but if it infinite deck then no need for this
				 */
				system("clear");
				b.Round();
				system("clear");
				break;
			}
			case '2': {
				system("clear");
				b.LeadingBoard();
				system("clear");
				break;
			}
			case '3': {
				system("clear");
				b.Rule();
				system("clear");
				break;
			}
			case '4': {
				std::cout << "Thank you for playing!\tGoodbye!!!\n";
				getchar();
				break;
			}
			default: {
				std::cout << "Error! Please try again!\n";
				getchar();
				system("clear");
				break;
			}
		}
	}
}

// *s is the "hand" string
void Blackjack::DealCard(char *s) {
	r = rand() % 13;
	/*
	while (1) {
		r = rand() % 13;
		// count[r]++;
		// if(count[r] <= 4)
		// these lines are needed if playing with finite number of decks
		break;
	}
	*/
	std::strcat(s, ", ");		// just so the output look nicer: King, 8, Ace
	std::strcat(s, card[r]);
	tmp_point += value[r];	//this will become the player's new point
}

void Blackjack::RoundStart() {
	std::cout << "---- Round " << round_count << "----\n";
	// ?
	delete(add);
	first = NULL;
	srand(time(0)); //for r's random value

	i = 0;
	while (i < 5) {
		add = (struct player*) malloc(sizeof(player));
		// check if there's enough space
		if (!add) {
			std::cout << "Memory error!\n";
			getchar();
			return;
		}
		strcpy(add->name, name[i]);
		std::cout << "Player "<< add->name << "\n";
		before = NULL;
		actual = first;
		while (actual) {
			before = actual;
			actual = actual->next;
		}
		if (!before)
			first = add;
		else
			before->next = add;
		tmp_point = 0;

		// Adding card to player's hand
			// generating random number from 0 to 12
		r = rand() % 13;
		/*
		while (1) {
			r = rand() % 13;
			// count[r]++;
			// if(count[r] <= 4)
			break;
		}
		*/
		strcpy(add->hand, card[r]);
		tmp_point += value[r];

		DealCard(add->hand);
		// I couldn't find a way to directly change the uj->point's value
		add->point = tmp_point;

		// Output for Dealer is different to others, need to hide the second card aka to be facedown
		if (!std::strcmp(add->name, "DEALER")) {
			std::cout << "Hand: ";
			tmp = 0;
			while (add->hand[tmp] != ',') {
				std::cout << add->hand[tmp];
				tmp++;
			}
			std::cout << ", (unknown)\nPoint: " << add->point - value[r] << "\n";
			dealer = add; //save back the dealer point for easier access later on
		//output for all of the other players
		} else {
			if (add->point == 21) {
				std::cout << "--- Blackjack! ---\n";
				add->winCount = 'O';
			} else
				if (add->point == 22) add->point = 11;
			std::cout << "Hand: " << add->hand << "\nPoint: " << add->point << "\n";
		}
		add->next = actual;
		std::cout << "--------\n";

		delete(add);
		i++;
	}
	getchar();
	system("clear");
}

void Blackjack::Round() {
	do {
		round_count++;
		Blackjack::RoundStart();
		actual = first;
		// will stop right at dealer's data
		while (actual->next) {
			if (actual->winCount == 'O') { // If already got Blackjack then skip to the next player
				actual = actual->next;
				continue;
			}
			std::cout << "---- Round " << round_count << "----\n";
			std::cout << "---- " << actual->name <<"'s turn----\n"
					  << "Hand: " << actual->hand
					  << "\nPoint: " << actual->point << "\n";
			do {
				std::cout << "------------\n";
				std::cout << "What do you want to do?"
						  << "\n[1] Stand\n[2] Hit";
				std::cin >> tmp;
				tmp_point = actual->point;
				while (tmp != 1 && tmp != 2) {
					std::cout << "Error!"
							  << "\n[1] Stand\n[2] Hit";
					std::cin >> tmp;
				}
				if (tmp==1) {
					std::cout << "Hand: " << actual->hand
							  << "\nPoint: " << actual->point << "\n";
					std::cout << "------------\n";
				} else {
					DealCard(actual->hand);
					actual->point = tmp_point;
					if (actual->point > 21) {
						std::cout << "Busted!\n";
						actual->winCount = 'X';
						std::cout << "Hand: " << actual->hand
								  << "\nPoint: " << actual->point
								  <<"\t(x)\n";
						std::cout << "------------\n";
						break;
					}
					else
						std::cout << "Hand: " << actual->hand
								  << "\nPoint: " << actual->point <<"\n";
				}
			} while(tmp!=1);
			getchar();
			actual = actual->next;
			system("clear");
		}

		Blackjack::Result();
		std::cout << "\n\n"
				  << "Do you want to keep playing?"
				  << "\n[1] Yes\n[2] No\n";
		std::cin >> pick;
		system("clear");
	} while(pick=='1');
}

void Blackjack::Result() {
	/*
	FILE *f;
	f = fopen("record.txt", "a");
	fprintf(f, "%3d", round_count);
	*/
	std::ofstream f;
	f.open("record.txt");
	f << std::setw(3) << round_count;

	tmp_point = dealer->point;
	while (dealer->point < 17) {
		DealCard(dealer->hand);
		dealer->point = tmp_point;
	}
	std::cout << dealer->name << ":\n"
			  << "Hand: " << dealer->hand
			  << "\nPoint: " << dealer->point << "\n\n\n";
	actual = first;
	if (dealer->point>21) {
		while (actual->next) {
			if (actual->winCount != 'X')
				actual->winCount = 'O';
			// fprintf(f, "\t\t%c", actual->winCount);
			f << "\t\t" << actual->winCount;
			actual = actual->next;
		}
	} else {
		while(actual->next){
			if(actual->winCount != 'X')
				if(actual->point > dealer->point)
					actual->winCount = 'O';
				else {
					if(actual->point < dealer->point)
						actual->winCount = 'X';
					else
						actual->winCount = '-';
				}
			// fprintf(f, "\t\t%c", actual->winCount);
			f << "\t\t" << actual->winCount;
			actual = actual->next;
		}
	}
	f << "\n";
	f.close();
	// fprintf(f, "\n");
	// fclose(f);

	actual = first;
	std::cout << "------------\n";
	std::cout << "The following people won:\n\n";
	while (actual->next) {
		if(actual->winCount == 'O')
			std::cout << actual->name
					  << "\nHand: " << actual->hand
					  << "\nPoint: " << actual->point << "\n";
		actual = actual->next;
	}

	actual = first;
	std::cout << "------------\n";
	std::cout << "The following people tied:\n\n";
	while (actual->next) {
		if (actual->winCount == '-')
			std::cout << actual->name
					  << "\nHand: " << actual->hand
					  << "\nPoint: " << actual->point << "\n";
		actual = actual->next;
	}
}

// Open record.txt to read
void Blackjack::LeadingBoard() {
	/*
	FILE *f;
	char line[256];
	f = fopen("record.txt", "r");
	while (fgets(line, sizeof(line), f))
		std::cout << line;
	*/
	std::string line;
	std::ifstream f;
	f.open("record.txt");
	while (std::getline(f, line)) std::cout << line;
	f.close();
	std::cout << "\n";
	getchar();
}

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
			std::cout << line;
		}
		f.close();
	} else {
		std::cout << "Error opening Rules!\n";
	}
	getchar();
}
