#include "../include/Plan.h"
#include "../include/Settlement.h"
#include "../include/Facility.h"
#include <iostream>
using namespace std;

//constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions): 
plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions){
    life_quality_score = 0;
    economy_score = 0;
    environment_score = 0; };

//constructor that uses selection policy string
Plan::Plan(const int planId, const Settlement &settlement, const string &selectionPolicy, const vector<FacilityType> &facilityOptions): 
plan_id(planId), settlement(settlement), facilityOptions(facilityOptions){
    life_quality_score = 0;
    economy_score = 0;
    environment_score = 0; 
    SelectionPolicy* pol;
    if(selectionPolicy == "env")
        pol = new SustainabilitySelection();
    else if(selectionPolicy =="nve")
        pol = new NaiveSelection();
    else if(selectionPolicy == "bal")
        pol = new BalancedSelection(0,0,0);
    else if(selectionPolicy == "eco")
        pol = new EconomySelection(); 
    };

//copy constructor
Plan::Plan(const Plan &other, const int thisplanId): plan_id(thisplanId), settlement(other.settlement), selectionPolicy(other.selectionPolicy), facilityOptions(other.facilityOptions),
 life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score){
    for(const Facility* Fpointer: other.facilities){
        this->facilities.push_back(new Facility(*Fpointer));
    }
    for(const Facility* UCpointer: other.underConstruction){
        this->underConstruction.push_back(new Facility(*UCpointer));
    }
 };

// Destructor
 Plan::~Plan() {
    // Clean up facilities
    for (Facility* facility : facilities) {
        delete facility;
    }
    facilities.clear();

    // Clean up underConstruction
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();
}

// Getters
const int Plan::getlifeQualityScore() const{
    return life_quality_score; }
const int Plan::getEconomyScore() const{
    return economy_score; }
const int Plan::getEnvironmentScore() const{
    return environment_score; }
const int Plan::getPlanId() const{
    return plan_id; };
const SelectionPolicy* Plan::getSelectionPolicy(){
    return selectionPolicy; };

void setSelectionPolicy(SelectionPolicy *selectionPolicy){
    selectionPolicy = selectionPolicy; }

void Plan::step(){
    //step 2
    //if available, add facilities according to the selection policy
    if (status == PlanStatus:: AVALIABLE){
        while(underConstruction.size() < settlement.Settlement::getLimit()){
            const FacilityType& nextR = selectionPolicy->selectFacility(facilityOptions);
            Facility next = Facility(nextR, settlement.getName());
            underConstruction.push_back(&next); }
    }
    //step 3
    //iterat ethrough all facilities under construction and advance a step
    //if they become operational, move to facilities vector
    for(int i = 0; i < underConstruction.size(); i++){
        underConstruction[i]->step();
        if(underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL){}
            facilities.push_back(underConstruction[i]);
            underConstruction.erase(underConstruction.begin() + i);
    }
    //step 4
    //update plan status
    if(underConstruction.size() == settlement.getLimit())
        status = PlanStatus::BUSY;
    else
        status = PlanStatus::AVALIABLE;   
    }

    void Plan::printStatus(){
        cout << to_string(static_cast<int>(status)) << endl;
    }

    const vector<Facility*>& Plan::getFacilities() const{
        return facilities;
    }
    void Plan::addFacility(Facility* facility){
        // FacilityType newf(facility->getName(), facility->getCategory(), facility->getCost(), facility->getLifeQualityScore(), facility->getEconomyScore(), facility->getEnvironmentScore());
        // this->facilityOptions.push_back(newf);
        this->underConstruction.push_back(facility);
    }
    const string Plan::toString() const{
        cout << "planID: " + to_string(plan_id) << endl;
        cout << "SettlementName: " + settlement.getName() << endl;
        cout << "PlanStatus: " + to_string(static_cast<int>(status)) << endl;
        cout << "SelectionPolicy: " + selectionPolicy->toString() << endl;
        cout << "LifeQualityScore: " + to_string(life_quality_score) << endl;
        cout << "EconomyScore: " + to_string(economy_score) << endl;
        cout << "EnvironmentScore: " + to_string(environment_score) << endl;
        //facilities under constructin loop
        for(Facility* facility: underConstruction){
            cout << "FacilityName: " + facility->getName() << endl;
            cout << "FacilityStatus: " + to_string(static_cast<int>(facility->getStatus())) << endl;
        }
        //facilities loop
        for(Facility* facility: facilities){
            cout << "FacilityName: " + facility->getName() << endl;
            cout << "FacilityStatus: " + to_string(static_cast<int>(facility->getStatus())) << endl;
        }
    }
