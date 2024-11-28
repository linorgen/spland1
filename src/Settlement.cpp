#include "../include/Settlement.h"
#include <iostream>
using namespace std;


Settlement::Settlement(const string &name, SettlementType type): name(name), type(type){
    cout<<"construtor"<<endl; };

const string &Settlement::getName() const{
    return this->name; };
SettlementType Settlement::getType() const{
    return type; };

const int Settlement::getLimit() const{
    if(type == SettlementType::VILLAGE){
        return 1;}
    else if(type == SettlementType::CITY){
        return 2;}
    else if(type == SettlementType::METROPOLIS){
        return 3;}
    return -1;
}

const string Settlement::toString() const{
    return name + " " + to_string(static_cast<int>(type)); };



    