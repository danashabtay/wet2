//
// Created by 97254 on 25/12/2022.
//

#include "team.h"

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
permutation_t team::getPermutation() const{
    return m_team_spirit;
}