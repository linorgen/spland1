#include "../include/Plan.h"
#include <iostream>
using namespace std;

//constructor
Plan::Plan(const int planId, 
            const Settlement &settlement, 
            SelectionPolicy *selectionPolicy, 
            const vector<FacilityType> &facilityOptions): 
                    plan_id(planId), 
                    settlement(settlement), 
                    selectionPolicy(selectionPolicy),
                    status(PlanStatus::AVALIABLE),
                    facilities(),
                    underConstruction(), 
                    facilityOptions(facilityOptions), 
                    life_quality_score(0),
                    economy_score(0),
                    environment_score(0){}


//constructor that uses selection policy string
Plan::Plan(const int planId, 
            const Settlement &settlement, 
            const string &selectionpolicy, 
            const vector<FacilityType> &facilityOptions): 
                    plan_id(planId), 
                    settlement(settlement), 
                    selectionPolicy(nullptr),
                    status(PlanStatus::AVALIABLE),
                    facilities(),
                    underConstruction(),
                    facilityOptions(facilityOptions),
                    life_quality_score(0),
                    economy_score(0),
                    environment_score(0){
    SelectionPolicy* pol;
    if(selectionpolicy == "env")
        pol = new SustainabilitySelection();
    else if(selectionpolicy =="nve")
        pol = new NaiveSelection();
    else if(selectionpolicy == "bal")
        pol = new BalancedSelection(0,0,0);
    else if(selectionpolicy == "eco")
        pol = new EconomySelection(); 
    selectionPolicy = pol;
}

//check if needed - it duplicates statuses and gives new ID
//duplicate plan constructor
Plan::Plan(const Plan &other, const int thisplanId): 
                plan_id(thisplanId), 
                settlement(other.settlement), 
                selectionPolicy(nullptr),
                status(other.status),
                facilities(),
                underConstruction(),
                facilityOptions(other.facilityOptions),
                life_quality_score(other.life_quality_score), 
                economy_score(other.economy_score), 
                environment_score(other.environment_score){

    for(const Facility* Fpointer: other.facilities){
        this->facilities.push_back(new Facility(*Fpointer));
    }
    for(const Facility* UCpointer: other.underConstruction){
        this->underConstruction.push_back(new Facility(*UCpointer));
    }
    selectionPolicy = other.selectionPolicy->clone();
 }

//copy constructor
Plan::Plan(const Plan &other): 
                plan_id(other.plan_id), 
                settlement(other.settlement), 
                selectionPolicy(nullptr),
                status(other.status),
                facilities(),
                underConstruction(),
                facilityOptions(other.facilityOptions),
                life_quality_score(other.life_quality_score), 
                economy_score(other.economy_score), 
                environment_score(other.environment_score){

    for(const Facility* Fpointer: other.facilities){
        this->facilities.push_back(new Facility(*Fpointer));
    }
    for(const Facility* UCpointer: other.underConstruction){
        this->underConstruction.push_back(new Facility(*UCpointer));
    }
    selectionPolicy = other.selectionPolicy->clone();
 }

// Destructor -*****CHECK!
 Plan::~Plan(){
    // Clean up facilities
    for (Facility* facility : facilities) {
        delete facility;
    }

    // Clean up underConstruction
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    delete selectionPolicy;
}

//Move constructor
Plan::Plan(Plan&& other):
                plan_id(other.plan_id), 
                settlement(other.settlement), 
                selectionPolicy(other.selectionPolicy),
                status(other.status),
                facilities(move(other.facilities)),
                underConstruction(move(other.underConstruction)),
                facilityOptions(other.facilityOptions),
                life_quality_score(other.life_quality_score), 
                economy_score(other.economy_score), 
                environment_score(other.environment_score){
        other.selectionPolicy = nullptr;
    }

//Getters--------------------------------------------------
const int Plan::getlifeQualityScore() const{
    return life_quality_score; }

const int Plan::getEconomyScore() const{
    return economy_score; }

const int Plan::getEnvironmentScore() const{
    return environment_score; }

const int Plan::getPlanId() const{
    return plan_id; }

const SelectionPolicy* Plan::getSelectionPolicy(){
    return selectionPolicy; }

string Plan::getSettlementName(){
    return settlement.getName(); }

const vector<Facility*>& Plan::getFacilities() const{
    return facilities; }



//step()---------------------------------------------------------
void Plan::step(){
    //step 2
    //if available, add facilities according to the selection policy
    if (status == PlanStatus:: AVALIABLE){ 
        while(underConstruction.size() < static_cast<size_t>(settlement.Settlement::getLimit())){

            const FacilityType& nextR = selectionPolicy->selectFacility(facilityOptions);
            Facility next = Facility(nextR, settlement.getName());
            underConstruction.push_back(&next); 
        }
    }
    //step 3
    //iterat ethrough all facilities under construction and advance a step
    //if they become operational, move to facilities vector
    for(size_t i = 0; i < underConstruction.size(); i++){
        underConstruction[i]->step();

        if(underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL){
            facilities.push_back(underConstruction[i]);
            underConstruction.erase(underConstruction.begin() + i);
        }
    }
    //step 4
    //update plan status
    if(underConstruction.size() == static_cast<size_t>(settlement.getLimit()))
        status = PlanStatus::BUSY;
    else
        status = PlanStatus::AVALIABLE;   
}
//--------------------------end of step()--------------------------------------------
//-----------------------------------------------------------------------------------

void Plan::printStatus(){
    cout << to_string(static_cast<int>(status)) << endl; }

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    selectionPolicy = selectionPolicy; }

void Plan::addFacility(Facility* facility){
    // FacilityType newf(facility->getName(), facility->getCategory(), facility->getCost(), facility->getLifeQualityScore(), facility->getEconomyScore(), facility->getEnvironmentScore());
    // this->facilityOptions.push_back(newf);
    this->underConstruction.push_back(facility);
}

//standart toString()--------------------------------------------------------------------------
const string Plan::toString() const {
    string result;

    //basic information
    result += "planID: " + to_string(plan_id) + " SettlementName: " + settlement.getName() + "\n";
    result += "PlanStatus: " + to_string(static_cast<int>(status)) + "\n";
    result += "SelectionPolicy: " + selectionPolicy->toString() + "\n";
    result += "LifeQualityScore: " + to_string(life_quality_score) + "\n";
    result += "EconomyScore: " + to_string(economy_score) + "\n";
    result += "EnvironmentScore: " + to_string(environment_score) + "\n";

    // under-construction facilities
    result += "Facilities Under Construction:\n";
    for (Facility* facility : underConstruction) {
        result += "  FacilityName: " + facility->getName() + "\n";
        result += "  FacilityStatus: " + to_string(static_cast<int>(facility->getStatus())) + "\n";
    }

    //operational facilities
    result += "Operational Facilities:\n";
    for (Facility* facility : facilities) {
        result += "  FacilityName: " + facility->getName() + "\n";
        result += "  FacilityStatus: " + to_string(static_cast<int>(facility->getStatus())) + "\n";
    }

    return result;
}

    
//similar to toString, excluding plan status for Action::Close---------------------------------
const string Plan::toStringClose() const{
    string result;

    result += "planID: " + to_string(plan_id) + " SettlementName: " + settlement.getName() + "\n";
    result += "SelectionPolicy: " + selectionPolicy->toString() + "\n";
    result += "LifeQualityScore: " + to_string(life_quality_score) + "\n";
    result += "EconomyScore: " + to_string(economy_score) + "\n";
    result += "EnvironmentScore: " + to_string(environment_score) + "\n";

    return result;
}
