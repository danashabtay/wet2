#include "worldcup23a2.h"

world_cup_t::world_cup_t()
{
	m_num_teams=0;
    m_all_teams_id = AvlTree<team, int>();
    m_all_teams_ability = AVLRankTree<int, int>();
    m_all_eligible_teams = AvlTree<team, int>();
    m_game = UnionFind();
}

world_cup_t::~world_cup_t()
{
    m_all_teams_id.deleteTree(m_all_teams_id.getRoot());
    m_all_teams_ability.Reset();
    m_all_eligible_teams.deleteTree(m_all_eligible_teams.getRoot());
    m_game.destroy();
}

StatusType world_cup_t::add_team(int teamId)
{
    if(teamId<=0){
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<team> tmp_id = m_all_teams_id.find_by_key(teamId);
    if(tmp_id != nullptr){
        tmp_id=nullptr;
        return StatusType::FAILURE;
    }

    std::shared_ptr<team> team1 (new team(teamId));
    m_all_teams_id.insert(team1, teamId);
    //m_all_teams_ability.Insert(teamId, *team1);
    m_all_teams_ability.Insert(0, teamId);
    //m_all_teams_ability.Insert(0, *team1);
    m_game.addTeam(teamId,team1);
    tmp_id=nullptr;
    m_num_teams++;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId<=0){
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<team> team1 = m_all_teams_id.find_by_key(teamId);
    if(team1 == nullptr){
        return StatusType::FAILURE;
    }
    //remove from id all trees:
    m_all_teams_id.remove(teamId);
    int ability=team1->getTeamAbility();
    m_all_teams_ability.Remove(ability,teamId);
    if(team1->hasKeeper()){
        m_all_eligible_teams.remove(teamId);
    }

    // mark team as deleted in teams hashtable:
    m_game.markDeleted(teamId);

    team1 = nullptr;
    m_num_teams--;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if(playerId<=0 || teamId<=0 || !spirit.isvalid() || gamesPlayed<0 || cards<0){
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<team> team1 = m_all_teams_id.find_by_key(teamId);
    if(team1 == nullptr){
        return StatusType::FAILURE;
    }
    if(m_game.doesExist(playerId)){
        return StatusType::FAILURE;
    }

    std::shared_ptr<player> newPlayer (new player(playerId,cards,gamesPlayed,ability,spirit,teamId,goalKeeper));

    int old_ability=team1->getTeamAbility();
    m_all_teams_ability.Remove(old_ability,teamId);

    if(goalKeeper && (!team1->hasKeeper())){
        m_all_eligible_teams.insert(team1,teamId);
    }

    team1->addPlayerStats(ability, spirit, goalKeeper);
    m_game.addSinglePlayer(newPlayer, playerId, teamId);

    int new_ability=team1->getTeamAbility();
    m_all_teams_ability.Insert(new_ability,teamId);

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1<=0 || teamId2<=0 ||teamId1==teamId2){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    std::shared_ptr<team> team1 = m_all_teams_id.find_by_key(teamId1);
    if(team1 == nullptr || !team1->hasKeeper()){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }
    std::shared_ptr<team> team2 = m_all_teams_id.find_by_key(teamId2);
    if(team2 == nullptr || !team2->hasKeeper()){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }

    int total_power_team1 = team1->getNumPoints() + team1->getTeamAbility();
    int total_power_team2 = team2->getNumPoints() + team2->getTeamAbility();

    if(total_power_team1 > total_power_team2){
        team1->addPoints(3);
        /*team1->addGame();
        team2->addGame();*/
        m_game.addGame(teamId1,teamId2);
        output_t<int> out(1);
        return out;
    }
    else if(total_power_team2 > total_power_team1){
        team2->addPoints(3);
/*
        team1->addGame();
        team2->addGame();
*/
        m_game.addGame(teamId1,teamId2);
        output_t<int> out(3);
        return out;
    }
    else{
        int team1_strength = team1->getPermutation().strength();
        int team2_strength = team2->getPermutation().strength();
        if(team1_strength > team2_strength){
            team1->addPoints(3);
            /*team1->addGame();
            team2->addGame();*/
            m_game.addGame(teamId1,teamId2);
            output_t<int> out(2);
            return out;
        }
        else if(team2_strength > team1_strength){
            team2->addPoints(3);
            /*team1->addGame();
            team2->addGame();*/
            m_game.addGame(teamId1,teamId2);
            output_t<int> out(4);
            return out;
        }
        else{
            team1->addPoints(1);
            team2->addPoints(1);
            /*team1->addGame();
            team2->addGame();*/
            m_game.addGame(teamId1,teamId2);
            output_t<int> out(0);
            return out;
        }
    }
	return StatusType::SUCCESS;
}//if the avl is working then this func should work now

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
    if(playerId<=0){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    if(!m_game.doesExist(playerId)){
        return StatusType::FAILURE;
    }
    int numGames=m_game.findNumGames(playerId);
	return numGames;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if(playerId<=0 || cards<0){
        return StatusType::INVALID_INPUT;
    }
    if(m_game.doesExist(playerId)){
        return StatusType::FAILURE;
    }
    if(!m_game.isActive(playerId)){
        return StatusType::FAILURE;
    }
    std::shared_ptr<player> player = m_game.findById(playerId);
    player->addCards(cards);
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if(playerId<=0){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    if(m_game.doesExist(playerId)){
        return StatusType::FAILURE;
    }
    std::shared_ptr<player> player = m_game.findById(playerId);
    int cards = player->getCards();
    output_t<int> out(cards);
    return out;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId<=0){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    std::shared_ptr<team> team1 = m_all_teams_id.find_by_key(teamId);
    if(team1 == nullptr){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }
    output_t<int> out(team1->getNumPoints());
    return out;
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    if(m_num_teams <= 0 || i<0 || i>=m_num_teams){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    if(m_num_teams==0 || i<0 || m_num_teams<=i){
        return StatusType::FAILURE;
    }
    std::shared_ptr<team> team1 = m_all_teams_ability.getByRank(i);
    output_t<int> out(team1->getAbility());
    return out;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if(playerId<=0){
        output_t<permutation_t> out(StatusType::INVALID_INPUT);
        return out;
    }
    if(m_game.doesExist(playerId)){
        return StatusType::FAILURE;
    }
    if(!m_game.isActive(playerId)){
        return StatusType::FAILURE;
    }

    output_t<permutation_t> out(m_game.getPartialSpirit(playerId));
    return out;
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    if(teamId1<=0 || teamId2<=0 || teamId1 == teamId2){
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<team> team1 = m_all_teams_id.find_by_key(teamId1);
    if(team1 == nullptr){
        return StatusType::FAILURE;
    }
    std::shared_ptr<team> team2 = m_all_teams_id.find_by_key(teamId2);
    if(team2 == nullptr){
        return StatusType::FAILURE;
    }
    m_game.UniteTeams(teamId1,teamId2);
    remove_team(teamId2);
    return StatusType::SUCCESS;
};
