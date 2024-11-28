#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "Auxiliary.h"
#include "Settlement.h"
#include "Facility.h"
#include "Plan.h"
#include "SelectionPolicy.h"
#include "Simulation.h"
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;
using namespace std;



Simulation::Simulation(const string &configFilePath){
    ifstream configFile(configFilePath);  // Open the config file for reading
    if (!configFile.is_open()) {
        std::cerr << "Unable to open configuration file: " << configFilePath << std::endl;
        throw std::runtime_error("Failed to open configuration file.");
    }
    string line;
    while (getline(configFile, line)){
        vector<string> arguments = Auxiliary::parseArguments(line);

        if (arguments.empty() || arguments[0] == "#") continue;  // Skip empty lines or instructions
        else if (arguments[0] == "settlement") {
            if()
            Settlement set = new Settlement(arguments[1], arguments[2]);
            addSettlement();
        }
        else if (arguments[0] == "Facility"){

        }
        else if (arguments[0] == "Plan") {

        }
    }
    configFile.close();  // Close the config file after processing
    start();
}

void Simulation::start(){
    cout<<"The simulation has started"<<endl;
    open();
    //wait for user to enter actions
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
Settlement& Simulation::getSettlement(const string &settlementName){
    for(Settlement* set: settlements){
        if(settlementName == set->getName()){
            return *set;
        }   
    }
};
Plan& Simulation::getPlan(const int planID){
    for(Plan p: plans){
        if(planID == p.getPlanId()){
            return p;
        }   
    }
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

};