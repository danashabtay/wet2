//
// Created by user on 25/12/2022.
//

include "UnionFind.h"

UnionFind::UnionFind(){

}

UnionFind::~UnionFind(){

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
        team->rep=player;
        player.team=team;
    }
    //else need to join to other set
    player.parent=team.rep;
    player.rg=player.rg-parent.rg;
    player.rs=parent.rs.inv()*player.rs;
}


void UnionFind::UniteTeams(int id1, int id2){
    Node node1 = players_hashTable.findNode(id1);
    Node node2 = players_hashTable.findNode(id2);

    Node parent1 = find(node1);
    Node parent2 = find(node2);

    //if they are part of same set do nothing
    if (parent1.data == parent2.data) {
        return;
    }

    //else whoever rank is higher becomes parent of other
    if (parent1.rank >= parent2.rank) {

        /// change rank

        ///change rs
        parent2.rs=parent1.rs.inv()*parent2.rs*///player1team.total_spirit
        ///change rg


        parent2.parent = parent1;

    } else {
        parent1.parent = parent2;
    }
    return;
}

Node UnionFind::find(Node data) {
    while(data.parent!= nullptr){
        data=data.parent;
    }
}

int UnionFind::findNumGames(int playerId){

}
bool UnionFind::isActive(int playerId){

}

void UnionFind::connectTeam(Node data, std::shared_ptr<team> teamPtr){
    data.team=teamPtr;
}
