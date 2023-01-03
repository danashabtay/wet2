//
// Created by 97254 on 25/12/2022.
//

#include "team.h"

permutation_t team::getPermutation() const {
    return m_team_spirit;
}


bool team::hasKeeper() const{
    return m_has_goalkeeper;
}

int team::getNumPoints() const{
    return m_points;
}

int team::getTeamAbility() const{
    return m_total_ability;
}

void team::addPoints(int points_to_add){
    m_points += points_to_add;
}

void team::addGame(){
    m_games_played++;
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