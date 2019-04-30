#include "Scheduler.h"
#include <algorithm>
Resource resources[4] = {{1},{2},{3},{4}};
extern string states[4];

vector<Process *> process_vector; //ready_list
vector<Process *> all_process;
struct compareProcess{
	bool const operator()(Process * p1, Process *p2)const{
		return p1->getPriority() > p2->getPriority();
	}
};


Scheduler :: Scheduler(){
	Process* init = new Process("init", 0);
	process_vector.push_back(init);
	all_process.push_back(init);
	runningProcess = init;
}
void Scheduler:: scheduler(){
	sort(process_vector.begin(), process_vector.end(), compareProcess());
	for(int i = 0 ; i < process_vector.size(); i++){
		if(!process_vector[i]->isBlocked()){				
			runningProcess = process_vector[i];
			break;
		}
	}
}
void Scheduler:: createProcess(string name, int priority){
	//have to have INIT before doing anything else!
	if(runningProcess == NULL && name.compare("init") != 0){
		cout << "error ";
		return;
	}
	//checking if it already exists
	for(auto process: all_process){
		//can't create process using same name!
		if(process->getName().compare(name) == 0){
			cout << "error ";
			return;
		}
	}

	Process* p = new Process(name, priority);
	if(name.compare("init") != 0){
		runningProcess->setChild(p);
		p->setParent(runningProcess);
	}
	p->setStatusList(&process_vector);
	process_vector.push_back(p);
	all_process.push_back(p);
	scheduler();
	cout << runningProcess->getName() << " ";

}

void Scheduler:: printProcesses(){
	for(int i = 0 ; i < process_vector.size(); i++){
		cout << process_vector[i]->getName() << "------>";
	}
	cout << endl;
}
void Scheduler:: destroyProcess(string pid){
	//no running process
	if(runningProcess == NULL){
		cout << "error ";
		return;
	}
	//can't run destroy command on init
	if(pid.compare("init")== 0){
		cout << "error ";
		return;
	}
	int toDelete = 0;
	for(int i = 0; i < all_process.size(); i++){
		if(all_process[i]->getName() == pid){ //pointers comp
			toDelete = i;
			break;
		}
	}
	Process * deleting = all_process[toDelete];
	
	//if not child...
	if(!deleting->checkIfChild(runningProcess)){
		cout << "error ";
		return;
	}
	
	deleting->parent->child.remove(deleting);
	killTree(all_process[toDelete]);
	scanBlockedList();
	scheduler();

	cout << runningProcess->getName() << " ";
}
void Scheduler:: scanBlockedList(){
	for(int i = 0; i < NUM_RESOURCES; i++){
		resources[i].unblock();
	}
}
void Scheduler:: killTree(Process * toDestroy){
	//keep going down to the most leaf child
	if(!toDestroy->child.empty()){
	Process * p = toDestroy->child.front();
	}
	while(!(toDestroy->child.empty())){
		Process * destroy = toDestroy->child.front();
		killTree(destroy);// delete child one by one	
		toDestroy->child.pop_front();
	}
	int toDelete = 0;
	

	//if it isn't blocked, then you need to remove from RL
	if(!(toDestroy->isBlocked())){
		for(int i = 0 ;  i < process_vector.size(); i++){
			if(process_vector[i] == toDestroy){
				process_vector.erase(process_vector.begin()+i);
				break;
			}
		}
	}
	//if blocked...
	else{
		vector<Process *> *blockedP = toDestroy->status_list;
		int deleting = 0;
		for(int i = 0 ; i < blockedP->size(); i++){
			if ((*blockedP)[i] == toDestroy){
				deleting = i;
				break;
				
			}
		}
		blockedP->erase(blockedP->begin() + deleting);;
	}
	for(int i = 0; i < all_process.size(); i++){
		if(all_process[i] == toDestroy){ //pointers comp
			all_process[i]->setStatus(NONE);
			all_process[i]->releaseAll();
			toDelete = i;
			break;
		}
	}
	
	delete(toDestroy);
	all_process.erase(all_process.begin()+toDelete);
}
void Scheduler:: timeOut(){
	if(runningProcess == NULL){
		cout << "error ";
		return;
	}
	Process * p = runningProcess;
	p->setStatus(READY);
	process_vector.erase(remove(process_vector.begin(),
	process_vector.end(), p), process_vector.end());

	process_vector.push_back(p);
	scheduler();
	cout << runningProcess->getName() << " ";
}
void Scheduler:: restarts(){
	for (int i = 0; i < all_process.size();i++){
		all_process[i]->releaseAll();
		delete(all_process[i]);
	}
	all_process.clear();
	process_vector.clear();
	runningProcess = NULL;
}
void Scheduler:: requestResource(int r, int numberUnits){
	if(runningProcess == NULL || 
	runningProcess->getName().compare("init") == 0){
		cout << "error ";
		return;
	}
	Resource * requestedR = &(resources[r-1]);
	int status = 0;
	status = runningProcess->request(requestedR, numberUnits);
	//if it is being blocked...
	if(status == -1){
		process_vector.erase(process_vector.begin());
	}
	else if(status == 0){
		cout << "error ";
		return;
	}
	scheduler();
	cout << runningProcess->getName() << " ";
	
}
void Scheduler:: releaseResource(int r, int numberUnits){
	if(runningProcess == NULL || 
	runningProcess->getName().compare("init") == 0){
		cout << "error ";
		return;
	}
	if(runningProcess->getResourceHeld(r) == 0){
		cout << "error";
		return;
	}
	int numUnblocked = 0;
	Process ** releasedProcess;
	releasedProcess = runningProcess->release(&resources[r-1], 
		numberUnits, numUnblocked);
	for(int i = 0 ; i < numUnblocked; i++){
		int need = releasedProcess[i]->getNeed();
		releasedProcess[i]->setStatus(READY);
		releasedProcess[i]->request(&resources[r-1], need);
		process_vector.push_back(releasedProcess[i]);
	}
	delete[] (releasedProcess);
	scheduler();
	cout << runningProcess->getName() << " ";
}

