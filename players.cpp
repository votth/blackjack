#include "blackjack.h"

// Player
// The Big Three
// Constructors
Blackjack::Players::Players() {
	this->name = "";
	this->hand = "";
	this->point = 0;
	this->winCount = 'X';
	this->next = nullptr;
}
Blackjack::Players::Players(const std::string& name) {
	this->name = name;
	this->hand = "";
	this->point = 0;
	this->winCount = 'X';
	this->next = nullptr;
}
// Copy ctor
// Destructor
void Blackjack::Players::EmptyLink(Players* head) {
	Players* tmp = head;
	while (head != nullptr) {
		head = head->next;
		delete tmp;
		tmp = head;
	}
}

// Setters
void Blackjack::Players::SetName(const std::string& name) {
	this->name = name;
}
void Blackjack::Players::UpdateHand(const std::string& card) {
	this->hand += card;
}
void Blackjack::Players::UpdatePoint(const int& value) {
	this->point += value;
}
void Blackjack::Players::UpdateWin(const char& c) {
	this->winCount = c;
}
void Blackjack::Players::UpdateNext(Players* next) {
	this->next = next;
}

// Getters
std::string Blackjack::Players::GetName() {
	return this->name;
}
int Blackjack::Players::GetPoint() {
	return this->point;
}
Blackjack::Players* Blackjack::Players::GetNext() {
	return this->next;
}

// Print
void Blackjack::Players::PrintPlayer() {
	std::cout << "--- Player " << this->name << " ---\n"
			  << "\tHand:  " << this->hand
			  << "\n\tPoint: " << this->point << "\n\n";
}


// DEALER
void Blackjack::Dealer::PrintPlayer() {
	std::cout << "||    The DEALER\n"
			  << "||\tHand:  " << this->hand
			  << "\n||\tPoint: " << this->point << "\n\n";
}

// Flip 2nd card to (unknown)
void Blackjack::Dealer::HideHand() {
	const std::string delimeter = " ";

	// Hide card
	// find start and end index of the 2nd word/card
	int start = this->hand.find(delimeter) + 1;
	int end = this->hand.find(delimeter, start);
	// extract word
	std::string extracted = this->hand.substr(start, end - start);
	this->hiddenCard = extracted;
	// replace word
	this->hand.erase(start, end - start);
	this->hand.insert(start, "(unknown)");

	// Hide value
	for (int i = 0; i < 13; ++i) {
		if (extracted == cards[i]) {
			this->hiddenPoint = values[i];
			this->point -= values[i];
			break;
		}
	}
}

