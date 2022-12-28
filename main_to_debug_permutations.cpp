
#include "iostream"
#include "wet2util.h"
#include "vector"
using namespace std;

#define N 5 // (the size of each permutation in Wet2Util.h)
// Choose the maximal amount of permutations that you want to create and use:
#define M 10


// Print the array that present the permutation:
// Important - move the field "a" in class "permutation_t" (the array. look there, in "wet2util.h") to public!
void print (const permutation_t& permut);

// Multiply chosen permutations from the vector:
permutation_t mult_permutations (const vector<permutation_t>& permutations, int array_of_indexes[], int num_of_permutations);



int main() {
    // Here you choose the permutations (the spirits):
    int array[M][N] = {{0,1,2,3,4},
                       {1,0,3,4,2},
                       {0,1,3,4,2},
                       {1,0,2,3,4},
                       {4,1,2,3,0},
                       {2,4,1,3,0},
                       {0,4,3,1,2},
                       {0,1,2,4,3},
                       {4,3,2,1,0},
                       {3,0,4,2,1}};
    // Now the code create the permutations and insert them to the voector:
    vector<permutation_t> spirits;
    for (int i = 0; i<M ; i++) {
        permutation_t to_pust(array[i]);
        if (to_pust.isvalid()) spirits.push_back(permutation_t(array[i]));
        else {
            cout << "spirit " << i << " is not valid: ";
            print (to_pust);
            cout << "valid permutation is all the numbers from 0 to 4" << endl;
        }
    }


    /*------------------------
     * Enter your code here:
     * (create teams and players, buy teams... use the spirits that yuo created, from the vector "spirits",
     * and then, choose one spirit (spirit of team or partial-spirit of player that you want to check,
     * and print it in the function "print".)
     */




 //todo:remove the //   print(/* Enter here spirit (e.g.: with the function "get_partial_spirit", or another method)*/  tmp_player39_->getSpirit());

    /*----------------------
     * Choose the permutations that you want to multiply and compare with the spirit that you chose before:
     * (e.g.: if in team 2 the first player has spirit[3] (the 4 spirit in the array and in the vector),
     * and the second player has spirit[7], and you entered to the function "print" above "get_partial_spirit" of player2,
     * enter to the array "indexes_array" {3, 7}, and 2 to the last argument of the function "mult_permutations").
     * Now, check if the 2 permutations that printed to are equal.
     */
    int indexes_array[M] = {/*the indexes (in vector "spirits") of the spirits that you want to multiply*/2,3};
    permutation_t expected_spirit = mult_permutations(spirits, indexes_array, /*num of the permutations to multiply*/2);
    print(expected_spirit);

    return 0;
};




void print (const permutation_t& permut) {
    for (int i=0; i<5 ; i++) {
        //std::cout << permut.a[i] << ", ";//todo: the top is theirs
        std::cout << permut.getIthPlace(i) << ", ";
    }
    std::cout << std::endl;
}

permutation_t mult_permutations (const vector<permutation_t>& permutations, int array_of_indexes[], int num_of_permutations) {
    permutation_t result = permutation_t::neutral();
    for (int i=0; i<num_of_permutations; i++) {
        result = result * permutations[array_of_indexes[i]];
    }
    return result;
}
