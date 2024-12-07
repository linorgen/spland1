#include "../include/Facility.h"
#include <iostream>
using namespace std;


//FacilityType-----------------------------------------------------------------------------

    //empty constructor
    FacilityType::FacilityType(): name(""), 
                                category(FacilityCategory::ECONOMY), 
                                price(0), 
                                lifeQuality_score(0), 
                                economy_score(0), 
                                environment_score(0) {}

    //constructor
    FacilityType::FacilityType(const string &name, 
                                const FacilityCategory category, 
                                const int price, 
                                const int lifeQuality_score, 
                                const int economy_score, 
                                const int environment_score):
                                    name(name), 
                                    category(category), 
                                    price(price), 
                                    lifeQuality_score(lifeQuality_score), 
                                    economy_score(economy_score), 
                                    environment_score(environment_score) {}

    // Copy constructor
    FacilityType::FacilityType(const FacilityType &other): 
                                    name(other.name), 
                                    category(other.category), 
                                    price(other.price),
                                    lifeQuality_score(other.lifeQuality_score), 
                                    economy_score(other.economy_score), 
                                    environment_score(other.environment_score) {}

    const string &FacilityType::getName() const{
        return name; }

    int FacilityType::getCost() const{
        return price; }

    int FacilityType::getLifeQualityScore() const{
        return lifeQuality_score; }

    int FacilityType::getEnvironmentScore() const{
        return environment_score; }

    int FacilityType::getEconomyScore() const{
        return economy_score; }

    FacilityCategory FacilityType::getCategory() const{
        return category; }
    
    string FacilityType::toStringFacCat(const FacilityCategory category) const{
    switch (category) {
        case FacilityCategory::LIFE_QUALITY: return "LIFE_QUALITY";
        case FacilityCategory::ECONOMY:      return "ECONOMY";
        case FacilityCategory::ENVIRONMENT:  return "ENVIRONMENT";
        default: throw::invalid_argument("Invalid FacilityCategory value");
        }
    }

    string FacilityType::toStringFacStat(const FacilityStatus status) const{
    switch (status) {
        case FacilityStatus::UNDER_CONSTRUCTIONS: return "UNDER_CONSTRUCTIONS";
        case FacilityStatus::OPERATIONAL: return "OPERATIONAL";
        default: throw std::invalid_argument("Invalid FacilityStatus value");
        }
    }
//----------------------------------------------------------  


//Facility-------------------------------------------------------------------------------------
   
    //empty constructor
    Facility::Facility(): FacilityType(), 
                            settlementName(""), 
                            status(FacilityStatus:: UNDER_CONSTRUCTIONS),
                            timeLeft(0) {}
    
    //constructor
    Facility::Facility(const string &name, 
                        const string &settlementName, 
                        const FacilityCategory category, 
                        const int price, 
                        const int lifeQuality_score, 
                        const int economy_score, 
                        const int environment_score): 
                            FacilityType(name, 
                                        category, 
                                        price, 
                                        lifeQuality_score, 
                                        economy_score, 
                                        environment_score),
                                    
                            settlementName(settlementName),
                            status(FacilityStatus:: UNDER_CONSTRUCTIONS),
                            timeLeft(price){}

    //copy constructor that gets FacilityType
    Facility::Facility(const FacilityType &type, 
                        const string &settlementName): 
                            FacilityType(type), 
                            settlementName(settlementName),
                            status(FacilityStatus:: UNDER_CONSTRUCTIONS),
                            timeLeft(price){}

    //copy constructor that gets Facility
    Facility::Facility(const Facility &other): FacilityType(other), 
                                                settlementName(other.settlementName), 
                                                status(other.status), 
                                                timeLeft(other.timeLeft) {}

    
    //getters------------------------------------------------------------------------
    const string &Facility::getSettlementName() const{
        return name; }

    const int Facility::getTimeLeft() const{
        return timeLeft; }

    const FacilityStatus& Facility::getStatus() const{
        return status; }
    //-------------------------------------------------------------------------------

    FacilityStatus Facility::step(){
        if(timeLeft < 0 )
            throw invalid_argument("Facility is in negative price");
        timeLeft--;
        
        if(timeLeft == 0){            
            status = FacilityStatus:: OPERATIONAL;
        }
        return status;
    }
    

    void Facility::setStatus(FacilityStatus status){
        status = status; 
    }

    
    
    const string Facility::toString() const{

        return name + " " + to_string(static_cast<int>(category)) + " " +
                to_string(price) + to_string(lifeQuality_score) + to_string(economy_score) 
                + to_string(environment_score); 
    }
