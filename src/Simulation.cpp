#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "../include/Auxiliary.h"
#include "../include/Settlement.h"
#include "../include/Facility.h"
#include "../include/Plan.h"
#include "../include/SelectionPolicy.h"
#include "../include/Simulation.h"
#include <Action.h>
using std::string;
using std::vector;
using namespace std;



Simulation::Simulation(const string &configFilePath):planCounter(0){
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
        
        else if (arguments[0] == "Facility"){
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
        
        else if (arguments[0] == "Plan") {
            SelectionPolicy* pol = SelectionPolicy::strToPolicy(arguments[2]);
            addPlan(getSettlement(arguments[1]), pol);
        }
    }
    configFile.close();  // Close the config file after processing
    start();
}

void Simulation::start(){
    cout<<"The simulation has started"<<endl;
    open();
    //wait for user to enter actions
    while(isRunning){
        cout << "enter next command" << endl;
        string text;
        cin >> text;
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
        else if(input[0] == "close"){
            Close* closeA = new Close();
            closeA->act(*this);
            close();
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
    //delete everything
};

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    const int id = planCounter;
    planCounter++;
    Plan p = Plan( id,  settlement, selectionPolicy, this->facilitiesOptions );
    plans.push_back(p);
};

//make sure - maybe we should create a copy and then insert
void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
};

bool Simulation::addSettlement(Settlement *settlement){
    settlements.push_back(settlement);
};
bool Simulation::addFacility(FacilityType facility){
    facilitiesOptions.push_back(facility);
};
bool Simulation::isSettlementExists(const string &settlementName){ 
    for(const Settlement* set: settlements){
        if(settlementName == set->getName()){
            return true;
        }   
    }
    return false;
};

bool Simulation::isFacilityExists(const string &facilityName){
    for(const FacilityType facType: facilitiesOptions){
        if(facilityName == facType.getName()){
            return true;
        }   
    }
    return false;
};

bool Simulation::isPlanExists(const int planId){
    if (planId < plans.size() && planId >= 0)
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

Settlement& Simulation::getSettlement(const string &settlementName){
    for(Settlement* set: settlements){
        if(settlementName == set->getName()){
            return *set;
        }   
    } 
    throw invalid_argument("Settlement " + settlementName + " doesn't exist");
};
Plan& Simulation::getPlan(const int planID){
    if (planID < plans.size() && planID >= 0)
        return plans[planID];
    else
        throw invalid_argument("Plan " + to_string(planID) + " doesn't exist");
};

const vector<Plan>& Simulation::getPlanVector(){
    return plans;
};

void Simulation::step(){
    for(Plan p: plans){
        p.step();
        //*** add to actionsLog *//
    }
};

void Simulation::open(){
    isRunning = true;
};        

void Simulation::close(){
    isRunning = false;

};