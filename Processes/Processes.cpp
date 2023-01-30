#include <iostream>
#include <unistd.h>
#include <vector>

int main()
{
	std::string userData;

	pid_t id;
	while (userData != "Done")
	{
		std::cout << "Please enter some text." << std::endl;

		std::cin >> userData;
		std::cout << "'" << userData << "' was entered." << std::endl;

		id = fork();
		if (id == 0)
		{
			break;
		}
	}

	while (id == 0)
	{
	}
	return 0;
}
