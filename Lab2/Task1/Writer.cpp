#include <iostream>
#include "thread.h"
#include "SharedObject.h"
#include "Blockable.h"
#include <time.h>
#include <stack>
#include <thread>
#include <unistd.h>


struct MyShared{
	int threadNum;
	int reportID;
	int timePassed;
	int totalTime;
	int delayT;
};

class WriterThread : public Thread{
	public:
		int threadNumber;
		bool flag = false;	//false so that the loop doesn't break. 
		int delay;
		int reportID = 0;
		//int totalTime;
		
		WriterThread(int in, int delayTime):Thread(delayTime*1000){
			this->threadNumber = in; 
			this->delay = delayTime;
			
			//alters property of a memory referenced object with -> notation
			
		}

		virtual long ThreadMain(void) override{
			
			//declare shared memory var so this thread can access it
			Shared<MyShared> sharedMemory ("sharedMemory");
			time_t firstTime = time(0);
			
			while(true)
			{
				this->reportID++; //increment report counter every cycle.
				
				sleep(delay); //sleep the thread for the specified delay
				time_t lastTime = time(0);

				int timeElapsed = lastTime - firstTime;
				sharedMemory-> threadNum = threadNumber; //set the shared memory location to the thread ID we are currently on
				sharedMemory-> timePassed = timeElapsed;
				sharedMemory-> reportID = reportID;		//update all the stuff for our shared memory location. 
				sharedMemory->totalTime = timeElapsed*reportID;
				sharedMemory->delayT = delay;
				if(flag){//Exit loop to end the thread
					break;
				}
			}
		return 1;
	}
};



int main(void)
{
	std::cout << "I am a Writer\n";
	
	Shared<MyShared> shared("sharedMemory", true); //This is the owner of sharedMamory
	
	int threadNumber = 0;
	std::string delayTime;
	std::string userInput;
	
	WriterThread * thread1; //we use this reference to create new threads in the while statement. 
	std::stack<WriterThread*> threadStack; 	//an empty stack. We will store the threads in here. 


	while(true){
		std::cout << "Do you want to create a new thread? (y/n)\n";
		//create thread using user input
		std::getline(std::cin, userInput);
		
		if (userInput == "n"){
			break;	
		}
		else{
			std::cout <<"What delay do u want on this thread? \n";
			threadNumber++;
			std::getline(std::cin, delayTime);
			int delayInt = std::stoi(delayTime);
			thread1 = new WriterThread(threadNumber, delayInt); //create a new writer thread with the thread number and the delay specified
			threadStack.push(thread1);
			
		}
	}
	//if we are no longer in the loop then we are done collecting and creating threads. Time to delete them. 
	for(auto i = 0; i < threadStack.size(); i++){
		//now we need to stop the thread. So we have to set the flag to true.
		thread1 = threadStack.top();
		thread1->flag = true;
		delete thread1;	//delete the thread! 
		threadStack.pop();	//pop the top thread. 
	}
}






