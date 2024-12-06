#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

enum class FacilityStatus {
    UNDER_CONSTRUCTIONS,
    OPERATIONAL,
};

enum class FacilityCategory {
    LIFE_QUALITY,
    ECONOMY,
    ENVIRONMENT,
};


class FacilityType {
    public:
        FacilityType();
        FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score);
        FacilityType(const FacilityType &other);
        const string &getName() const;
        int getCost() const;
        int getLifeQualityScore() const;
        int getEnvironmentScore() const;
        int getEconomyScore() const;
        FacilityCategory getCategory() const;
        string toStringFacCat(const FacilityCategory category) const; //convert enum categry to string
        string toStringFacStat(const FacilityStatus status) const; //convert enum status to string

    protected:
        const string name;
        const FacilityCategory category;
        const int price;
        const int lifeQuality_score;
        const int economy_score;
        const int environment_score;
};



class Facility: public FacilityType {

    public:
        Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score);
        Facility(const FacilityType &type, const string &settlementName);
        Facility(const Facility &other);
        Facility();
        const string &getSettlementName() const;
        const int getTimeLeft() const;
        const FacilityStatus& getStatus() const;
        FacilityStatus step();
        void setStatus(FacilityStatus status);
        const string toString() const;

    private:
        const string settlementName;
        FacilityStatus status;
        int timeLeft;
};