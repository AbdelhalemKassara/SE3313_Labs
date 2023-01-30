#include <iostream>
#include <unistd.h>

int main() {
	std::string userData;

	pid_t id;
	while (userData != "Done") {
		std::cout << "Please enter some text." << std::endl;

		std::cin >> userData;
		std::cout << "'" << userData << "' was entered." << std::endl;

		id = fork();
		if (id == 0) {//breaks out of the loop for the child process
			break;
		}
	}
	
	//for the child processes
	while(id == 0) {
		std::cout << "this is a child: "<< userData << std::endl;
		sleep(1);//makes the child process sleep for 1 second
	}
	

	return 0;
}