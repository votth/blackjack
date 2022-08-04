#ifndef BLACKJACK_H
#define BLACKJACK_H

class Blackjack {
	private:
		struct player{
			char name[10];
			char hand[50];
			int point;
			char winCount;	// and used for a record bin file
			struct player *next;
		};
		// These are for the memory chain's build
		// player *actual{}, *before{}, *add{}, *dealer{};
		// player *first{};

	public:
		// menu builder
		void Menu();
		void RecordCreate();
		void LeadingBoard();
		void Rule();
		// game methods
		void RoundStart();	//give out 2 cards for each person
		void Round();		//player takes turn one after another
		void DealCard(char *);
		void Result();
};

#endif
