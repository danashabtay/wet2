//
// Created by user on 25/12/2022.
//

include "UnionFind.h"

UnionFind::UnionFind() : players_hashTable(new HashTable<playerNode>()), teams_hashTable(new HashTable<teamNode>()) {}

UnionFind::~UnionFind(){
    destroy();
    return;
}

void UnionFind::destroy(){
    players_hashTable.deleteTable():
    teams_hashTable.deleteTable():
    return;
}


void UnionFind::addTeam(int teamId, shared_ptr<team> newData){
    std::shared_ptr<teamNode> node (new teamNode());
   node->rep=nullptr;
   node->data=newData;
   node->rank = 0;
   node->team_spirit=permutation_t::neutral();
   teams_hashTable.insertNode(teamId, node);
}

void UnionFind::addSinglePlayer(shared_ptr<player> newData ,int playerId, int teamId){
    std::shared_ptr<playerNode> node (new playerNode());
    node->data = newData;
    node->parent = node;
    node->rs=newData->getSpirit;
    node->rg=newData->getNumGames;
    players_hashTable.insertNode(playerId, node);
    std::shared_ptr<teamNode> team = teams_HashTable.findNode(teamId);
    team->rank++;

    //if team is empty
    if(team->rep==nullptr){
        team->rep=node;
        node->team=team;
    }
    //else need to join to other set
    node->parent=team->rep;
    node->rg=node->rg-parent->rg;
    node->rs=(parent->rs->inv())*(team->team_spirit)*(node->rs);
    team->team_spirit=(team->team_spirit)*(newData->getSpirit);
}

///update here rs and rg
void UnionFind::UniteTeams(int owner, int added){
    std::shared_ptr<playerNode> node1 = players_hashTable.findNode(owner);
    std::shared_ptr<playerNode> node2 = players_hashTable.findNode(added);
    std::shared_ptr<playerNode> ownerParent = find(node1);
    std::shared_ptr<playerNode> addedParent = find(node2);
    //if they are part of same set do nothing
    if (parent1->data == parent2->data) {
        return;
    }
    //else whoever rank is higher becomes parent of other
    if (ownerParent->team->rank >= addedParent->team->rank) { // add added team to owner team
        teams_HashTable.markDeleted(added);
        /// change rank
        ownerParent->team->rank+=addedParent->team->rank;
        ///change rs
        addedParent->rs=(ownerParent->rs.inv())*(ownerParent->team->team_spirit)*(addedParent->rs);
        ///change rg
        addedParent->rg=addedParent->rg-ownerParent->rg;
        addedParent->parent = ownerParent;
    } else { // add owner team to added team
        std::shared_ptr<teamNode> ownerTeamNode = teams_HashTable.findNode(owner);
        std::shared_ptr<teamNode> addedTeamNode = teams_HashTable.findNode(added);
        ownerTeamNode->rep=addedTeamNode->rep;
        addedParent->team=ownerParent->team;
        teams_HashTable.markDeleted(added);
        /// change rank
        addedParent->team->rank+=ownerParent->team->rank;
        ///change rs
        permutation_t temp_b = addedParent->rs;
        ownerParent->rs=(temp_b.inv())*ownerParent->rs;
        addedParent->rs=temp_b*(ownerParent->team->team_spirit);
        ///change rg
        ownerParent->rg=ownerParent->rg-addedParent->rg;
        ownerParent->parent = addedParent;
    }
    return;
}

/**
    * Find the representative recursively and does path compression as well.
    */
std::shared_ptr<playerNode> UnionFind::find(std::shared_ptr<playerNode> node) {
/*
    std::shared_ptr<playerNode> parent = node->parent;
    if (parent == node) {
        return parent;
    }
    node->parent = find(node.parent);
    return node->parent;
*/

    std::shared_ptr<playerNode> og_node=node;
    permutation_t spirit_sum= permutation_t::neutral();
    int game_sum=0;
    while(node->parent!=node){
        spirit_sum=spirit_sum*node->rs;
        game_sum+=node->rg;
        node=node->parent;
    }
    int partial_games=0;
    permutation_t partial_spirit=permutation_t::neutral();
    while(og_node!=node){
        std::shared_ptr<playerNode> next=og_node->parent;
        og_node->parent=node;
        permutation_t tmp_spirit=og_node->rs;
        int tmp_games=og_node->rg;
        og_node->rs=(partial_spirit.inv())*spirit_sum;
        og_node->rg=game_sum-partial_games;
        partial_spirit=partial_spirit*tmp_spirit;
        partial_games+=tmp_games;
        og_node=next;
    }
    return node->parent;
}



int UnionFind::findNumGames(int playerId){
    std::shared_ptr<playerNode> node = players_hashTable.findNode(playerId);
    std::shared_ptr<playerNode> parent = find(node);
    int sum=0;
    while(node!=parent){
        sum+=node->rg;
        node=node->parent;
    }
    return sum;
}

permutation_t UnionFind::findSpirit(int playerId){
    std::shared_ptr<playerNode> node = players_hashTable.findNode(playerId);
    std::shared_ptr<playerNode> parent = find(node);
    permutation_t spiritSum= spiritSum.neutral();
    while(node!=parent){
        spiritSum=node->rs.inv()*spiritSum;
        node=node->parent;
    }
    return spiritSum;
}

bool UnionFind::isActive(int playerId){
    std::shared_ptr<playerNode> node = players_hashTable.findNode(playerId);
    std::shared_ptr<playerNode> parent = find(node);
    std::shared_ptr<teamNode> team = parent->team;
    int teamId = team->data->getId();
    return teams_hashTable.isDeleted(teamId);
}

void UnionFind::markDeleted(int teamId){
    teams_hashTable.markDeleted(teamId);
}

bool UnionFind::doesExist(int playerId){
    std::shared_ptr<playerNode> node = players_hashTable.findNode(playerId);
    if(node== nullptr){
        return true;
    }
    return false;
}

void UnionFind::addGame(int teamId1, int teamId2){
    std::shared_ptr<teamNode> team1 = teams_HashTable.findNode(teamId1);
    std::shared_ptr<teamNode> team2 = teams_HashTable.findNode(teamId2);
    std::shared_ptr<player> rep1= team1->rep;
    std::shared_ptr<player> rep2= team2->rep;
    rep1->rg++;
    rep2->rg++;
    return;
}

std::shared_ptr<player> UnionFind::findById(int playerId){
    std::shared_ptr<playerNode> node=players_hashTable.findNode(playerId);
    std::shared_ptr<playerNode> parent=find(node);
    return parent->data;
}

permutation_t UnionFind::getPartialSpirit(int playerId){
    std::shared_ptr<playerNode> node=players_hashTable.findNode(playerId);
    std::shared_ptr<playerNode> parent=find(node);
    permutation_t sum= permutation_t::neutral();
    while(node!=parent){
        sum=(parent->rs)*sum;
        node=node->parent;
    }
    return sum;
}
