///
// Created by 97254 on 25/12/2022.
//

#include "player.h"

permutation_t player::getSpirit(){
    return m_spirit;
}

int player::getNumGames() const{
    return m_games_played;
}

void player::addCards(int sum){
    m_cards+=sum;
}

int player::getCards() const{
    return m_cards;
}

void player::addGame(){
    m_games_played++;
}
