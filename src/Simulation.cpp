#include "../include/Simulation.h"
#include "../include/Action.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
using std::string;
using std::vector;
using namespace std;
extern Simulation* backup;

//Simulation constructor - initiating the simulation with configFile--------------------------
Simulation::Simulation(const string &configFilePath):
                            isRunning(false),
                            planCounter(0),
                            actionsLog(),
                            plans(),
                            settlements(),
                            facilitiesOptions(){
    ifstream configFile(configFilePath);  // Open the config file for reading
    
    if (!configFile.is_open()) {
        std::cerr << "Unable to open configuration file: " << configFilePath << std::endl;
        throw std::runtime_error("Failed to open configuration file.");
    }

    string line;

    while (getline(configFile, line)){

        vector<string> arguments = Auxiliary::parseArguments(line);
        
        // Skip empty lines or instructions
        if (arguments.empty() || arguments[0] == "#") continue;  
        
        else if (arguments[0] == "settlement") {
            SettlementType type;
            if(arguments[2] == "0")
                type = SettlementType::VILLAGE;
            else if(arguments[2] == "1")
                type = SettlementType::CITY;    
            else if(arguments[2] == "2")
                type = SettlementType::METROPOLIS;
            Settlement* set = new Settlement(arguments[1], type);
            addSettlement(set);
        }
        
        else if (arguments[0] == "facility"){
            FacilityCategory category;
            if(arguments[2] == "0")
                category = FacilityCategory::LIFE_QUALITY;
            else if(arguments[2] == "1")
                category = FacilityCategory::ECONOMY;
            else if(arguments[2] == "2")
                category = FacilityCategory::ENVIRONMENT;
            FacilityType fac = FacilityType(arguments[1], category, stoi(arguments[3]), stoi(arguments[4]), stoi(arguments[5]), stoi(arguments[6]));
            addFacility(fac);
        }
        
        else if (arguments[0] == "plan") {
            SelectionPolicy* pol = SelectionPolicy::strToPolicy(arguments[2]);
            addPlan(getSettlement(arguments[1]), pol);
        }
    }
    configFile.close();  // Close the config file after processing
    start();
}
//-----------------------------------------------------------------
// Destructor
Simulation::~Simulation(){
    for(BaseAction* action:actionsLog){
        if(action)
            delete action;
    }
    for(Settlement* set:settlements){
        if(set)
            delete set;
    }
    plans.clear();
    facilitiesOptions.clear();
}
// Copy constructor
Simulation::Simulation(const Simulation& other): 
                            isRunning(other.isRunning),
                            planCounter(other.planCounter),
                            actionsLog(),
                            plans(),
                            settlements(),
                            facilitiesOptions(){
    for(const BaseAction* action:other.actionsLog){
        actionsLog.push_back(action->clone()); }
    for(const Settlement* set:other.settlements){
        settlements.push_back(new Settlement(*set)); }
    for(const FacilityType& fac: other.facilitiesOptions){
        facilitiesOptions.emplace_back(FacilityType(fac)); //calls FacilityType's copy constructor
        }

    for(const Plan& otherPlan : other.plans){
        int thisID = otherPlan.getPlanId();
        Settlement *thisSet = &(getSettlement(otherPlan.getSettlementName()));
        //check if we need to delete the pointer we received 
        SelectionPolicy* thispol = (otherPlan.getSelectionPolicy()->clone());
        plans.push_back(Plan(thisID, *thisSet, thispol, facilitiesOptions));
    }
} 
// Move constructor
Simulation::Simulation(Simulation&& other) noexcept: 
                    isRunning(other.isRunning),
                    planCounter(other.planCounter),
                    actionsLog(move(other.actionsLog)),
                    plans(move(other.plans)),
                    settlements(move(other.settlements)),
                    facilitiesOptions(move(other.facilitiesOptions)) {
    other.isRunning = false;
    other.planCounter = 0;
}
// Copy assignment operator
Simulation& Simulation::operator=(const Simulation& other){
    if(this == &other){
        return *this;
    }
    // Clean up existing resources
    for (BaseAction* action : actionsLog) {
        delete action; }
    actionsLog.clear();
    for (Settlement* set : settlements) {
        delete set; }
    settlements.clear();
    
    facilitiesOptions.clear();
    plans.clear(); 

    // Copy primitive members
    isRunning = other.isRunning;
    planCounter = other.planCounter;

    for (const BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone()); }
    for (const Settlement* set : other.settlements) {
        settlements.push_back(new Settlement(*set)); }
    for(const FacilityType& fac: other.facilitiesOptions){
        facilitiesOptions.emplace_back(fac); //calls FacilityType's copy constructor
    }

    for (const Plan& otherPlan : other.plans) {
        int thisID = otherPlan.getPlanId();
        Settlement* thisSet = &(getSettlement(otherPlan.getSettlementName()));
        SelectionPolicy* thisPol = otherPlan.getSelectionPolicy()->clone();
        plans.push_back(Plan(thisID, *thisSet, thisPol, facilitiesOptions));
    }
    return *this;
}

