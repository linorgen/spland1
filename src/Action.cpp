
#include "Plan.h"
#include "Settlement.h"
#include "Facility.h"
#include "Action.h"
#include "SelectionPolicy.h"
#include "Simulation.h"
#include <iostream>
using namespace std;

//BaseAction
    BaseAction::BaseAction():errorMsg(""),status(ActionStatus::COMPLETED){};
    ActionStatus BaseAction::getStatus() const{ return status; };
    void BaseAction::complete(){ status = ActionStatus::COMPLETED };
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
    const string SimulateStep::toString() const override{
        //for the log
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
    const string AddPlan::toString() const override{
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
    AddSettlement* AddSettlement::clone() const override;
        //for the log
    const string AddSettlement::toString() const override;
        //for the log

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
    AddFacility* AddFacility::clone() const override;
    const string AddFacility::toString() const override;

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
    PrintPlanStatus* PrintPlanStatus::clone() const override;
    const string PrintPlanStatus::toString() const override;

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
    ChangePlanPolicy* ChangePlanPolicy::clone() const override;
    const string ChangePlanPolicy::toString() const override;

//PrintActionsLog
    PrintActionsLog();
    void act(Simulation &simulation) override;
    PrintActionsLog *clone() const override;
    const string toString() const override;

//Close
    Close();
    void act(Simulation &simulation) override;
    Close *clone() const override;
    const string toString() const override;

//BackupSimulation
    BackupSimulation();
    void act(Simulation &simulation) override;
    BackupSimulation *clone() const override;
    const string toString() const override;

//RestoreSimulation
    RestoreSimulation();
    void act(Simulation &simulation) override;
    RestoreSimulation *clone() const override;
    const string toString() const override;