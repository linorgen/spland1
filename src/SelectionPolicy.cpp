#include "../include/SelectionPolicy.h"
#include <vector>
using std::vector;
#include <iostream>
#include <algorithm>
using namespace std;

//convert selectionPolicy string to selectionPolicy
    SelectionPolicy* SelectionPolicy::strToPolicy (const string& selectionPolicy){

        SelectionPolicy* pol = nullptr;
        if(selectionPolicy == "env")
            pol = new SustainabilitySelection();
        else if(selectionPolicy =="nve")
            pol = new NaiveSelection();
        else if(selectionPolicy == "bal")
            pol = new BalancedSelection(0,0,0);
        else if(selectionPolicy == "eco")
            pol = new EconomySelection();
        return pol;
    };
    

//Naive selection------------------------------------------------------------------------------------------

    //default constructor
    NaiveSelection::NaiveSelection(): lastSelectedIndex(-1){};

    //copy constructor 
    NaiveSelection::NaiveSelection(const NaiveSelection &other): lastSelectedIndex(other.getLastIndex()){};

    //select next facility to build
    const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){

        if(static_cast<size_t>(lastSelectedIndex) == facilitiesOptions.size())
            lastSelectedIndex = -1;
        lastSelectedIndex++;
        return facilitiesOptions[lastSelectedIndex]; 
    }

    const string NaiveSelection::toString() const{
        return "nve"; }

    const int NaiveSelection::getLastIndex() const{
        return lastSelectedIndex; }

    NaiveSelection* NaiveSelection::clone() const{
        NaiveSelection *clone = new NaiveSelection(*this);
        return clone; }
    


//BalancedSelection-----------------------------------------------------------------------------------------
    
    //constructor
    BalancedSelection::BalancedSelection(int LifeQualityScore, 
                                        int EconomyScore, 
                                        int EnvironmentScore): 
                                                LifeQualityScore(LifeQualityScore),
                                                EconomyScore(EconomyScore), 
                                                EnvironmentScore(EnvironmentScore){};
    
    //copy constructor 
    BalancedSelection::BalancedSelection(const BalancedSelection &other): 
                                                LifeQualityScore(other.LifeQualityScore), 
                                                EconomyScore(other.EconomyScore), 
                                                EnvironmentScore(other.EnvironmentScore){};
    
    //select next facility to build
    const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){

        const FacilityType* best = &facilitiesOptions[0];
        
        for(size_t i = 1; i<facilitiesOptions.size(); i++){

            if (distance(facilitiesOptions[i]) < distance(*best))
                best = &facilitiesOptions[i];
        }
        return *best;
    }
    
    //distance function - aid in selection
    int BalancedSelection::distance(FacilityType facility){

        int lifeq = LifeQualityScore + facility.getLifeQualityScore();
        int eco = EconomyScore + facility.getEconomyScore();
        int envi = EnvironmentScore + facility.getEnvironmentScore();
        return max({lifeq, eco, envi}) - min({lifeq, eco, envi});
    }

    const string BalancedSelection::toString() const{
        return "bal"; }

    BalancedSelection* BalancedSelection::clone() const{
        BalancedSelection *clone = new BalancedSelection(*this);
        return clone; }



//EconomySelection----------------------------------------------------------------------------------------
    
    //constructor
    EconomySelection::EconomySelection(): lastSelectedIndex(-1){};
    
    //copy constructor
    EconomySelection::EconomySelection(const EconomySelection &other): lastSelectedIndex(other.lastSelectedIndex){};
    
    //select next facility to build
    const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){

        bool found = false;
        int index = lastSelectedIndex;
        
        while(!found){
            index++;
            if((static_cast<size_t>(index) < facilitiesOptions.size()) && (facilitiesOptions[index].getCategory() == FacilityCategory::ECONOMY))
                found = true;
            else if(static_cast<size_t>(index) >= facilitiesOptions.size())
                index = -1;
        }
        lastSelectedIndex = index;
        return facilitiesOptions[index]; 
    }

    const string EconomySelection::toString() const{
        return "eco"; }
        
    EconomySelection* EconomySelection::clone() const{
        EconomySelection *clone = new EconomySelection(*this);
        return clone; }
    


//SustainabilitySelection-------------------------------------------------------------------------------
    
    //constructor
    SustainabilitySelection::SustainabilitySelection(): lastSelectedIndex(-1){};
    
    //copy constructor
    SustainabilitySelection::SustainabilitySelection(const SustainabilitySelection &other):   
                                                            lastSelectedIndex(other.lastSelectedIndex){};
    
    //select next facility to build
    const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
        bool found = false;
        int index = lastSelectedIndex;
        
        while(!found){
            index++;
            
            if((static_cast<size_t>(index) < facilitiesOptions.size()) &&(facilitiesOptions[index].getCategory() == FacilityCategory::ENVIRONMENT))
                found = true;
            else if(static_cast<size_t>(index) >= facilitiesOptions.size())
                index = -1;
        }
        lastSelectedIndex = index;
        return facilitiesOptions[index]; 
    }

    const string SustainabilitySelection::toString() const{
        return "env"; }

    SustainabilitySelection* SustainabilitySelection::clone() const{
        SustainabilitySelection *clone = new SustainabilitySelection(*this);
        return clone; }
    