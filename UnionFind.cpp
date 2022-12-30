//
// Created by user on 25/12/2022.
//

include "UnionFind.h"

UnionFind::UnionFind() : players_hashTable(new HashTable<playerNode>()), teams_hashTable(new HashTable<teamNode>()) {}

UnionFind::~UnionFind(){
    players_hashTable.deleteTable():
    teams_hashTable.deleteTable():
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
    playerNode* node1 = players_hashTable.findNode(owner);
    playerNode* node2 = players_hashTable.findNode(added);
    playerNode* ownerParent = find(node1);
    playerNode* addedParent = find(node2);
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
        addedParent->rs=ownerParent->rs.inv()*addedParent->rs;
        ///change rg
        addedParent->rg=ownerParent.rg-addedParent.rg;
        addedParent->parent = ownerParent;
    } else { // add owner team to added team
        teamNode* ownerTeamNode = teams_HashTable.findNode(owner);
        teamNode* addedTeamNode = teams_HashTable.findNode(added);
        ownerTeamNode->rep=addedTeamNode.rep;
        addedParent->team=ownerParent.team;
        teams_HashTable.markDeleted(added);
        /// change rank
        addedParent->team->rank+=ownerParent->team->rank;
        ///change rs
        ownerParent->rs=addedParent->rs.inv()*ownerParent->rs;
        ///change rg
        ownerParent->rg=ownerParent->rg-addedParent->rg;
        ownerParent->parent = addedParent;
    }
    return;
}

/**
    * Find the representative recursively and does path compression as well.
    */
playerNode* UnionFind::find(playerNode* node) {
    playerNode* parent = node->parent;
    if (parent == node) {
        return parent;
    }
    node->parent = find(node.parent);
    return node->parent;
}



int UnionFind::findNumGames(int playerId){
    playerNode* node = players_hashTable.findNode(playerId);
    playerNode* parent = find(node);
    int sum=0;
    while(node!=parent){
        sum+=node->rg;
        node=node->parent;
    }
    return sum;
}

permutation_t UnionFind::findSpirit(int playerId){
    playerNode* node = players_hashTable.findNode(playerId);
    playerNode* parent = find(node);
    permutation_t spiritSum= spiritSum.neutral();
    while(node!=parent){
        spiritSum=node->rs.inv()*spiritSum;
        node=node->parent;
    }
    return spiritSum;
}

bool UnionFind::isActive(int playerId){
    playerNode* node = players_hashTable.findNode(playerId);
    playerNode* parent = find(node);
    teamNode* team = parent->team;
    int teamId = team->data->getId();
    return teams_hashTable.isDeleted(teamId);
}

void UnionFind::markDeleted(int teamId){
    teams_hashTable.markDeleted(teamId);
}

bool UnionFind::doesExist(int playerId){
    playerNode* node = players_hashTable.findNode(playerId);
    if(node== nullptr){
        return true;
    }
    return false;
}

void UnionFind::addGame(int teamId1, int teamId2){
    teamNode team1 = teams_HashTable.findNode(teamId1);
    teamNode team2 = teams_HashTable.findNode(teamId2);
    std::shared_ptr<player> rep1= team1->rep;
    std::shared_ptr<player> rep2= team2->rep;
    rep1->rg++;
    rep2->rg++;
    return;
}

std::shared_ptr<player> UnionFind::findById(int playerId){
    playerNode* node=players_hashTable.findNode(playerId);
    playerNode* parent=find(node);
    return parent->data;
}

permutation_t UnionFind::getPartialSpirit(int playerId){
    playerNode* node=players_hashTable.findNode(playerId);
    playerNode* parent=find(node);
    permutation_t sum= permutation_t::neutral();
    while(node!=parent){
        sum=(parent->rs)*sum;
        node=node->parent;
    }
    return sum;
}
