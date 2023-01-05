//
// Created by 97254 on 25/12/2022.
///

#include "team.h"
#include "iostream"


permutation_t team::getPermutation() const {
    return m_team_spirit;
}


bool team::hasKeeper() {
    if(m_has_goalkeeper== true){
        return true;
    }
    return false;
}

int team::getNumPoints() const{
    if(m_has_goalkeeper== 0){
        return 0;
    }
    return m_points;
}

int team::getTeamAbility() const{
    return m_total_ability;
}

void team::addPoints(int points_to_add){
    m_points += points_to_add;
}


int team::getId() const{
    return m_team_id;
}


int team::getAbility() const{
    return m_total_ability;
}


void team::addPlayerStats(int ability, permutation_t spirit, bool isKeeper){
    m_team_spirit = m_team_spirit*spirit;//todo:figure out how to handle with no operator=
    m_total_ability+=ability;
    if(isKeeper){
        m_has_goalkeeper=true;
    }
}

int team::getPlayMatchStats() {
    int sum = m_points+m_total_ability;
    std::cout<<"sum: "<<sum<<"\n";
    return sum;
}
