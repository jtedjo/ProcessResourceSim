#include "Resource.h"

string states[4] = {"ready", "running", "blocked", "none"};

	//Default constructor
	Process:: Process(){
		pid = (string)"default";
		status_type = &states[READY];
		priority = 0;
		neededRequest = 0;
		for(int i = 0 ; i < NUM_RESOURCES; i++){
			resourcesHeld[i] = 0;
		}
	}
	Process:: Process(string name, int pr){
		pid = name;
		priority = pr;
		status_type = &states[READY];
		neededRequest = 0;
		for(int i = 0 ; i < NUM_RESOURCES; i++){
			resourcesHeld[i] = 0;
		}
	}
	Process:: Process(const Process& toCopy){
		pid = toCopy.pid;
		priority = toCopy.priority;
		other_resources = toCopy.other_resources;
		status_type = toCopy.status_type;
		neededRequest = toCopy.neededRequest;
		for(int i = 0 ; i < NUM_RESOURCES; i++){
			resourcesHeld[i] = toCopy.neededRequest;
		}
	}
	
	//less than is used by priority queue
	bool Process:: operator<(const Process& b) const{
		return this->getPriority() < b.getPriority();
	}
	Process& Process:: operator=(const Process& assignProcess) {
		if(&assignProcess != this){
			pid = assignProcess.pid;
			priority = assignProcess.priority;
			other_resources = assignProcess.other_resources;
		}
	 return *this;
    }
	int Process::getResourceHeld(int rNum){
		//asking for a resource that is greater than existing resources
		if(rNum > NUM_RESOURCES){
			cout << "ERROR, RESOURCE NUM INVALID" << endl;
		}
		else{
			return resourcesHeld[rNum-1];
		}
		return 0;
	}
	int Process:: getNeed() const{
		return neededRequest;
	}
	void Process:: setNeed(int needed){
		neededRequest = needed;
	}
	//-1 denotes blocking, 0 is other error, 1 is completed
	int Process:: request(Resource * r, int unit){
		//incase you are asking for more than available
		int max = r->resourceMax();
		if(max < unit){
			return 0;
		}
		//blocking algorithm: if not enough resource left or something ahead
		if(unit > r->resourceLeft() || !r->blocked_list.empty()){
			//check if it already holding some, and the request exceed max
			if((resourcesHeld[r->getRID()-1] + unit) > max){
				return 0;
			}
			status_type = &states[BLOCKED];
			setStatusList(&(r->blocked_list));
			neededRequest = unit;
			r->blocked_list.push_back(this);
			return -1;
			//block algorithm here
		}
		//holds how many units from a particular resource is held.
		resourcesHeld[r->getRID()-1] = unit;
		for(int i = 0; i < unit; i++){
			other_resources.push_back(r->requestUnit());
		}
		setNeed(0); 
		return 1;
	}
	Process** Process:: release(Resource * r, int numToRelease, int & numUnblocked){
		int toRelease = r->getRID();
		int numResources = numToRelease;
		auto it = other_resources.begin();
		int blocked = r->blocked_list.size();
		while(it != other_resources.end() && numResources > 0){
			if((*it)->unitResourceID() == toRelease){
				Unit* pointer = *it;
				other_resources.erase(it++);
				r->released(pointer);
				numResources--;
			}
			else{
				++it;
			}
		}
		//not as many resources released as expected...
		if(numResources > 0){
			cout << "ERROR: REQUESTING RELEASE GREATER THAN HELD!\"n";
		}
		
		Process ** returnPointers = r->checkBlocked();
		numUnblocked = blocked - r->blocked_list.size();
		return returnPointers;
	}
	//release all resources if the process is terminated
	void Process:: releaseAll(){
		while(!other_resources.empty()){
			Unit * releasing = other_resources.front();
			resources[releasing->unitResourceID()-1].released(releasing);
			other_resources.pop_front();
		}
	}
	void Process:: setResource(int RID, Unit * toAdd){
		other_resources.push_back(toAdd);
		resourcesHeld[RID-1]++;
	}
	string Process:: getName() const{
		return pid;
	}
	bool Process:: isBlocked(){
		return (status_type == &states[BLOCKED]);
	}
	string Process:: getStatus() const{
		return *status_type;
	}
	void Process:: setStatus(int newStatus){
		status_type = &states[newStatus];
	}
	void Process:: setParent(Process* newParent){
		parent = newParent;
	}
	void Process:: setChild(Process* newChild){
		child.push_back(newChild);
	}
	void Process:: setStatusList(vector<Process *>* vectorAddress){
		status_list = vectorAddress;
	}
	void Process:: printResources(){
		for(Unit * resourceUnits : other_resources){
			resourceUnits->printUnitInfo();
		}
	}
	void Process::printInformation(){
		for (auto a: child){
			cout << a->getName() << " ";
		}
		cout << endl;
	}
	int Process:: getPriority() const{
		return priority;
	}
	bool Process:: checkIfChild(Process * callingProcess){
		Process * toDelete = this;
		//deleting itself
		if(toDelete == callingProcess){
			return true;
		}
		while(toDelete->parent != NULL){
			if(toDelete->parent == callingProcess){
				return true;
			}
			toDelete = toDelete->parent;//continue going up til null
		}
		return false;
	}





