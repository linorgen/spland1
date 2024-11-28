#include "../include/Settlement.h"
#include "../include/Facility.h"
#include <iostream>
using namespace std;

// Simulation* backup = nullptr;

int main(int argc, char** argv){
    // if(argc!=2){
    //     cout << "usage: simulation <config_path>" << endl;
    //     return 0;
    // }
    // string configurationFile = argv[1];
    // Simulation simulation(configurationFile);
    // simulation.start();
    // if(backup!=nullptr){
    // 	delete backup;
    // 	backup = nullptr;
    // }
    // return 0;

    Settlement S1("BeerSheva", SettlementType::CITY);
    Facility F1("University","BeerSheva", FacilityCategory::ECONOMY, 1, 2, 3, 4);
    cout << S1.toString() << endl;
    cout << F1.toString() << endl;
    return 0;
}