#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <fstream>

int main() {
	std::string userData;
	std::vector<pid_t> processes;
	pid_t id;

	while (true) {
		std::cout << "Please enter some text. parent: " << getpid() << std::endl;

		std::getline(std::cin, userData);//takes in the whole string including spaces
		std::cout << "'" << userData << "' was entered." << std::endl;

		if(userData == "Done" || userData == "done") {//breaks out of the loop when 'Done' is entered just before creating a new process.
			break;
		}

		id = fork();//returns 0 for the child process, and returns the child process id to the parent process
		if (id == 0) {//breaks out of the loop for the child process
			break;
		} else {
			processes.push_back(id);
		}
	}

	//for the child processes
	if(id == 0) {
		std::ofstream MyFile(std::to_string(getpid()) + ".txt");

		while(true) {
			MyFile << userData << std::endl;
			sleep(1);//makes the child process sleep for 1 second
		}

	}	else {	//for the parent process
		for(int i = 0; i < processes.size(); i++) {
				kill(processes.at(i), SIGKILL);//kills all child processes as well as the parent
				std::cout << "kill : " << processes.at(i) << std::endl;
			}
	}

	return 0;
}