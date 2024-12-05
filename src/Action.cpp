#include "../include/Action.h"
#include "../include/Simulation.h"
#include <iostream>
using namespace std;
extern Simulation* backup;

//BaseAction-----------------------------------------------------------------------
    BaseAction::BaseAction():errorMsg(""),status(ActionStatus::COMPLETED){};
    
    ActionStatus BaseAction::getStatus() const{ return status; };
    
    string BaseAction::getStatusStr() const{ 
        if(status == ActionStatus::COMPLETED)
            return "COMPLETED";
        else if (status == ActionStatus::ERROR)
            return "ERROR";
        return "No status allocated";
    };

    void BaseAction::complete(){ 
        status = ActionStatus::COMPLETED; };

    void BaseAction::error(string errorMsg) { 
        status = ActionStatus::ERROR;
        errorMsg = errorMsg;
    };
    const string& BaseAction::getErrorMsg() const {
        return errorMsg;
    };

//SimulateStep-----------------------------------------------------------------
    SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){};

    void SimulateStep::act(Simulation &simulation){
        int stepsLeft = numOfSteps;
        while(stepsLeft != 0){
            simulation.step();
            stepsLeft--;
        }
        simulation.addAction(this->clone());
    };

    const string SimulateStep::toString() const{
        return "simulateStep " + to_string(numOfSteps) + " "+ getStatusStr();
    } ;

    SimulateStep* SimulateStep::clone() const{
        return new SimulateStep(*this); 
    };

//AddPlan-------------------------------------------------------------------------------------------------------
    AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName),selectionPolicy(selectionPolicy){};
    
    void AddPlan::act(Simulation &simulation){
        if (simulation.isSettlementExists(settlementName) && simulation.isPolicyExists(selectionPolicy)){
            Settlement& set = simulation.getSettlement(settlementName);
            SelectionPolicy *policy = SelectionPolicy::strToPolicy(selectionPolicy);
            simulation.addPlan(set, policy);
        }
        else{
            BaseAction::error("Cannot create this plan"); 
            cout<< getErrorMsg() << endl;
        }
        simulation.addAction(this->clone());
    };
    
    const string AddPlan::toString() const{
        return "plan " + settlementName + " " + selectionPolicy + " "+ getStatusStr(); 
    };
    
    AddPlan* AddPlan::clone() const {
        return new AddPlan(*this); 
    };

//AddSettlement-------------------------------------------------------------------------------------------------------
    AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName), settlementType(settlementType){};
    
    void AddSettlement::act(Simulation &simulation){
        if(simulation.isSettlementExists(settlementName)){
            BaseAction::error("Settlment already exsists"); 
            cout<< getErrorMsg() << endl;
        }
        else{
            Settlement* newSet = new Settlement(settlementName, settlementType);
            simulation.addSettlement(newSet);
        }
        simulation.addAction(this->clone());
    };

    AddSettlement* AddSettlement::clone() const{
        return new AddSettlement(*this); 
    };
        
    const string AddSettlement::toString() const{
        string type;
        if(settlementType == SettlementType::VILLAGE){
            type = "VILLAGE";}
        else if(settlementType == SettlementType::CITY){
            type = "CITY";}
        else if(settlementType == SettlementType::METROPOLIS){
            type = "METROPOLIS";}
        return "settlement " + settlementName + " " + type + " "+ getStatusStr();
    };

//AddFacility-----------------------------------------------------------------------
    AddFacility::AddFacility(const string &facilityName, 
                            const FacilityCategory facilityCategory, 
                            const int price, const int lifeQualityScore, 
                            const int economyScore, const int environmentScore):

                                    facilityName(facilityName), 
                                    facilityCategory(facilityCategory), price(price), 
                                    lifeQualityScore(lifeQualityScore),
                                    economyScore(economyScore), 
                                    environmentScore(environmentScore){};

    void AddFacility::act(Simulation &simulation){
        if(simulation.isFacilityExists(facilityName)){
            BaseAction::error("Facility already exsists"); 
            cout<< getErrorMsg() << endl;
        }
        else{
            FacilityType newf = FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
            simulation.addFacility(newf);
        }
        simulation.addAction(this->clone());
    };
    
    AddFacility* AddFacility::clone() const {
        return new AddFacility(*this); 
    };

    const string AddFacility::toString() const{
        int category = static_cast<int>(facilityCategory);
        return "facility " + facilityName + to_string(category) + " " + to_string(price)+ " " 
        + to_string(lifeQualityScore) + " " + to_string(economyScore) 
        + " " + to_string(environmentScore) + " "+ getStatusStr(); 
    };

