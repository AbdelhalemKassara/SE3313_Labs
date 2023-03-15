#include <iostream>
#include "SharedObject.h"

struct MyShared
{
	int threadId;
	int reportNum;
	int delayT;
};

int main(void)
{
	std::cout << "I am a reader" << std::endl;
	std::cout << "Thread Number | Report Id | Time Elapsed | Delay" << std::endl;

	Shared<MyShared> sharedMem("origin");
	time_t startTime = time(0);

	// loops for 30 seconds
	for (int i = 0; i < 30; i++)
	{
		int timer = time(0) - startTime;
		std::cout << sharedMem->threadId << " | " << sharedMem->reportNum << " | " << timer << " | " << sharedMem->delayT << std::endl;
		sleep(1);
	}
}
