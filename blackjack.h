#ifndef BLACKJACK_H
#define BLACKJACK_H

class Blackjack {
	private:
		struct player{
			char name[10];
			char hand[50];
			int point;
			char winCount; //X if busted	and later used for a record bin file
			struct player *next;
		};
		player *actual, *before, *add, *dealer; //these are for the memory chain's build
		player *first;

	public:
		// menua builder
		void menu();
		void recordCreate();
		void leadingBoard();
		void Rule();
		// game methods
		void roundStart(); //give out 2 cards for each person
		void round(); //player takes turn one after another
		void dealCard(char *);
		void result();
};

#endif
