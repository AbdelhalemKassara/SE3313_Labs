#include <iostream>
#include "thread.h"
#include "SharedObject.h"
#include "Blockable.h"
#include <time.h>
#include <stack>
#include <thread>
#include <unistd.h>

struct MyShared{
	int delayTime;
	int timePassed;
	int threadNumber;
	int reportId;
	int totalTime;
};

int main(void)
{
	std::cout << "I am a Writer" << std::endl;
	
	Shared<MyShared> shared("sharedMemory", true); //This is the owner of sharedMamory

	string createThreadResp;
	string delayResp;
	int threadCount = 0;

	while(true){
		std::cout << "Would you like to create a writer thread?" << std::endl;
		std::getline(std::cin, createThreadResp);

		if(createThreadResp == "n") {
			break;
		} else {
			std::cout << "What is the delay time for this thread?" << std::endl;
			std::getline(std::cin, delayResp);

			threadCount++;

			int delay = std::stoi(delayResp);//converts string to int
			//create thread using user input
			thread1 = new WriterThread(threadCount, delay); //add arguments as needed

		}
	}

	//example for one thread thread1
	thread1->flag= true;
	delete thread1;
}

class WriterThread : public Thread{
	public:
		int threadNum;
		bool flag;
		int delay;
		int reportID = 0;

		WriterThread(int in, int delayT):Thread(delayT*1000){
			//this alters the property of memory referenced object with -> notation
			this->threadNum = in; //or whatever you want/need here
			this->delay = delayT;
			
		}

		virtual long ThreadMain(void) override{

			//declare shared memory var so this thread can access it
			Shared<MyShared> sharedMemory ("sharedMemory");
			
			while(true)
			{
				//write to shared memory
				this-> reportID++;//increments the report counter for every cycle
				sleep(delay);
				if(flag){//Exit loop to end the thread
					break;
				}
			}
		}
};

