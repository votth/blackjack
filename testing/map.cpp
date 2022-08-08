#include <iostream>
#include <map>

int main() {
	const std::map<std::string, int> myMap = {
		{"2", 2}, {"3", 3}, {"4", 4},
		{"5", 5}, {"6", 6}, {"7", 7}
	};

	std::string key {};
	int value {};

	for (auto& i: myMap) {
		std::cout << i << '\n';
		std::cout << i.first << " " << i.second << "\n\n";
	}

	return 0;
}
