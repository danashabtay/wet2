//
// Created by user on 03/01/2023.
//
#include <iostream>
#define TOTAL 2
#include "WorldCupManager.h"

WorldCupManager::WorldCupManager() : teams_table(new teamNode*[TOTAL*sizeof(teamNode*)]), players_table(new playerNode*[TOTAL*sizeof(playerNode*)]), max_size_team(TOTAL),max_size_player(TOTAL), curr_size_player(0), curr_size_team(0) {
    for(int i = 0; i < TOTAL; i++){
        teams_table[i] = nullptr;
        players_table[i] = nullptr;
    }
}

WorldCupManager::~WorldCupManager() {
    for(int i = 0; i < max_size_team; i++){
        delete teams_table[i];
    }
    for(int i = 0; i < max_size_player; i++){
        delete players_table[i];
    }
    delete [] teams_table;
    delete [] players_table;
}

WorldCupManager::playerNode *WorldCupManager::findRep(WorldCupManager::playerNode *player) {
    while(player->m_parent!=player){
        player=player->m_parent;
    }
    return player;
}

void WorldCupManager::AddTeam(int teamId, team* data) {
    if (curr_size_team == max_size_team) {
        // No more place in array so make it bigger
        max_size_team *= 2;
        teamNode **new_team_table = new teamNode *[max_size_team * sizeof(teamNode*)];
        for(int i = 0; i < max_size_team; i++) {
            new_team_table[i] = nullptr;
            if (teams_table[i]!= nullptr) {
                insertTeam(new_team_table, teams_table[i]);
            }
        }
        delete[] teams_table;
        teams_table = new_team_table;
    }

    teamNode *newTeam = new teamNode();
    newTeam->m_key=teamId;
    newTeam->m_data=data;
    insertTeam(teams_table,newTeam);
    curr_size_team++;
}

void WorldCupManager::AddPlayer(int playerId, player* data, int teamId) {
    if (curr_size_player == max_size_player) {
        // No more place in array so make it bigger
        max_size_player *= 2;
        playerNode **new_player_table = new playerNode *[max_size_player * sizeof(playerNode*)];
        for(int i = 0; i < max_size_team; i++) {
            new_player_table[i] = nullptr;
            if (teams_table[i]!= nullptr) {
                insertPlayer(new_player_table, players_table[i]);
            }
        }
        delete[] players_table;
        players_table = new_player_table;
    }

    playerNode *newPlayer = new playerNode();
    newPlayer->m_key=playerId;
    newPlayer->m_data=data;

    teamNode* team1 = FindTeam(teamId);
    if(team1->m_rep== nullptr){
        newPlayer->m_team=team1;
        newPlayer->m_parent=newPlayer;
        newPlayer->m_rg=data->getNumGames();
        newPlayer->m_rs=data->getSpirit();
    }
    else{
        newPlayer->m_parent=team1->m_rep;
        newPlayer->m_rg=data->getNumGames()-(newPlayer->m_parent->m_rg);
        newPlayer->m_rs=(newPlayer->m_parent->m_rs.inv())*(team1->m_team_spirit)*data->getSpirit();
    }
    team1->m_team_spirit=team1->m_team_spirit*data->getSpirit();
    team1->m_rank++;
    insertPlayer(players_table,newPlayer);
    curr_size_player++;
}

WorldCupManager::playerNode *WorldCupManager::FindPlayer(int playerId) {
    int key=playerId;
    int counter = 0;
    int hashIndex = ( key%max_size_player + (counter * 1 + key%(max_size_player-1)) );
    while (players_table[hashIndex] != nullptr) {
        if (counter++ > max_size_player)
            return nullptr;
        // if node found return its value
        if (players_table[hashIndex]->m_key == key)
            return players_table[hashIndex];
        counter++;
        hashIndex =( key%max_size_team + (counter * 1 + key%(max_size_team-1)) );
    }
    return nullptr;
}

permutation_t WorldCupManager::getPartialSpirit(int playerId) {
playerNode* player1= FindPlayer(playerId);
playerNode* playerParent = findRep(player1);
    permutation_t sum= permutation_t::neutral();
    while(player1!=playerParent){
        sum=(playerParent->m_rs)*sum;
        player1=player1->m_parent;
    }
    return sum;
}