int RID = 1;//starting RID	
	Unit:: Unit(){
		unitNumber =0;
	}
	Unit:: Unit(int unitNum, int resID){
		unitNumber = unitNum;
		unitRID = resID;
	}
	int Unit:: getNumber(){
		return unitNumber;
	}
	void Unit:: printUnitInfo(){
		cout <<" this is unit : " << unitNumber << endl;
	}
	int Unit:: unitResourceID(){
		return unitRID;
	}
	
	//default constructor
	Resource:: Resource(){
		resourceMaximum = 0;
		resourceName = RID;
		RID++;
	}
	
	//constructor for Resource
	Resource:: Resource(int amount){
		resourceName = RID;
		RID++;
		for(int i = 0 ; i < amount ; i++){
			Unit * unit = new Unit(i+1, this->getRID());
			resourceList.push_back(unit);
		}
		resourceMaximum = amount;
	}
	Resource:: ~Resource(){
		while(!resourceList.empty()){
			Unit * toDelete = resourceList.back();
			resourceList.pop_back();
			delete((Unit *)toDelete);
		}
	}
	int Resource:: resourceLeft(){
		return resourceList.size();
	}
	int Resource:: resourceMax(){
		return resourceMaximum;
	}
	int Resource:: getRID(){
		return resourceName;
	}
	Unit* Resource:: requestUnit(){
		Unit * returnPointer = resourceList.front();
		resourceList.pop_front();
		
		return returnPointer;
	}
	void Resource:: released(Unit * release){
		resourceList.push_back(release);
		
	}
	Process ** Resource:: checkBlocked(){
		if(!blocked_list.empty()){ //if there's resources waiting...
			//potentially all of the blockedlist would be free'd
			Process ** returnPointers = new Process*[blocked_list.size()];
			int i = 0;
			int total = resourceList.size();
			while(!blocked_list.empty()){
				if((total -= blocked_list.front()->getNeed()) >= 0){  
				returnPointers[i] = blocked_list.front();
				blocked_list.erase(blocked_list.begin());
				i++;
				}
				else{
					break;
				}
			}
			return returnPointers;
		}
		else{
			return NULL;
		}
	}
	//how to set status list back to ready_list?
	void Resource:: unblock(){
		if(!blocked_list.empty()){ //if there's resources waiting...
			//potentially all of the blockedlist would be free'd
			int i = 0;
			int total = resourceList.size();
			Process* p = blocked_list.front();
			while(!blocked_list.empty() && (total -= blocked_list.front()->getNeed()) >= 0){
				blocked_list.erase(blocked_list.begin());
				p->setStatus(READY);
				for(int i = 0 ; i < p->getNeed(); i++){
					p->setResource(getRID(),requestUnit());
				}
				p->setNeed(0);
				process_vector.push_back(p);
				p = blocked_list.front();
			}
		}
	}
	void Resource::printList(){
		cout << "resource List size " <<resourceList.size()<< endl;
		for(auto unit : resourceList){
			cout << "Belongs to RID : " << unit->unitResourceID() << endl;
			cout << "Unit Number " <<unit->getNumber() << endl;
			
		}
	}
	void Resource::addBlocked(Process * blocked){
		blocked_list.push_back(blocked);
	}
	void Resource::printBlockedList(){
		for(int i =0; i < blocked_list.size(); i++){
			cout << blocked_list[i]->getName() << " ";
		}
		cout << endl;
	}

