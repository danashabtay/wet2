//
// Created by user on 25/12/2022.
//

include "UnionFind.h"

UnionFind::UnionFind() : players_hashTable(new HashTable<shared_ptr<player>>()), teams_hashTable(new HashTable<shared_ptr<team>>()) {}

UnionFind::~UnionFind(){
    players_hashTable.deleteTable():
    teams_hashTable.deleteTable():
}

void UnionFind::addTeam(int teamId, shared_ptr<team> newData){
   teamNode node = new teamNode();
   node.rep=nullptr;
   node.data=newData;
   node.rank = 0;
   teams_hashTable.insertNode(teamId, newData);
}

void UnionFind::addSinglePlayer(shared_ptr<player> newData ,int playerId, int teamId){
    playerNode node = new playerNode();
    node.data = newData;
    node.parent = node;
    node.rs=newData->getSpirit;
    node.rg=0;
    players_hashTable.insertNode(playerId, newData);
    teamNode team = teams_HashTable.findNode(teamId);
    team.rank++;
    //if team is empty
    if(team->rep==nullptr){
        team->rep=node;
        node.team=team;
    }
    //else need to join to other set
    node.parent=team.rep;
    node.rg=node.rg-parent.rg;
    node.rs=parent.rs.inv()*node.rs;
}


void UnionFind::UniteTeams(int owner, int added){
    playerNode node1 = players_hashTable.findNode(owner);
    playerNode node2 = players_hashTable.findNode(added);
    playerNode ownerParent = find(node1);
    playerNode addedParent = find(node2);
    //if they are part of same set do nothing
    if (parent1.data == parent2.data) {
        return;
    }
    //else whoever rank is higher becomes parent of other
    if (ownerParent.team.rank >= addedParent.team.rank) { // add added team to owner team
        teams_HashTable.markDeleted(added);
        /// change rank
        ownerParent.team.rank+=addedParent.team.rank;
        ///change rs
        addedParent.rs=ownerParent.rs.inv()*addedParent.rs;
        ///change rg
        addedParent.rg=ownerParent.rg-addedParent.rg;
        addedParent.parent = ownerParent;
    } else { // add owner team to added team
        teamNode ownerTeamNode = teams_HashTable.findNode(owner);
        teamNode addedTeamNode = teams_HashTable.findNode(added);
        ownerTeamNode.rep=addedTeamNode.rep;
        addedParent.team=ownerParent.team;
        teams_HashTable.markDeleted(added);
        /// change rank
        addedParent.team.rank+=ownerParent.team.rank;
        ///change rs
        ownerParent.rs=addedParent.rs.inv()*ownerParent.rs;
        ///change rg
        ownerParent.rg=ownerParent.rg-addedParent.rg;
        ownerParent.parent = addedParent;
    }
    return;
}

/**
    * Find the representative recursively and does path compression as well.
    */
playerNode UnionFind::find(playerNode node) {
    playerNode parent = node.parent;
    if (parent == node) {
        return parent;
    }
    node.parent = findSet(node.parent);
    return node.parent;

}

int UnionFind::findNumGames(int playerId){
    playerNode node = players_hashTable.findNode(playerId);
    playerNode parent = find(node);
    int sum=0;
    while(node!=parent){
        sum+=node.rg;
        node=node.parent;
    }
    return sum;
}

permutation_t UnionFind::findSpirit(int playerId){
    playerNode node = players_hashTable.findNode(playerId);
    playerNode parent = find(node);
    permutation_t spiritSum= spiritSum.neutral();
    while(node!=parent){
        spiritSum=node.rs.inv()*spiritSum;
        node=node.parent;
    }
    return spiritSum;
}

bool UnionFind::isActive(int playerId){
    playerNode node = players_hashTable.findNode(playerId);
    playerNode parent = find(node);
    int teamId = parent.data->getId();
    return teams_hashTable.isDeleted(teamId);
}

