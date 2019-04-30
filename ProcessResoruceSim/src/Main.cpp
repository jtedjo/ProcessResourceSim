#include <iostream>
#include <string>
#include <queue>
using namespace std;

#define MAX_LINE 256
//sum of char for commands
#define DE 201
#define CR 213
#define TO 227
#define REL 323
#define REQ 328
#define INIT 436
#define ENDING 0 
//to run pipeline the file through stdin for input!
//ex: ./a.out < input.txt
#include "Resource.h"
#include "Scheduler.h"
extern Resource resources[4];
int decodeInput(string input);

int main(int argc, char **argv){ 
	
	string line;

	Scheduler sch;
	cout << sch.runningProcess->getName() << " ";
	while(getline(cin, line)){
		int command = decodeInput(line);
		int arg1 = line.find(' ', 0);
		int arg2 = line.find(' ', arg1+1);
		//if error occurs...
		
		switch(command){
			case DE:
				sch.destroyProcess(line.substr(arg1+1));
				break;
			case CR:
				sch.createProcess(line.substr(arg1+1,arg2-3), stoi(line.substr(arg2)));
				break;
			case TO:
				sch.timeOut();
				break;
			case REL:
				sch.releaseResource(atoi(&(line.at(arg1+2))), stoi(line.substr(arg2)));
				break;
			case REQ:
				sch.requestResource(atoi(&(line.at(arg1+2))), stoi(line.substr(arg2)));
				break;
			case INIT:
				cout <<"\n";
				sch.createProcess("init", 0);
				break;
			case ENDING:
				sch.restarts();
				break;
			default:
			//not one of the commands!
				cout <<"..."<<endl;
		}
		
	}
	//clears everything
	sch.restarts();
    return 0;
}

//basically hashes the input
//cr = 213, to = 227 , req = 328 ,de = 201, rel = 323, init = 436
int decodeInput(string input){
	int sum = 0;
	for(int i = 0 ; i < input.length(); i++){
		if(input[i] == ' ' || input[i] == '\n' || input[i] == '\0'){
			break;
		}
		sum +=input[i];
	}
	return sum;
}
