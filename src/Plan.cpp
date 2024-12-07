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
        this->facilities.emplace_back(new Facility(*Fpointer));
    }
    for(const Facility* UCpointer: other.underConstruction){
        this->underConstruction.emplace_back(new Facility(*UCpointer));
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
        cout << "copyConstructor copying facility " + Fpointer->getName() + "size before " + to_string(facilities.size()) << endl; //TODO delete later
        this->facilities.emplace_back(new Facility(*Fpointer));
        cout << "size after " + to_string(facilities.size()) << endl;
    }
    for(const Facility* UCpointer: other.underConstruction){
        this->underConstruction.emplace_back(new Facility(*UCpointer));
    }
    selectionPolicy = other.selectionPolicy->clone();
 }

 //copy constructor for simulation's copy constructor
Plan::Plan(const Plan &other, Settlement *newSet): 
                plan_id(other.plan_id), 
                settlement(*newSet), 
                selectionPolicy(nullptr),
                status(other.status),
                facilities(),
                underConstruction(),
                facilityOptions(other.facilityOptions),
                life_quality_score(other.life_quality_score), 
                economy_score(other.economy_score), 
                environment_score(other.environment_score){

    for(const Facility* Fpointer: other.facilities){
        cout << "simulations copyConstructor copying facility " + Fpointer->getName() + "size before " + to_string(facilities.size()) << endl; //TODO delete
        this->facilities.emplace_back(new Facility(*Fpointer));
        cout << "size after " + to_string(facilities.size()) << endl; //TODO delete 
    }
    for(const Facility* UCpointer: other.underConstruction){
        this->underConstruction.emplace_back(new Facility(*UCpointer));
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

const SelectionPolicy* Plan::getSelectionPolicy() const{
    return selectionPolicy; }

const string Plan::getSettlementName() const{
    return settlement.getName(); }

const vector<Facility*>& Plan::getFacilities() const{
    return facilities; }

//Copy scores from other------------------------------------------
    void Plan::setScores(const Plan& other){
        life_quality_score = other.getlifeQualityScore();
        economy_score = other.getEconomyScore();
        environment_score = other.getEnvironmentScore();
    }


//step()---------------------------------------------------------
void Plan::step(){
    //stage 2
    //if available, add facilities according to the selection policy
    if (status == PlanStatus:: AVALIABLE){ 
        
        while(underConstruction.size() < static_cast<size_t>(settlement.Settlement::getLimit())){
        
            const FacilityType& nextR = selectionPolicy->selectFacility(facilityOptions);
            Facility* next = new Facility(nextR, settlement.getName());
            underConstruction.emplace_back(next); 
        
        }
    }
    //stage 3
    //iterat ethrough all facilities under construction and advance a step
    //if they become operational, move to facilities vector
    for(auto itr = underConstruction.begin(); itr != underConstruction.end();){
        
        (*itr)->step(); // facility timeLeft--

        if((*itr)->getTimeLeft() == 0 ){
            life_quality_score += (*itr)->getLifeQualityScore();
            economy_score += (*itr)->getEconomyScore();
            environment_score += (*itr)->getEnvironmentScore();
            
            facilities.emplace_back((*itr));
            itr = underConstruction.erase((itr));
        }
        else 
            itr ++;
    }
    //stage 4
    //update plan status
    if(underConstruction.size() == static_cast<size_t>(settlement.getLimit()))
        status = PlanStatus::BUSY;
    else
        status = PlanStatus::AVALIABLE;   
}
//--------------------------end of step--------------------------------------------
//-----------------------------------------------------------------------------------

void Plan::printStatus(){
    cout << to_string(static_cast<int>(status)) << endl; }

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    if (this->getSelectionPolicy() != nullptr)
        delete this->selectionPolicy;
    this->selectionPolicy = selectionPolicy; }

void Plan::addFacility(Facility* facility){
    this->underConstruction.emplace_back(facility);
}

//standart toString()--------------------------------------------------------------------------
const string Plan::toString() const {
    string result;

    //basic information
    result += "planID: " + to_string(plan_id) + "\n"; 
    result += "SettlementName: " + settlement.getName() + "\n";
    result += "PlanStatus: " + toStringPlanStat(status) + "\n";
    result += "SelectionPolicy: " + selectionPolicy->toString() + "\n";
    result += "LifeQualityScore: " + to_string(life_quality_score) + "\n";
    result += "EconomyScore: " + to_string(economy_score) + "\n";
    result += "EnvironmentScore: " + to_string(environment_score) + "\n";

    // under-construction facilities
    result += "Facilities Under Construction size :" + to_string(underConstruction.size()) + "\n";
    for (const Facility* facility : underConstruction) {
        cout << "entered underCOnstruction loop in plan" << endl; //TODO delete later ****
        result += "  FacilityName: " + facility->getName() + "\n";
        result += "  FacilityStatus: " + facility->toStringFacStat(facility->getStatus()) + "\n";
        result += " ***timeLeft: " + to_string(facility->getTimeLeft()) + "\n"; //FIXME: delete later
    }

    //operational facilities
    result += "Operational Facilities:\n";
    for (const Facility* facility : facilities) {
        result += "  FacilityName: " + facility->getName() + "\n";
        result += "  FacilityStatus: " + facility->toStringFacStat((facility->getStatus())) + "\n";
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

string Plan::toStringPlanStat(PlanStatus status) const{
    switch (status) {
        case PlanStatus::AVALIABLE: return "AVALIABLE";
        case PlanStatus::BUSY: return "BUSY";
        default: throw invalid_argument("Invalid PlanStatus value");

    }
}
