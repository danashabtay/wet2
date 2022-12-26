#include "worldcup23a2.h"

world_cup_t::world_cup_t()
{
	m_num_teams=0;
    m_all_teams_id = AvlTree<team, int>();
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}

StatusType world_cup_t::add_team(int teamId)
{
    if(teamId<=0){
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<team> tmp = m_all_teams.find_by_key(teamId);
    if(tmp != nullptr){
        tmp=nullptr;
        return StatusType::FAILURE;
    }

    std::shared_ptr<team> team1 (new team(teamId));
    m_all_teams.insert(team1, teamId);
    tmp = nullptr;
    m_num_teams++;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId<=0){
        return StatusType::INVALID_INPUT;
    }
	// TODO: Your code goes here
	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if(playerId<=0 || teamId<=0 || !spirit.isvalid() || gamesPlayed<0 || cards<0){
        return StatusType::INVALID_INPUT;
    }

	// TODO: Your code goes here
	return StatusType::SUCCESS;
}
//updated
output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1<=0 || teamId2<=0 ||teamId1==teamId2){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    std::shared_ptr<team> team1 = m_all_teams.findByKey(teamId1);
    if(team1 == nullptr || !team1->hasKeeper()){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }
    std::shared_ptr<team> team2 = m_all_teams.findByKey(teamId2);
    if(team2 == nullptr || !team2->hasKeeper()){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }
    int total_power_team1 = team1->getNumPoints() + team1->getTeamAbility();
    int total_power_team2 = team2->getNumPoints() + team2->getTeamAbility();
    if(total_power_team1 > total_power_team2){
        team1->addPoints(3);
        team1->addGame();
        team2->addGame();
        output_t<int> out(1);
        return out;
    }
    else if(total_power_team2 > total_power_team1){
        team2->addPoints(3);
        team1->addGame();
        team2->addGame();
        output_t<int> out(3);
        return out;
    }
    else{
        int team1_strength = team1->getPermutation().strength();
        int team2_strength = team2->getPermutation().strength();
        if(team1_strength > team2_strength){
            team1->addPoints(3);
            team1->addGame();
            team2->addGame();
            output_t<int> out(2);
            return out;
        }
        else if(team2_strength > team1_strength){
            team2->addPoints(3);
            team1->addGame();
            team2->addGame();
            output_t<int> out(4);
            return out;
        }
        else{
            team1->addPoints(1);
            team2->addPoints(1);
            team1->addGame();
            team2->addGame();
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
	// TODO: Your code goes here
	return 22;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if(playerId<=0 || cards<0){
        return StatusType::INVALID_INPUT;
    }
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if(playerId<=0){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}
//updated
output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId<=0){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    std::shared_ptr<team> team1 = m_all_teams.findByKey(teamId);
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
	// TODO: Your code goes here
	return 12345;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if(playerId<=0){
        output_t<permutation_t> out(StatusType::INVALID_INPUT);
        return out;
    }
	// TODO: Your code goes here
	return permutation_t();
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    if(teamId1<=0 || teamId2<=0 || teamId1 == teamId2){
        return StatusType::INVALID_INPUT;
    }
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}
