#include <vector>
#include <iostream>
#include "Continent.h"
#include "Country.h"

Continent::Continent(std::string name, int controlValue) {
    name_ = name;
    countries_ = {};
    controlValue_ = controlValue;
}

Continent::~Continent() {
    for (Country* country : countries_) {
        std::vector<Country*> connectionsCopy(country->GetConnections());
        for (Country* connection : connectionsCopy) {
            Country::Disconnect(country, connection);
        }
    }

    for (Country* country : countries_) {
        delete country;
    }
    countries_.clear();
}

void Continent::AddCountry(Country* country) {
    if (std::find(countries_.begin(), countries_.end(), country) == countries_.end()) {
        countries_.push_back(country);
        country->SetContinent(this);
    }
}

void Continent::RemoveCountry(Country* country) {
    countries_.erase(std::find(countries_.begin(), countries_.end(), country));
    delete country;
}

bool Continent::HasCountry(Country* country) {
    return std::find(countries_.begin(), countries_.end(), country) != countries_.end();
}

bool Continent::IsPlayerControllingContinent(Player* player) {
    for (Country* country : countries_) {
        if (country->GetOwner() != player) {
            return false;
        }
    }
    return true;
}

void Continent::ToString() {
    int SIZE = (int)countries_.size();
    std::cout << GetName() << " includes the following countries: " << std::endl;
    for (int i = 0; i < SIZE; i++) {
        std::cout << countries_[i]->GetName() << std::endl;
    }
}