// Move assignment operator
Simulation& Simulation::operator=(Simulation&& other) noexcept{
    if (this == &other) {
        return *this; }

    // Clean up existing resources
    for (BaseAction* action : actionsLog) {
        delete action; }
    for (Settlement* set : settlements) {
        delete set; }

    plans.clear();

    // Transfer ownership of resources from `other`
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    actionsLog = move(other.actionsLog);
    plans = move(other.plans);
    settlements = move(other.settlements);
    facilitiesOptions = move(other.facilitiesOptions);

    other.isRunning = false;
    other.planCounter = 0;
    //all vectors in 'other' are cleared by the move function 

    return *this;
}

void Simulation::start(){

    cout<<"The simulation has started"<<endl;
    open();
    int enteredClose = 0; //TODO delete later
    //wait for user to enter actions
    while(this->isRunning){
        cout << "enter next command" << endl;
        string text = "";
        getline(cin, text);

        vector<string> input = Auxiliary::parseArguments(text);

        if (input.empty())
            continue;
        else if(input[0] == "step"){
            SimulateStep* step = new SimulateStep(stoi(input[1]));
            step->act(*this); 
        }
        else if(input[0] == "plan"){
            AddPlan* plan = new AddPlan(input[1], input[2]);
            plan->act(*this);
        }
        else if(input[0] == "settlement"){
            SettlementType type;
            if(input[2] == "0")
                type = SettlementType::VILLAGE;
            else if(input[2] == "1")
                type = SettlementType::CITY;    
            else if(input[2] == "2")
                type = SettlementType::METROPOLIS;
            AddSettlement* set = new AddSettlement(input[1], type);
            set->act(*this);
        }
        else if(input[0] == "facility"){
            FacilityCategory category;
            if(input[2] == "0")
                category = FacilityCategory::LIFE_QUALITY;
            else if(input[2] == "1")
                category = FacilityCategory::ECONOMY;
            else if(input[2] == "2")
                category = FacilityCategory::ENVIRONMENT;
            AddFacility* fac = new AddFacility(input[1], category, stoi(input[3]), stoi(input[4]), stoi(input[5]), stoi(input[6]));
            fac->act(*this);
        }
        else if(input[0] == "planStatus"){
            PrintPlanStatus* stat = new PrintPlanStatus(stoi(input[1]));
            stat->act(*this);
        }
        else if(input[0] == "changePolicy"){
            ChangePlanPolicy* pol = new ChangePlanPolicy(stoi(input[1]), input[2]);
            pol->act(*this);
        }
        else if(input[0] == "log"){
            PrintActionsLog* log = new PrintActionsLog();
            log->act(*this);
        }
        else if(input[0] == "close" || input[0] == "Close"){
            close();
            enteredClose++; //TODO delete
            Close* closeA = new Close();
            closeA->act(*this);
            cout << "you asked to close " + to_string(enteredClose) + "times" << endl; //TODO delete cout
        }
        else if(input[0] == "backup"){
            BackupSimulation* backup = new BackupSimulation();
            backup->act(*this); 
        }
        else if(input[0] == "restore"){
            RestoreSimulation* restore = new RestoreSimulation();
            restore->act(*this);
        }
        else
            cout << "unkown command" << endl;
    }
};

//add------------------------------------------------------------------------------
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    const int id = planCounter;
    planCounter++;
    Plan p = Plan( id, settlement, selectionPolicy, this->facilitiesOptions);
    plans.push_back(p);
};

//!!***make sure - maybe we should create a copy and then insert
void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
};

bool Simulation::addSettlement(Settlement *settlement){
    if(isSettlementExists(settlement->getName())){
        delete settlement;
        return false;
    }
    settlements.push_back(settlement);
    return true;
};

bool Simulation::addFacility(FacilityType facility){
    if(isFacilityExists(facility.getName())){
        return false;
    }
    facilitiesOptions.push_back(facility);
    return true;
};

//isExists---------------------------------------------------------------------
bool Simulation::isSettlementExists(const string &settlementName){ 
    for(const Settlement* set: settlements){
        if(settlementName == set->getName()){
            return true;
        }   
    }
    return false;
};

bool Simulation::isFacilityExists(const string &facilityName){
    for(const FacilityType& facType: facilitiesOptions){
        if(facilityName == facType.getName()){
            return true;
        }   
    }
    return false;
};

bool Simulation::isPlanExists(const int planId){
    if (static_cast<size_t>(planId) < plans.size() && planId >= 0)
        return true;
    else
        return false;
};

bool Simulation:: isPolicyExists(const string policy){
    if (policy == "eco" || policy == "nve" || policy == "bal" || policy == "env")
        return true;
    else
        return false;
}

//getters----------------------------------------------------------------------
Settlement& Simulation::getSettlement(const string &settlementName){
    for(Settlement* set: settlements){
        if(settlementName == set->getName()){
            return *set;
        }   
    } 
    throw invalid_argument("Settlement " + settlementName + " doesn't exist");
};

Plan& Simulation::getPlan(const int planID){
    if (static_cast<size_t>(planID) < plans.size() && planID >= 0)
        return plans[planID];
    else
        throw invalid_argument("Plan " + to_string(planID) + " doesn't exist");
};

const vector<Plan>& Simulation::getPlanVector() const{
    return plans;
};

const vector<BaseAction*>& Simulation::getActionsLog() const{
    return actionsLog;
}

//------------------------------------------------------------------------------


void Simulation::step(){
    for(Plan& p: plans){
        p.step();
    }
};

void Simulation::open(){
    isRunning = true;
};        

void Simulation::close(){
    isRunning = false;
};