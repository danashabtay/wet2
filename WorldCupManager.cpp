/////
// Created by user on 03/01/2023.
//
#include <iostream>

#define TOTAL 2

#include "WorldCupManager.h"

WorldCupManager::WorldCupManager() : teams_table(new HashTable<teamNode>()),
                                     players_table(new HashTable<playerNode>()) {
}

WorldCupManager::~WorldCupManager() = default;

WorldCupManager::playerNode *WorldCupManager::findRep(WorldCupManager::playerNode *player) {
    while (player->m_parent != player) {
        player = player->m_parent;
    }
    return player;
}

void WorldCupManager::AddTeam(int teamId, team *data) {
    teamNode *newTeam = new teamNode();
    newTeam->m_key = teamId;
    newTeam->m_data = data;
    newTeam->m_isDeleted = false;
    teams_table->insert(newTeam, newTeam->m_key);
}

void WorldCupManager::AddPlayer(int playerId, player *data, int teamId) {
    playerNode *newPlayer = new playerNode();
    newPlayer->m_key = playerId;
    newPlayer->m_data = data;
    ///////////////find team
    teamNode *team1 = FindTeam(teamId);
    //////////////////////////
    if (team1->m_rank == 0) {
        newPlayer->m_team = team1;
        newPlayer->m_parent = newPlayer;
        newPlayer->m_rg = data->getNumGames();
        newPlayer->m_rs = data->getSpirit();
        team1->m_rep = newPlayer;
    } else {
        newPlayer->m_parent = team1->m_rep;
        newPlayer->m_rg = data->getNumGames() - (newPlayer->m_parent->m_rg);
        newPlayer->m_rs = (newPlayer->m_parent->m_rs.inv()) * (team1->m_team_spirit) * data->getSpirit();
    }
    team1->m_team_spirit = team1->m_team_spirit*data->getSpirit();
    team1->m_rank++;
    players_table->insert(newPlayer, newPlayer->m_key);
}

WorldCupManager::playerNode *WorldCupManager::FindPlayer(int playerId) {
    playerNode* player1 = players_table->find(playerId);
    if(player1== nullptr){
        return nullptr;
    }
    playerNode* playerParent= findRep(player1);
    if(player1->m_parent==playerParent){
        return player1;
    }
    playerNode* og_player=player1;
    permutation_t spirit_sum= permutation_t::neutral();
    int game_sum=0;
    while(player1!=playerParent){
        spirit_sum= (player1->m_rs) * spirit_sum;
        game_sum+=player1->m_rg;
        player1=player1->m_parent;
    }
    int partial_games=0;
    permutation_t partial_spirit=permutation_t::neutral();
    while(og_player->m_parent!=playerParent){
        playerNode* next=og_player->m_parent;
        og_player->m_parent=playerParent;
        permutation_t tmp_spirit=og_player->m_rs;
        int tmp_games=og_player->m_rg;
        og_player->m_rs=spirit_sum * (partial_spirit.inv());
        og_player->m_rg=game_sum-partial_games;
        partial_spirit=tmp_spirit*partial_spirit;
        partial_games+=tmp_games;
        og_player=next;
    }
    return players_table->find(playerId);
}

permutation_t WorldCupManager::getPartialSpirit(int playerId) {
    playerNode *player1 = FindPlayer(playerId);
    playerNode *playerParent = findRep(player1);
    permutation_t sum = permutation_t::neutral();
    if (player1 == playerParent) {
        return player1->m_rs;
    }
    while (player1 != playerParent) {
        sum = (player1->m_rs) * sum;
        player1 = player1->m_parent;
    }
    sum = (player1->m_rs) * sum;
    return sum;
}

WorldCupManager::teamNode *WorldCupManager::FindTeam(int teamId) {
    return teams_table->find(teamId);
}

void WorldCupManager::UniteTeams(int teamId1, int teamId2) {
    teamNode *team1 = FindTeam(teamId1);
    teamNode *team2 = FindTeam(teamId2);
    playerNode *ownerParent = team1->m_rep;
    playerNode *addedParent = team2->m_rep;

    if (ownerParent == addedParent && addedParent == nullptr) {
        //both teams are empty
        return;
    }
    if (ownerParent == nullptr) {
        //buying team is empty
        team1->m_team_spirit=team2->m_team_spirit;
        team1->m_rank=team2->m_rank;
        team1->m_rep=team2->m_rep;
        team1->m_rep->m_team=team1;
        return;
    } else if (addedParent == nullptr) {
        //bought team is empty
        return;
    }
    if (ownerParent->m_data == addedParent->m_data) {
        //if they are part of same set do nothing
        return;
    }
    //else whoever rank is higher becomes parent of other
    if (ownerParent->m_team->m_rank >= addedParent->m_team->m_rank) { // add added team to owner team
        /// change rank
        ownerParent->m_team->m_rank += addedParent->m_team->m_rank;
        ///change rs
        addedParent->m_rs = (ownerParent->m_rs.inv()) * (ownerParent->m_team->m_team_spirit) * (addedParent->m_rs);
        ownerParent->m_team->m_team_spirit=(ownerParent->m_team->m_team_spirit)*(addedParent->m_team->m_team_spirit);
        ///change rg
        addedParent->m_rg = addedParent->m_rg - ownerParent->m_rg;
        addedParent->m_parent = ownerParent;
    } else { // add owner team to added team
        team1->m_rep = team2->m_rep;
        addedParent->m_team = ownerParent->m_team;
        /// change rank
        addedParent->m_team->m_rank += ownerParent->m_team->m_rank;
        ///change rs
        addedParent->m_rs = (ownerParent->m_team->m_team_spirit) * addedParent->m_rs;
        ownerParent->m_rs = (addedParent->m_rs.inv()) * ownerParent->m_rs;
        ownerParent->m_team->m_team_spirit=(ownerParent->m_team->m_team_spirit)*(addedParent->m_team->m_team_spirit);
        ///change rg
        ownerParent->m_rg = ownerParent->m_rg - addedParent->m_rg;
        ownerParent->m_parent = addedParent;
    }
}

int WorldCupManager::findNumGames(int playerId) {
    playerNode *player1 = FindPlayer(playerId);
    playerNode *playerParent = findRep(player1);
    int sum = player1->m_rg;
    while (player1 != playerParent) {
        sum += playerParent->m_rg;
        player1 = player1->m_parent;
    }
    return sum;
}

bool WorldCupManager::isActive(int playerId) {
    playerNode *player1 = FindPlayer(playerId);
    playerNode *playerParent = findRep(player1);
    teamNode *team1 = playerParent->m_team;
    return !team1->m_isDeleted;
}

void WorldCupManager::markDeleted(int teamId) {
    teamNode *team1 = FindTeam(teamId);
    team1->m_isDeleted = true;
    team1->m_key = -1;
}

bool WorldCupManager::doesExist(int playerId) {
    playerNode *player1 = FindPlayer(playerId);
    if (player1 == NULL) {
        return false;
    }
    return true;
}

void WorldCupManager::addGame(int teamId1, int teamId2) {
    teamNode *team1 = FindTeam(teamId1);
    teamNode *team2 = FindTeam(teamId2);
    playerNode *rep1 = team1->m_rep;
    playerNode *rep2 = team2->m_rep;
    rep1->m_rg++;
    rep2->m_rg++;
}

player *WorldCupManager::findPlayer(int playerId) {
    playerNode *node = FindPlayer(playerId);
    if (node) {
        return node->m_data;
    }
    return NULL;
}