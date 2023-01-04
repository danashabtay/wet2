//
// Created by user on 03/01/2023.
//

#ifndef WET2TEST_WORLDCUPMANAGER_H
#define WET2TEST_WORLDCUPMANAGER_H

#include "player.h"
#include "team.h"

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
        explicit playerNode();
        ~playerNode() = default;
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
        explicit teamNode();
        ~teamNode() = default;
        friend class WorldCupManager;
        friend class playerNode;
    };
    teamNode** teams_table;
    playerNode** players_table;
    int max_size_team;
    int curr_size_team;
    int max_size_player;
    int curr_size_player;
    WorldCupManager::playerNode* findRep(playerNode* player);

public:
    WorldCupManager();
    ~WorldCupManager();
    void AddTeam(int teamId, team* data);
    void AddPlayer(int playerId, player* data, int teamId);
    void insertTeam(teamNode** table, teamNode* newTeam);
    void insertPlayer(playerNode** table, playerNode* newPlayer);
    WorldCupManager::playerNode* FindPlayer(int playerId);
    WorldCupManager::teamNode* FindTeam(int teamId);
    void UniteTeams(int teamId1, int teamId2);
    int findNumGames(int playerId);
    bool isActive(int playerId);
    void markDeleted(int teamId);
    bool doesExist(int playerId);
    void addGame(int teamId1, int teamId2);
    permutation_t getPartialSpirit(int playerId);
};

WorldCupManager::teamNode::teamNode() : m_key(-1) ,m_data(nullptr), m_rep(nullptr), m_rank(0), m_team_spirit(permutation_t::neutral()), m_isDeleted(
        false) {}

WorldCupManager::playerNode::playerNode() : m_key(-1) ,m_data(nullptr), m_parent(nullptr), m_rg(0), m_rs(permutation_t::neutral()), m_team(
        nullptr) {}


#endif //WET2TEST_WORLDCUPMANAGER_H

