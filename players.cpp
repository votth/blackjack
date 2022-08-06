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
/*
Blackjack::Players::~Players() {
	EmptyLink(this);
}
*/
void Blackjack::Players::EmptyLink(Players* head) {
	Players* tmp = head;
	while (head != nullptr) {
		head = head->next;
		delete tmp;
		tmp = head;
	}
}

// Edit methods
void Blackjack::Players::SetName(const std::string& name) {
	this->name = name;
}
void Blackjack::Players::UpdateHand(const std::string& card) {
	this->hand += card;
}
void Blackjack::Players::UpdatePoint(const int& value) {
	this->point += value;
}
/* void Blackjack::Player::UpdateWin(char &c) { */
/* 	this->winCount = &c; */
/* } */
void Blackjack::Players::UpdateNext(Players* next) {
	this->next = next;
}

// Print methods
Blackjack::Players* Blackjack::Players::GetNext() {
	return this->next;
}
void Blackjack::Players::PrintPlayer() {
	std::cout << "Player " << this->name << "\n"
			  << "\tHand: " << this->hand
			  << "\n\tPoint: " << this->point << "\n";
	std::cout << "--------\n";
}


// DEALER
void Blackjack::Dealer::HideHand() {
}
void Blackjack::Dealer::PrintPlayer() {
	std::cout << "This is the DEALER hand: " << this->hand << "\n\n";
}