WorldCupManager::teamNode *WorldCupManager::FindTeam(int teamId) {
    int key=teamId;
    int counter = 0;

    int hashIndex = ( key%max_size_team + (counter * 1 + key%(max_size_team-1)) );

    while (teams_table[hashIndex] != nullptr) {
        if (counter++ > max_size_team)
            return nullptr;
        // if node found return its value
        if (teams_table[hashIndex]->m_key == key)
            return teams_table[hashIndex];
        counter++;
        hashIndex =( key%max_size_team + (counter * 1 + key%(max_size_team-1)) );
    }
    return nullptr;
}

void WorldCupManager::UniteTeams(int teamId1, int teamId2) {
teamNode* team1 = FindTeam(teamId1);
teamNode* team2 = FindTeam(teamId2);
playerNode* ownerParent = team1->m_rep;
playerNode* addedParent = team2->m_rep;
    //if they are part of same set do nothing
if(ownerParent->m_data==addedParent->m_data){
    return;
}
    //else whoever rank is higher becomes parent of other
    if (ownerParent->m_team->m_rank >= addedParent->m_team->m_rank) { // add added team to owner team
        markDeleted(teamId2);
        /// change rank
        ownerParent->m_team->m_rank+=addedParent->m_team->m_rank;
        ///change rs
        addedParent->m_rs=(ownerParent->m_rs.inv())*(ownerParent->m_team->m_team_spirit)*(addedParent->m_rs);
        ///change rg
        addedParent->m_rg=addedParent->m_rg-ownerParent->m_rg;
        addedParent->m_parent = ownerParent;
    } else { // add owner team to added team
        team1->m_rep=team2->m_rep;
        addedParent->m_team=ownerParent->m_team;
        markDeleted(teamId2);
        /// change rank
        addedParent->m_team->m_rank+=ownerParent->m_team->m_rank;
        ///change rs
        permutation_t temp_b = addedParent->m_rs;
        ownerParent->m_rs=(temp_b.inv())*ownerParent->m_rs;
        addedParent->m_rs=temp_b*(ownerParent->m_team->m_team_spirit);
        ///change rg
        ownerParent->m_rg=ownerParent->m_rg-addedParent->m_rg;
        ownerParent->m_parent = addedParent;
    }
}

int WorldCupManager::findNumGames(int playerId) {
playerNode* player1 = FindPlayer(playerId);
playerNode* playerParent = findRep(player1);
    int sum=0;
    while(player1!=playerParent){
        sum+=player1->m_rg;
        player1=player1->m_parent;
    }
    return sum;
}

bool WorldCupManager::isActive(int playerId) {
    playerNode* player1 = FindPlayer(playerId);
    playerNode* playerParent = findRep(player1);
    teamNode* team1 = playerParent->m_team;
    return team1->m_isDeleted;
}

void WorldCupManager::markDeleted(int teamId) {
teamNode* team1 = FindTeam(teamId);
team1->m_isDeleted= true;
}

bool WorldCupManager::doesExist(int playerId) {
    playerNode* player1 = FindPlayer(playerId);
    if(player1== nullptr){
        return true;
    }
    return false;
}

void WorldCupManager::addGame(int teamId1, int teamId2) {
    teamNode* team1 = FindTeam(teamId1);
    teamNode* team2 = FindTeam(teamId2);
    playerNode* rep1 = team1->m_rep;
    playerNode* rep2 = team2->m_rep;
    rep1->m_rg++;
    rep2->m_rg++;
}

void WorldCupManager::insertTeam(teamNode** table, teamNode* newTeam) {
    int key=newTeam->m_key;
    int counter=0;
    int hashIndex = ( key%max_size_team + (counter * 1 + key%(max_size_team-1)) );
    // find next free space
    while (table[hashIndex] != nullptr && table[hashIndex]->m_key != -1) {
        counter++;
        hashIndex =( key%max_size_team + (counter * 1 + key%(max_size_team-1)) );
    }
    table[hashIndex] = newTeam;
}

void WorldCupManager::insertPlayer(playerNode** table, playerNode *newPlayer) {
    int key=newPlayer->m_key;
    int counter=0;
    int hashIndex = ( key%max_size_player + (counter * 1 + key%(max_size_player-1)) );
    // find next free space
    while (table[hashIndex] != nullptr && table[hashIndex]->m_key != -1) {
        counter++;
        hashIndex =( key%max_size_player + (counter * 1 + key%(max_size_player-1)) );
    }
    table[hashIndex] = newPlayer;
}

player *WorldCupManager::findPlayer(int playerId) {
    playerNode* node = FindPlayer(playerId);
    if(node){
        return node->m_data;
    }
    return nullptr;
}
