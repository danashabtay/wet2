//
// Created by 97254 on 25/12/2022.
//

#include "player.h"

permutation_t player::getSpirit(){
    return m_spirit;
}

int player::getNumGames(){
    return m_games_played;
}

int player::addCards(int sum){
    m_cards+=sum;
    return;
}