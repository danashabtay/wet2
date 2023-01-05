///
// Created by 97254 on 25/12/2022.
//

#ifndef WET2_TEAM_H
#define WET2_TEAM_H
#include <memory>
#include "wet2util.h"
class team {
    private:
        int m_team_id;
        bool m_has_goalkeeper;
        permutation_t m_team_spirit;
        int m_points;
        int m_total_ability;

    public:
        bool hasKeeper();
        int getNumPoints() const;
        int getTeamAbility() const;
        void addPoints(int points_to_add);
        int getAbility() const;
        void addPlayerStats(int ability, permutation_t spirit, bool isKeeper);
        int getId() const;
        permutation_t getPermutation() const;
        int getPlayMatchStats();
        explicit team(int teamId) : m_team_id(teamId), m_has_goalkeeper(false),
                            m_team_spirit(permutation_t::neutral()), m_points(0) ,m_total_ability(0){}
};




#endif //WET2_TEAM_H
