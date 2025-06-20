#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const Plan &other, const int planId);
        Plan(const int planId, const Settlement &settlement, const string &selectionPolicy, const vector<FacilityType> &facilityOptions); 
        ~Plan();
        Plan(const Plan &other); //copy constructor
        Plan(const Plan &other, Settlement *newSet); //copy constructor for simulation's copy constructor
        Plan(Plan&& other); //move constructor
        Plan& operator = (const Plan&) = delete; //deleted copy assignment operator
        Plan& operator = (Plan&&) = delete; //deleted move assignment operator
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        const int getPlanId() const;
        const SelectionPolicy* getSelectionPolicy() const;
        const vector<Facility*> &getFacilities() const;
        const string getSettlementName() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void setScores(const Plan& other);
        void step();
        void printStatus();
        void addFacility(Facility* facility);
        const string toString() const;
        const string toStringClose() const;
        string toStringPlanStat(PlanStatus status) const; // convert enum PlanStatus to string
        
    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; 
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};