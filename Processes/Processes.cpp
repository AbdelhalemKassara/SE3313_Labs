#include <iostream>
#include <unistd.h>
#include <vector>

void print(std::vector<int> v);

int main() {
	std::string userData;
	std::vector<int> processes;
	int i = 0;

	while(userData != "Done") {
		std::cout << "Please enter some text." << std::endl;

		std::cin >> userData;
		std::cout << "'" << userData << "' was entered." << std::endl;
			
		processes.push_back(i);
		i++;

		print(processes);
	}	

	return 0;
}

void print(std::vector<int> v) {
	std::string output = "";
	for(int d = 0; d < v.size(); d++) {
		output += ", " + std::to_string(v.at(d));
	}

	std::cout << output << std::endl;
}
