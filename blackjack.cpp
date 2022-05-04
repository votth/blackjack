#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <ctime>

#include "blackjack.h"

// card-value pairs
char card[13][10] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
int value[13]     = { 2,   3,   4,   5,   6,   7,   8,   9,   10,   10,     10,      10,     11};
/* for an infinite decks of card, count[i] = 4 then no more of that card
int count[13]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
*/
// Players
char name[5][10] = {"John","Kim","Leo","Kati","DEALER"};
// ???
int i, r, tmp, round_count;
char pick;
int tmp_point = 0;

void Blackjack::recordCreate() {
	FILE *fp;
	fp = fopen("record.txt","w");
	fprintf(fp,"----------------------------------------\n");
	fprintf(fp,"-----         Record table         -----\n");
	fprintf(fp,"----------------------------------------\n");
	fprintf(fp,"\nExplication: O means win, X means lost, - means tied\n\n");
	fprintf(fp,"\nRound\t\tJohn\t\tKim\t\tLeo\t\tKati\n\n");
	fclose(fp);
} ;

void Blackjack::menu() {
	FILE *f; //run through the record file by counting lines
	char line[256];
	f = fopen("record.txt","r");
	if (f == NULL) recordCreate();
	else
		while (fgets(line,sizeof(line),f)) round_count++;
	fclose(f);
	round_count -= 9; //Check if there're already a play record
	if (round_count < 0) round_count = 0;

	Blackjack b;
	dealer = NULL;
	add = NULL;
	while (pick != '4') {
		printf("----------------------------------------\n");
		printf("-----   Blackjack  Simulator  1.2  -----\n");
		printf("----------------------------------------\n");
		printf("Please choose from the below functions:\n");
		//if 1st start out or have reset (chosen New) then don't print out option 0
		if (round_count!= 0)
			printf("	[0]	Continue\n	[1]	New\n	[2]	Record\n	[3]	Rules\n	[4]	Exit\n");
		else
			printf("	[1]	New\n	[2]	Record\n	[3]	Rules\n	[4]	Exit\n");
		scanf("%c",&pick);	fflush(stdin);
		switch (pick) {
			case '0': {
				if( round_count==0 ){
					printf("Error! Please try again!\n");
					system("pause");
					system("cls");
					break;
				}
				system("cls");
				b.round();
				break;
			}
			case '1': {
				system("DELETE record.txt");
				recordCreate();
				round_count = 0;
//				for(tmp=0;tmp<13;tmp++)	count[tmp]=0;	reset the count of cards also, but if it infinite deck then no need for this
				system("cls");
				b.round();
				system("cls");
				break;
			}
			case '2': {
				system("cls");
				b.leadingBoard();
				system("cls");
				break;
			}
			case '3': {
				system("cls");
				b.Rule();
				system("cls");
				break;
			}
			case '4': {
				printf("Thank you for playing!\tGoodbye!!!\n");
				system("pause");
				break;
			}
			default: {
				printf("Error! Please try again!\n");
				system("pause");
				system("cls");
				break;
			}
		}
	}
}

void Blackjack::dealCard(char *s) {
	while (1) {
		r = rand() % 13;
		/*
		count[r]++;
		if(count[r] <= 4)
		these lines are needed if playing with finite number of decks
		*/
		break;
	}
	strcat(s,", "); //just so the output look nicer: King, 8, Ace
	strcat(s,card[r]);
	tmp_point += value[r]; //this will become the player's new point
}

void Blackjack::roundStart() {
	printf("----Round %2d----\n",round_count);
	free(add);
	first = NULL;
	srand(time(0)); //for r's random value
	i = 0;
	while (i<5) {
		add = (struct player*) malloc(sizeof(player));
		//check if there's enough space
		if (!add) {
			printf("Memory error!\n");
			system("pause");
		}
		strcpy(add->name,name[i]);
		printf("Player %s\n",add->name);
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

		//generating random number from 0 to 12
		while (1) {
			r = rand() % 13;
			/*
			count[r]++;
			if(count[r] <= 4)
			*/
			break;
		}
		strcpy(add->hand,card[r]); //adding the card to player's hand
		tmp_point += value[r];

		dealCard(add->hand);
		add->point = tmp_point; //I couldn't find a way to directly change the uj->point's value

		if (!strcmp(add->name,"DEALER")) { //output for Dealer is different to others, need to hide the second card aka to be facedown
			printf("Hand: ");
			tmp = 0;
			while (add->hand[tmp] != ',') {
				printf("%c",add->hand[tmp]);
				tmp++;
			}
			printf(", (unknown)\nPoint: %d\n",add->point-value[r]);
			dealer = add; //save back the dealer point for easier access later on
		//output for all of the other players
		} else {
			if (add->point == 21) {
				printf("--- Blackjack! ---\n");
				add->winCount = 'O';
			} else
				if (add->point == 22) add->point = 11;
			printf("Hand: %s\nPoint: %d\n",add->hand,add->point);
		}
		add->next = actual;
		printf("--------\n");
		i++;
	}
	system("pause");
	system("cls");
}

