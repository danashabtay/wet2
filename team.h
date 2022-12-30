//
// Created by 97254 on 25/12/2022.
//

#ifndef WET2_TEAM_H
#define WET2_TEAM_H
#include "wet2util.h"
#include <memory>
#include "wet2util.h"
class team {
    private:
        int m_team_id;
        int m_points;
        bool m_has_goalkeeper;
        std::shared_ptr<permutation_t> m_team_spirit; ///why shared ptr?
        int m_total_ability;
        int m_games_played; ///probably don't need;

    public:
        bool hasKeeper() const;
        int getNumPoints() const;
        int getTeamAbility() const;
        void addPoints(int points_to_add);
        void addGame();
        int getAbility();
        int getId();
        std::shared_ptr<permutation_t> getPermutation() const;
        team(int teamId) : m_team_id(teamId), m_points(0), m_has_goalkeeper(false),
                            m_team_spirit(new permutation_t()), m_total_ability(0), m_games_played(0){
            m_team_spirit=m_team_spirit.neutral();
        }
        void addPlayerStats(int ability, permutation_t spirit);
};


#endif //WET2_TEAM_H
