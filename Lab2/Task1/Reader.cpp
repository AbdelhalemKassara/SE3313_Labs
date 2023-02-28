#include <iostream>
#include "SharedObject.h"

using namespace std;

struct MyShared{
	int threadNum;
	int reportID;
	int timePassed;	//stores time passed since thread was initialized. I stopped using it. 
	int totalTime;
	int delayT;
};

int main(void)
{
	std::cout << "I am a reader" << std::endl;
	time_t first = time(0);
	Shared<MyShared> sharedMemory("sharedMemory");
	
	int count = 0;
	while (count <60){//the count exists since there isn't a way to break out of the loop
		time_t last = time(0);
		int timer = last-first;
		
		std::cout<<"Thread Number: " <<sharedMemory->threadNum << " | Report Count: " << sharedMemory->reportID << " | Time Elapsed: " << timer << " | Delay: " << sharedMemory->delayT << " | Count: " << count << std::endl;
		
		count++;
		sleep(1);
	}
	
}
