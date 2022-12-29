//
// Created by user on 25/12/2022.
//

#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H
#include "HashTable.h"
#include "player.h"
#include "team.h"

class UnionFind {
class playerNode{
    std::shared_ptr<player> data;
    playerNode* parent; ///unique ptr?
    int rg;
    permutation_t rs;
    teamNode* team; ///unique ptr?
};

class teamNode{
    std::shared_ptr<team> data;
    std::shared_ptr<player> rep;
    int rank;
};

HashTable<playerNode> players_hashTable;
HashTable<teamNode> teams_hashTable;

public:
    UnionFind();
    ~UnionFind();
    void addSinglePlayer(shared_ptr<player> newData ,int playerId, int teamId);
    void addTeam(shared_ptr<team> newData);
    void UniteTeams(int id1, int id2));
    Node find(Node data);
    int findNumGames(int playerId);
    permutation_t findSpirit(int playerId);
    bool isActive(int playerId);
};


#endif //WET2_UNIONFIND_H
