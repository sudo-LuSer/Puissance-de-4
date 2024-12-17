#include <stdio.h>
#include "Puissance4.h"

#define SEARCH_DEPTH 6

/*const int priority[42] = {  3,  4,  5,  7,  5,  4,  3,
                            4,  6,  8,  10, 8,  6,  4,
                            5,  8,  11, 13, 11, 8,  5,
                            5,  8,  11, 13, 11, 8,  5,
                            4,  6,  8,  10, 8,  6,  4,
                            3,  4,  5,  7,  5,  4,  3};*/







int put_token(char player, int col, char* grille) {
    // Put a token for given player, in given column.
    // returns 1 if column was already full, oherwise 0.
    
    // there is no check on player and col because this function is called by
    // another valid function, so no illegal values should be received

    int index = col;           // start from row 0
    while (grille[index]) {    // while there's a token at position
        index += 7;            // go to the next row
    }
    grille[index] = player;
    return 0;
}


void rem_token(int col, char* grille) {
    // remove last token at column col.
    
    // there is no check on col and grille because this function is called by
    // another valid function, so no illegal values should be received.
    // Grille should already have at least one token.

    int index = col+35;        // start from last row
    while (! grille[index]) {    // while there's no token at position
        index -= 7;            // go to the row below
    }
    grille[index] = 0;
}




float check_play_rec(char joueur, char *grille, int depth, float max_brother) {
    // returns score of grille
    int v;

    // if there is a winner, return max score (1 or -1 according to winner)
    if (v=TestVainqueur(grille)) {
        return 2.0*(v==joueur) - 1.0;
    }
    
    // if depth is 0, the max recursion is reached, the return score is that
    // of the most valuable reachable position
    if (depth<1) {
        int score = 0;
        int index;
        for (int i=0; i<7; i++) {
            if (! grille[i+35]) {
                index = i;
                while (grille[index]) {
                    index += 7;
                }
                if (score<priority[index]) {
                    score = priority[index];
                }
            }
        }
        return -score*0.01;
    }

    // compute score for each possible move and keep maximum value
    float max_score = -1.0;     // start with minimal score
    float new_score;            // to store the current score
    for (int i=0; i<7; i++) {
        // first check is the move is possible, if not, skip the move
        if (grille[35+i]) {
            continue;
        }
        
        // put a token, then compute the score, finally remove the tested move
        put_token(joueur, i, grille);
        new_score = -check_play_rec(3-joueur, grille, depth-1, -max_score);
        rem_token(i, grille);
        
        // compute maximal score
        if (new_score>max_score) {
            max_score = new_score;
            if (max_score*0.99 > max_brother) {
                // here, computing next moves have no influence of the final
                // result, so skip them
                return max_score*0.99;
            }
        }
        
    }
    return max_score*0.99;
}



int CalculColonneAI() {
    // compute the column to play with a basic AI
    
    // first make a copy of the current game 
    char grille[42];
    for (int i=0; i<42; i++) {
        grille[i] = GrilleDeJeu[i];
    }
    
    // now compute scores for all positions and remember all moves that give
    // the highest score
    float max_score = -2.0;     // we start with minimal score
    float new_score;            // current score
    char best_pos[7];           // moves that provided max_score
    for (int i=0; i<7; i++) {
        // if move is illegal, blank it in best_pos and skip
        if (grille[35+i]) {
            best_pos[i]=0;
            continue;
        }
        
        // make move, compute score, then clear move
        put_token(2, i, grille);
        new_score = -check_play_rec(1, grille, SEARCH_DEPTH, 10.0);
        rem_token(i, grille);
        
        // fill best_pos according to new_score
        if (new_score>max_score) {
            max_score = new_score;
            for (int j=0; j<i; j++) {
                best_pos[j]=0;
            }
            best_pos[i]=1;
        } else if (new_score < max_score) {
            best_pos[i]=0;
        } else {
            best_pos[i]=1;
        }
    }
    // from all equal scored solutions, choose the one that correspond to the
    // position which is the most valuable.
    max_score = 0.0;
    int index;
    int final_col;
    for (int i=0; i<7; i++) {
        // skip if column is not part of the most interesting
        if (! best_pos[i]) {
            continue;
        }
        
        // look for row of the corresponding column
        index = i;
        while (grille[index]) {
            index += 7;
        }
        
        // get position value and compare it to the previous ones
        if (priority[index] > max_score) {
            max_score = priority[index];
            final_col = i;
        }
    }
    //finall return result
    return final_col;
}