void Blackjack::round() {
	do {
		round_count++;
		Blackjack::roundStart();
		actual = first;
		while (actual->next) { //stop right at dealer's data
			if (actual->winCount == 'O') { //if already got Blackjack then skip to the next player
				actual = actual->next;
				continue;
			}
			printf("----Round %2d----\n",round_count);
			printf("----%s's turn----\n",actual->name);
			printf("Hand: %s\nPoint: %d\n",actual->hand,actual->point);
			do {
				printf("------------\n");
				printf("What do you want to do? [1] Stand or [2] Hit?	");
				scanf("%d",&tmp);	fflush(stdin);
				tmp_point = actual->point;
				while (tmp != 1 && tmp != 2) {
					printf("Error!\n");
					printf("[1] Stand or [2] Hit?	");
					scanf("%d",&tmp);
					fflush(stdin);
				}
				if (tmp==1) {
					printf("Hand: %s\nPoint: %d\n",actual->hand,actual->point);
					printf("------------\n");
				} else {
					dealCard(actual->hand);
					actual->point = tmp_point;
					if (actual->point > 21) {
						printf("Busted!\n");
						actual->winCount = 'X';
						printf("Hand: %s\nPoint: %d	(x)\n",actual->hand,actual->point);
						printf("------------\n");
						break;
					}
					else
						printf("Hand: %s\nPoint: %d\n",actual->hand,actual->point);
				}
			} while(tmp!=1);
			system("pause");
			actual = actual->next;
			system("cls");
		}

		Blackjack::result();
		printf("\n\nDo you want to keep playing?	[1] Yes\t[2] No\n");
		scanf("%c",&pick);	fflush(stdin);
		system("cls");
	} while(pick=='1');
}

void Blackjack::result() {
	FILE *f;
	f = fopen("record.txt","a");
	fprintf(f,"%3d",round_count);

	tmp_point = dealer->point;
	while (dealer->point < 17) {
		dealCard(dealer->hand);
		dealer->point = tmp_point;
	}
	printf("%s:\nHand: %s\nPoint: %d\n\n\n",dealer->name,dealer->hand,dealer->point);
	actual = first;
	if (dealer->point>21) {
		while (actual->next) {
			if (actual->winCount != 'X')
				actual->winCount = 'O';
			fprintf(f,"\t\t%c",actual->winCount);
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
			fprintf(f,"\t\t%c",actual->winCount);
			actual = actual->next;
		}
	}
	fprintf(f,"\n");
	fclose(f);

	actual = first;
	printf("------------\n");
	printf("The following people won:\n\n");
	while (actual->next) {
		if(actual->winCount == 'O')
			printf("%s\nHand: %s\nPoint: %d\n",actual->name,actual->hand,actual->point);
		actual = actual->next;
	}

	actual = first;
	printf("------------\n");
	printf("The following people tied:\n\n");
	while (actual->next) {
		if (actual->winCount == '-')
			printf("%s\nHand: %s\nPoint: %d\n",actual->name,actual->hand,actual->point);
		actual = actual->next;
	}
}

void Blackjack::leadingBoard() {
	FILE *f;
	char line[256];
	f = fopen("record.txt","r");
	while (fgets(line,sizeof(line),f))
		printf("%s",line);
	printf("\n");
	system("pause");
}

void Blackjack::Rule() {
	FILE *f;
	char s;
	f = fopen("rule.txt","r");
	if (f) {
		while( (s = getc(f)) != EOF )
			putchar(s);
		fclose(f);
	} else
		printf("Error opening Rules!\n");
	system("pause");
}
