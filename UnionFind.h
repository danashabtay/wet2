//
// Created by user on 25/12/2022.
//

#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H
#include "HashTable.h"
#include "player.h"
#include "team.h"
#include <memory>


class UnionFind;

class playerNode;

class UnionFind {

    class teamNode{
    public:
        std::shared_ptr<team> data;
        std::shared_ptr<playerNode> rep;
        int rank;
        permutation_t team_spirit;

        teamNode(){
            data=nullptr;
            rep= nullptr;
            rank=0;
            team_spirit=permutation_t::neutral();
        }
    };

    class playerNode{
        public:
            std::shared_ptr<player> data;
            std::shared_ptr<playerNode> parent;
            int rg;
            permutation_t rs;
            std::shared_ptr<teamNode> team;
            playerNode& operator=(const playerNode& n){
                data = n.data;
                parent = n.parent;
                rg = n.rg;
                rs = n.rs;
                team = n.team;
            }

    };


    HashTable<playerNode> players_hashTable;
    HashTable<teamNode> teams_hashTable;

    public:
        UnionFind();
        ~UnionFind();
        void destroy();
        void addSinglePlayer(std::shared_ptr<player> newData ,int playerId, int teamId);
        void addTeam(int teamId, std::shared_ptr<team> newData);
        void UniteTeams(int owner, int added);
        std::shared_ptr<playerNode> find(std::shared_ptr<playerNode> data);
        int findNumGames(int playerId);
        permutation_t findSpirit(int playerId);
        bool isActive(int playerId);
        void markDeleted(int teamId);
        bool doesExist(int playerId);
        void addGame(int teamId1, int teamId2);
        std::shared_ptr<player> findById(int playerId);
        permutation_t getPartialSpirit(int playerId);
};


#endif //WET2_UNIONFIND_H
