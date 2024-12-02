
#include "../include/Plan.h"
#include "../include/Settlement.h"
#include "../include/Facility.h"
#include "../include/Action.h"
#include "../include/SelectionPolicy.h"
#include "../include/Simulation.h"
#include <iostream>
using namespace std;

//BaseAction
    BaseAction::BaseAction():errorMsg(""),status(ActionStatus::COMPLETED){};
    ActionStatus BaseAction::getStatus() const{ return status; };
    string BaseAction::getStatusStr() const{ 
        if(status == ActionStatus::COMPLETED)
        return "COMPLETED";
        else if (status == ActionStatus::ERROR)
            return "ERROR";
    };

    void BaseAction::complete(){ status = ActionStatus::COMPLETED; };
    void BaseAction::error(string errorMsg) { 
        status = ActionStatus::ERROR;
        errorMsg = errorMsg;
    };
    const string& BaseAction::getErrorMsg() const {
        return errorMsg;
    };

//SimulateStep
    //Constructor
    SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){};
    void SimulateStep::act(Simulation &simulation){
        int stepsLeft = numOfSteps;
        while(stepsLeft != 0){
            simulation.step();
            stepsLeft--;
        }
    };
    const string SimulateStep::toString() const{
        return "simulateStep " + to_string(numOfSteps) + " "+ getStatusStr();
    } ;
    SimulateStep SimulateStep:: *clone() const override{
        //for the log
    };

//AddPlan
    AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName),selectionPolicy(selectionPolicy){};
    void AddPlan::act(Simulation &simulation){
        Settlement& set = simulation.getSettlement(settlementName);
        SelectionPolicy *policy = SelectionPolicy::strToPolicy(selectionPolicy);
        if (set == nullptr || policy == nullptr) {
            BaseAction::error("Cannot create this plan"); // set error message
                        //need to check how to return the error ms to user
        }
        else{
            simulation.addPlan(set, policy);
        }
    };
    const string AddPlan::toString() const{
        return "plan " + settlementName + " " + selectionPolicy + " "+ getStatusStr();  
        //for the log
    };
    AddPlan* AddPlan::clone() const override;

//AddSettlement
    AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName), settlementType(SettlementType){};
    void AddSettlement::act(Simulation &simulation){
        if(simulation.isSettlementExists(settlementName)){
            BaseAction::error("Settlment already exsists"); // set error message
            //need to check how to return the error ms to user
        }
        else{
            Settlement* newSet = new Settlement(settlementName, settlementType);
            simulation.addSettlement(newSet);
        }
    } ;
    AddSettlement* AddSettlement::clone() const override{
        //for the log
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

//AddFacility
    AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
    facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore),economyScore(economyScore), environmentScore(enviromentScore){};
    void AddFacility::act(Simulation &simulation){
        if(simulation.isFacilityExists(facilityName)){
            BaseAction::error("Facility already exsists"); // set error message
        }
        else{
            FacilityType newf = FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
            simulation.addFacility(newf);
        }
    } ;
    AddFacility* AddFacility::clone() const {};
    const string AddFacility::toString() const{
        int category = static_cast<int>(facilityCategory);
        return "facility " + facilityName + to_string(category) + " " + to_string(price)+ " " 
        + to_string(lifeQualityScore) + " " + to_string(economyScore) + " " + to_string(environmentScore) + " "+ getStatusStr(); 
    };

//PrintPlanStatus
    PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){};
    void PrintPlanStatus::act(Simulation &simulation){
        if(simulation.isPlanExists(planId)){
            (simulation.getPlan(planId)).toString();
        }
        else{
            BaseAction::error("Plan doesn't exsist"); // set error message
        }
    };
    PrintPlanStatus* PrintPlanStatus::clone() const {};
    const string PrintPlanStatus::toString() const {
        return "planStatus " + to_string(planId) + " "+ getStatusStr();
    };

//ChangePlanPolicy
    ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):planId(planId), newPolicy(newPolicy){};
    void ChangePlanPolicy::act(Simulation &simulation){
        const SelectionPolicy* currPol = (simulation.getPlan(planId)).getSelectionPolicy();
        if(currPol->toString() == newPolicy){
            BaseAction::error("Cannot change selection policy"); // set error message
        }
        else{
            SelectionPolicy* newPol = SelectionPolicy::strToPolicy(newPolicy);
            (simulation.getPlan(planId)).setSelectionPolicy(newPol);
        }
    };
    ChangePlanPolicy* ChangePlanPolicy::clone() const {};
    const string ChangePlanPolicy::toString() const {
        return "changePolicy " + to_string(planId) + " " + newPolicy + " " + getStatusStr();
    };

//PrintActionsLog
    PrintActionsLog::PrintActionsLog();
    void PrintActionsLog::act(Simulation &simulation) override;
    PrintActionsLog* PrintActionsLog::clone() const {};
    const string PrintActionsLog::toString() const {
        return "printLog " + getStatusStr();
    };

//Close
    Close::Close(){};
    void Close::act(Simulation &simulation) {};
    Close* Close::clone() const {};
    const string Close::toString() const {
        return "close " + getStatusStr();
    };

//BackupSimulation
    BackupSimulation::BackupSimulation(){};
    void BackupSimulation::act(Simulation &simulation) {};
    BackupSimulation* BackupSimulation::clone() const {};
    const string BackupSimulation::toString() const {
        return "backup " + getStatusStr();
    };

//RestoreSimulation
    RestoreSimulation::RestoreSimulation(){};
    void RestoreSimulation::act(Simulation &simulation) {};
    RestoreSimulation* RestoreSimulation::clone() const {};
    const string RestoreSimulation::toString() const {
        return "restore" + getStatusStr();
    };