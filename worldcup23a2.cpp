#include "worldcup23a2.h"

////
world_cup_t::world_cup_t() {
    m_num_teams = 0;
    m_all_teams_id = new AVLTree<int, team>; //its an avl tree
    m_all_teams_ability = new AVLRankTree<int, int>();
    m_game = new WorldCupManager();
}

world_cup_t::~world_cup_t() {
    m_all_teams_id->Reset();
    m_all_teams_ability->Reset();
}

StatusType world_cup_t::add_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    team *tmp_id = m_all_teams_id->getByKey(teamId);
    if (tmp_id != nullptr) {
        tmp_id = nullptr;
        return StatusType::FAILURE;
    }
    team *team1 = new team(teamId);
    m_all_teams_id->Insert(teamId,team1);
    m_all_teams_ability->Insert(0, teamId);
    m_game->AddTeam(teamId, team1);
    tmp_id = nullptr;
    m_num_teams++;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    team *team1 = m_all_teams_id->getByKey(teamId);
    if (team1 == nullptr) {
        return StatusType::FAILURE;
    }
    //remove from id all trees:
    m_all_teams_id->Remove(teamId);
    int ability = team1->getTeamAbility();
    m_all_teams_ability->Remove(ability, teamId);
    // mark team as deleted in teams hashtable:
    m_game->markDeleted(teamId);
    team1 = nullptr;
    m_num_teams--;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper) {
    if (playerId <= 0 || teamId <= 0 || !spirit.isvalid() || gamesPlayed < 0 || cards < 0) {
        return StatusType::INVALID_INPUT;
    }

    team *team1 = m_all_teams_id->getByKey(teamId);
    if (team1 == nullptr) {
        //delete team1;
        return StatusType::FAILURE;
    }
    if (m_game->doesExist(playerId)) {
        //delete team1;
        return StatusType::FAILURE;
    }

    player *newPlayer = new player(playerId, cards, gamesPlayed, ability, spirit, teamId, goalKeeper);
    if (newPlayer == nullptr) {
        return StatusType::ALLOCATION_ERROR;
    }
    int old_ability = team1->getTeamAbility();
    m_all_teams_ability->Remove(old_ability, teamId);
    team1->addPlayerStats(ability, spirit, goalKeeper);
    m_game->AddPlayer(playerId, newPlayer, teamId);
    int new_ability = team1->getTeamAbility();
    m_all_teams_ability->Insert(new_ability, teamId);
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    team *team1 = m_all_teams_id->getByKey(teamId1);
    if (team1 == nullptr || !team1->hasKeeper()) {
        output_t<int> out(StatusType::FAILURE);
        return out;
    }

    team *team2 = m_all_teams_id->getByKey(teamId2);
    if (team2 == nullptr || !team2->hasKeeper()) {

        output_t<int> out(StatusType::FAILURE);
        return out;
    }


    int total_power_team1 = team1->getPlayMatchStats();
    int total_power_team2 = team2->getPlayMatchStats();
    m_game->addGame(teamId1, teamId2);

    if (total_power_team1 > total_power_team2) {
        team1->addPoints(3);
        output_t<int> out(1);

        return out;
    } else if (total_power_team2 > total_power_team1) {
        team2->addPoints(3);
        output_t<int> out(3);

        return out;
    } else {
        int team1_strength = team1->getPermutation().strength();
        int team2_strength = team2->getPermutation().strength();
        if (team1_strength > team2_strength) {
            team1->addPoints(3);
            output_t<int> out(2);

            return out;
        } else if (team2_strength > team1_strength) {
            team2->addPoints(3);
            output_t<int> out(4);

            return out;
        } else {
            team1->addPoints(1);
            team2->addPoints(1);
            output_t<int> out(0);

            return out;
        }
    }

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId) {
    if (playerId <= 0) {
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    if (!m_game->doesExist(playerId)) {
        return StatusType::FAILURE;
    }
    int numGames = m_game->findNumGames(playerId);
    return numGames;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards) {
    if (playerId <= 0 || cards < 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!m_game->doesExist(playerId)) {
        return StatusType::FAILURE;
    }
    if (!m_game->isActive(playerId)) {
        return StatusType::FAILURE;
    }
    player *player1 = m_game->findPlayer(playerId);
    player1->addCards(cards);
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId) {
    if (playerId <= 0) {
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    if (!m_game->doesExist(playerId)) {
        return StatusType::FAILURE;
    }
    player *player1 = m_game->findPlayer(playerId);
    int cards = player1->getCards();
    output_t<int> out(cards);
    return out;
}

output_t<int> world_cup_t::get_team_points(int teamId) {
    if (teamId <= 0) {
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    team *team1 = m_all_teams_id->getByKey(teamId);
    if (team1 == nullptr) {
        output_t<int> out(StatusType::FAILURE);
        return out;
    }
    output_t<int> out(team1->getNumPoints());
    return out;
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i) {
    if (m_num_teams <= 0 || i < 0 || i >= m_num_teams) {
        output_t<int> out(StatusType::FAILURE);
        return out;
    }
    int teamId = m_all_teams_ability->getByRank(i);
    output_t<int> out(teamId);
    return out;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId) {
    if (playerId <= 0) {
        output_t<permutation_t> out(StatusType::INVALID_INPUT);
        return out;
    }
    if (!m_game->doesExist(playerId)) {
        output_t<permutation_t> out(StatusType::FAILURE);
        return out;
    }
    if (!m_game->isActive(playerId)) {
        output_t<permutation_t> out(StatusType::FAILURE);
        return out;
    }
    output_t<permutation_t> out(m_game->getPartialSpirit(playerId));
    return out;
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }
    team *team1 = m_all_teams_id->getByKey(teamId1);
    if (team1 == nullptr) {
        return StatusType::FAILURE;
    }
    team *team2 = m_all_teams_id->getByKey(teamId2);
    if (team2 == nullptr) {
        return StatusType::FAILURE;
    }
    m_game->UniteTeams(teamId1, teamId2);
    remove_team(teamId2);
    return StatusType::SUCCESS;
};
