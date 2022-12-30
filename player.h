//
// Created by 97254 on 25/12/2022.
//

#ifndef WET2_PLAYER_H
#define WET2_PLAYER_H
#include "wet2util.h"

class player {
    private:
        int m_player_id;
        int m_cards;
        int m_games_played;
        int m_ability;
        const permutation_t& m_spirit;
        int m_team_id;
        bool m_isGoalKeeper;

public:
player(int playerId, int cards, int gamesPlayed, int ability, const permutation_t& spirit, int teamId, bool goalKeeper)
: m_player_id(playerId), m_cards(cards), m_games_played(gamesPlayed), m_ability(ability), m_spirit(spirit),
m_team_id(teamId), m_isGoalKeeper(goalKeeper) {}

permutation_t getSpirit():
int getNumGames();
void addCards(int sum);
int getCards();

};


#endif //WET2_PLAYER_H
