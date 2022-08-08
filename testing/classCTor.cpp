#include <iostream>

class player {
		std::string name{};
		std::string hand{};
		player* next{};

	public:
		player(const std::string& name, const std::string& hand) {
			this->name = name;
			this->hand = hand;
			this->next = nullptr;
		}
		// copy constructor
		~player() {
			std::cout << this->name << " is being free'd\n";
			if (this->next != nullptr) {
				delete this->next;
			}
		}

		void updateNext(player *next) {
			this->next = next;
		}
};

void OutOfScopeDeath() {
	player *Two = new player("Deddo", "1 1");
	delete Two;
}

int main() {
	player *One = new player("Henry", "1 3 5");
	player *nextPlayer = new player("Peter", "0 0");

	One->updateNext(nextPlayer);
	delete One;
	std::cout << "Empty link One ended.\n";

	 OutOfScopeDeath();

	return 0;
}
