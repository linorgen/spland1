#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "../include/Action.h"
#include "Auxiliary.h"
#include "SelectionPolicy.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath); //constructor
        ~Simulation();  // Destructor
        Simulation(const Simulation& other);  // Copy constructor
        Simulation& operator=(const Simulation& other);  // Copy assignment operator
        Simulation(Simulation&& other) noexcept;  // Move constructor
        Simulation& operator=(Simulation&& other) noexcept;  // Move assignment operator
        
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isFacilityExists(const string &facilityName);
        bool isPlanExists(const int planId);
        bool isPolicyExists(const string policy);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        const vector<BaseAction*>& getActionsLog() const;
        const vector<Plan>& getPlanVector() const; 
        void step();
        void close();
        void open();
        string toString() const;

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};