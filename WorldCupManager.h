//
// Created by user on 03/01/2023.
//

#ifndef WET2TEST_WORLDCUPMANAGER_H
#define WET2TEST_WORLDCUPMANAGER_H

#include "player.h"
#include "team.h"
#include "HashTable.h"

class WorldCupManager {
    class teamNode;
    class playerNode{
        int m_key;
        player* m_data;
        playerNode* m_parent;
        int m_rg;
        permutation_t m_rs;
        teamNode* m_team;
    public:
        explicit playerNode() : m_key(-1) ,m_data(NULL), m_parent(NULL), m_rg(0), m_rs(permutation_t::neutral()), m_team(
                NULL) {}
        ~playerNode() = default;
        int getKey(){
            return m_key;
        }
        friend class WorldCupManager;
        friend class teamNode;

    };

    class teamNode{
        int m_key;
        team* m_data;
        playerNode* m_rep;
        int m_rank;
        permutation_t m_team_spirit;
        bool m_isDeleted;
    public:
        explicit teamNode() : m_key(-1) ,m_data(NULL), m_rep(NULL), m_rank(0), m_team_spirit(permutation_t::neutral()), m_isDeleted(
                false) {}
        ~teamNode() = default;
        int getKey(){
            return m_key;
        }
        friend class WorldCupManager;
        friend class playerNode;
    };
    HashTable<teamNode> teams_table;
    HashTable<playerNode> players_table;
    WorldCupManager::playerNode* findRep(playerNode* player);

public:
    explicit WorldCupManager();
    ~WorldCupManager();
    void AddTeam(int teamId, team* data);
    void AddPlayer(int playerId, player* data, int teamId);
    WorldCupManager::playerNode* FindPlayer(int playerId);
    player* findPlayer(int playerId);
    WorldCupManager::teamNode* FindTeam(int teamId);
    void UniteTeams(int teamId1, int teamId2);
    int findNumGames(int playerId);
    bool isActive(int playerId);
    void markDeleted(int teamId);
    bool doesExist(int playerId);
    void addGame(int teamId1, int teamId2);
    permutation_t getPartialSpirit(int playerId);
};

///
#endif //WET2TEST_WORLDCUPMANAGER_H

