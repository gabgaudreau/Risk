#include <cmath>
#include <algorithm>
#include <iostream>
#include "ReinforcementPhase.h"
#include "Player.h"
#include "PlayerList.h"
#include "World.h"
#include "Country.h"
#include "Continent.h"
#include "RiskGame.h"

ReinforcementPhase::ReinforcementPhase(World* world, PlayerList* players)
    : Phase(world, players) {
}

void ReinforcementPhase::Start() {
    std::cout << "\t| Reinforcement Phase" << std::endl;
    placeReinforcements(calculateReinforcements() + turnInCards());
}

int ReinforcementPhase::calculateReinforcements() {
    int controlledContinentSummedValues = 0;
    for (Continent* continent : world_->GetContinents()) {
        if (continent->IsPlayerControllingContinent(players_->Current())) {
            controlledContinentSummedValues += continent->GetControlValue();
        }
    }
    return (int)std::max(3.0, floor(players_->Current()->GetOwnedCountries().size() / 3.0)
                         + controlledContinentSummedValues);
}

void ReinforcementPhase::placeReinforcements(int reinforcements) {
    const std::vector<Country*>& ownedCountries = players_->Current()->GetOwnedCountries();
    for (size_t i = 0; i < ownedCountries.size(); ++i) {
        std::cout << "\t\t" << i << ". " << ownedCountries.at(i)->GetName() << " : " << ownedCountries.at(i)->GetArmy() << " armies" << std::endl;
    }

    while (reinforcements > 0) {
        std::cout << "Enter the country index that you wish to reinforce: " << std::endl;
        int inputIndex = -1;
        while (inputIndex < 0 || inputIndex >= (int)ownedCountries.size()) {
            std::cout << "> ";
            std::cin >> inputIndex;
        }

        std::cout << "Enter the # of armies to send (" << reinforcements << " left): " << std::endl;
        int inputArmies = -1;
        while (inputArmies <= 0 || inputArmies > reinforcements) {
            std::cout << "> ";
            std::cin >> inputArmies;
        }

        reinforcements -= inputArmies;
        RiskGame::Instance().GetWorld()->IncrementCountryArmy(ownedCountries.at(inputIndex), inputArmies);

        std::cout << "You have placed " << inputArmies << " armies in " << ownedCountries.at(inputIndex)->GetName() << "!" << std::endl;
    }
}

int ReinforcementPhase::turnInCards() {
    int numOfCards = players_->Current()->GetCardCount();
    if (numOfCards > 4) {
        std::cout << "You have more than 4 cards and are thereby forced to turn in cards." << std::endl;
        return players_->Current()->TurnIn();
    }
    else if (numOfCards > 2 && players_->Current()->EligibleForTurnIn()) {
        std::string reply;
        std::cout << "Would you like to turn in cards? (y/n) " << std::endl;
        std::cin >> reply;
        if(reply == "y")
            return players_->Current()->TurnIn();
    }
    return 0;
}