//PrintPlanStatus-----------------------------------------------------------------------
    PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){};
    
    void PrintPlanStatus::act(Simulation &simulation){
        if(simulation.isPlanExists(planId)){
            cout << (simulation.getPlan(planId)).toString() << endl; 
        }
        else{
            BaseAction::error("Plan doesn't exsist"); 
            cout<< getErrorMsg() << endl;
        }
        simulation.addAction(this->clone());
    };

    PrintPlanStatus* PrintPlanStatus::clone() const {
        return new PrintPlanStatus(*this); 
    };
    
    const string PrintPlanStatus::toString() const {
        return "planStatus " + to_string(planId) + " "+ getStatusStr();
    };

//ChangePlanPolicy-----------------------------------------------------------------------
    ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):planId(planId), newPolicy(newPolicy){};
    
    void ChangePlanPolicy::act(Simulation &simulation){
        if(simulation.isPolicyExists(newPolicy) && simulation.isPlanExists(planId)){
            const SelectionPolicy* currPol = (simulation.getPlan(planId)).getSelectionPolicy();
            
            //if the current policy is equal to the new policy
            if(currPol->toString() == newPolicy){
                BaseAction::error("Cannot change selection policy"); 
                cout<< getErrorMsg() << endl;
            }
            else{
                (simulation.getPlan(planId)).setSelectionPolicy(SelectionPolicy::strToPolicy(newPolicy));
            }
        }
        //if the selection policy or planID don't exist
        else{
            BaseAction::error("Cannot change selection policy"); // set error message
        }
        simulation.addAction(this->clone());
    };

    ChangePlanPolicy* ChangePlanPolicy::clone() const {
        return new ChangePlanPolicy(*this); 
    };
    
    const string ChangePlanPolicy::toString() const {
        return "changePolicy " + to_string(planId) + " " + newPolicy + " " + getStatusStr();
    };

//PrintActionsLog-----------------------------------------------------------------------
    PrintActionsLog::PrintActionsLog(){};
    
    void PrintActionsLog::act(Simulation &simulation) {
        //print actions log mimush
        simulation.addAction(this->clone());
    };
    
    PrintActionsLog* PrintActionsLog::clone() const {
        return new PrintActionsLog(*this); 
    };
    
    const string PrintActionsLog::toString() const { 
        return "printLog " + getStatusStr();
    };

//Close-----------------------------------------------------------------------------
    Close::Close(){};
    
    void Close::act(Simulation &simulation) {
        
        for(const Plan& plan : simulation.getPlanVector()){
            int planId = plan.getPlanId();
            cout << simulation.getPlan(planId).toStringClose() << endl;
        }
        
    };
    //add clone to actionslog?
    Close* Close::clone() const {
        return new Close(*this); 
    };
    
    const string Close::toString() const { 
        return "close " + getStatusStr();
    };

//BackupSimulation-----------------------------------------------------------------------
    BackupSimulation::BackupSimulation(){};
    
    void BackupSimulation::act(Simulation &simulation) {
        //deep copy to simulation
        simulation.addAction(this->clone());
    };
    
    BackupSimulation* BackupSimulation::clone() const {
        return new BackupSimulation(*this); 
    };
    
    const string BackupSimulation::toString() const {
        return "backup " + getStatusStr();
    };

//RestoreSimulation-----------------------------------------------------------------------
    RestoreSimulation::RestoreSimulation(){};
    void RestoreSimulation::act(Simulation &simulation) {
        //lemamesh
        simulation.addAction(this->clone());
    };
    RestoreSimulation* RestoreSimulation::clone() const {
        return new RestoreSimulation(*this); 
    };
    const string RestoreSimulation::toString() const {
        return "restore" + getStatusStr();
    };