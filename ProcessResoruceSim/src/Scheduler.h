#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include <queue>

#include "Resource.h"
using namespace std;
extern Resource resources[4];

class Scheduler{
	private:
		
	public:
		Process * runningProcess;
		Scheduler();
		void scheduler();
		void createProcess(string name, int priority);
		void destroyProcess(string pid);
		void killTree(Process * toDestroy);
		void printProcesses();
		void timeOut();
		void restarts();
		void scanBlockedList();
		void requestResource(int r, int numberUnits);
		void releaseResource(int r, int numberUnits);
		
	
};


#endif
