#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <list>
#include <queue>
#include <algorithm>
using namespace std;
#define NUM_RESOURCES 4
#define READY 0
#define RUNNING 1
#define BLOCKED 2
#define NONE 3

class Unit;//forward declaration
class Resource;//forward declaration
extern Resource resources[4];
class Process{
	//process control block (PCB)
	private:
		string pid;
		list <Unit *> other_resources;
		int resourcesHeld[NUM_RESOURCES];
		string * status_type;
		int neededRequest;
		
		int priority = 0; //0,1,2 (Init, User, System)
	public:
		vector<Process *> * status_list;
		Process* parent = NULL;
		list <Process*> child;
		bool operator<(const Process&b) const;
		Process& operator=(const Process& assignProcess);
		Process();
		Process(string name, int pr);
		Process(const Process& toCopy);
		int request(Resource * r, int unit);
		string getName() const;
		string getStatus() const;
		int getResourceHeld(int rNum);
		int getNeed() const;
		bool isBlocked();
		void setResource(int resourceID,Unit * toAdd);
		void setNeed(int needed);
		void setStatus(int status);
		void setStatusList(vector<Process *>* vectorAddress);
		void setParent(Process * newParent);
		void setChild(Process * newChild);
		void printResources();
		void printInformation();
		Process** release(Resource * r,int numToRelease, int & numUnblocked);
		void releaseAll();
		int getPriority() const;
		bool checkIfChild(Process* callingProcess);
		
};
extern vector<Process *> process_vector; 
extern vector<Process *> all_process; 
//units for resources
class Unit{
	private:
		int unitNumber = 0;
		int unitRID = 0;
		
		
	public:
		Unit();
		Unit(int number, int resourceID);
		int getNumber();
		void printUnitInfo();
		int unitResourceID();
		
};

class Resource{
	//Resource Control Block
	private:
		list<Unit *> resourceList;
		int resourceMaximum;
		int resourceName = 0;
		
	public:
		vector<Process *> blocked_list;
		Resource();
		~Resource();
		Resource(int amount);
		int resourceLeft();
		int resourceMax();
		void printList();
		Unit * requestUnit();
		int getRID();
		void released(Unit * release);
		Process** checkBlocked();
		void unblock();
		void addBlocked(Process * blocked);
		void printBlockedList();
};


#endif
