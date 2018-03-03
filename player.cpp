#include "player.hpp"
#define CORNER_GAIN 20
#define BUFFER_GAIN -5
#define EDGE_GAIN 3

//TODO: Implement minimax recursively for more depth in search
//TODO: Alpha-Beta pruning
//TODO: Tune heuristic, clean up code into functions to get best move (through minimax etc...)


/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

    if(!testingMinimax){
         myBoard = new Board();
    }

    mySide = side;

    if (mySide == WHITE){
        oppSide = BLACK;
    }
    else {
        oppSide = WHITE;
    }

    if(mySide == BLACK){
        std::cerr << "WE ARE BLACK PLAYER" << std::endl;
    }
    else {
        std::cerr << "WE ARE WHITE PLAYER" << std::endl;
    }

}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete myBoard;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    
    // First process opponents move
    if(opponentsMove != nullptr){
        myBoard->doMove(opponentsMove, oppSide);
    }

    if(!testingMinimax){

        // Naive Approach: Loop through every possible move
        // and make the first valid one.

        // Move *move = new Move(0, 0);

        // for (int i = 0; i < 8; i++) {
        //     for (int j = 0; j < 8; j++) {

        //         move->setX(i);
        //         move->setY(j);

        //         if (myBoard->checkMove(move, mySide)){
        //             myBoard->doMove(move, mySide);
        //             return move;
        //         } 
        //     }
        // }
        // // If found no valid move, then return nullptr
        // return nullptr;

        // Approach that beats Simple Player: Loop through every
        // possible move and choose the one that gives the highest
        // score (our coins - opponents coins). Adjust the score however
        // by adding a gain for a corner move and subtracting a factor
        // for a move that is a buffer move (enables a corner move)

        // int score;
        // int max_score = -10000;

        // Move *move = new Move(0, 0);
        // int best_move_X = -1;
        // int best_move_Y = -1;

        // for (int i = 0; i < 8; i++) {
        //     for (int j = 0; j < 8; j++) {

        //         move->setX(i);
        //         move->setY(j);

        //         if (myBoard->checkMove(move, mySide)){
        //             // Make copy of board, and do a move 
        //             // and see how profitable it is
        //             Board *myBoardCopy = myBoard->copy();
        //             myBoardCopy->doMove(move, mySide);
        //             score = myBoardCopy->count(mySide) - myBoardCopy->count(oppSide);

        //             // Adjust score for whether move is a corner or buffer
        //             if ( (i == 0 && j == 0)||(i == 7 && j == 0)||(i == 7 && j == 7) ||(i == 0 && j == 7)  ){
        //                 score += CORNER_GAIN;

        //             }
        //             else if(  ( (i + j) <= 2  )||( (i + j) >= 13  )||( i < 2 && j > 5 ) ||( j < 2 && i > 5 ) ){
        //                 score += BUFFER_GAIN;
        //             }

        //             // Update maxGain
        //             if(score > max_score){
        //                 max_score = score;
        //                 best_move_X = i;
        //                 best_move_Y = j;
        //             }

        //             // Delete the board copy we made for exploration
        //             delete myBoardCopy;
        //         } 
        //     }
        // }

        // // Delete the move object we use for iteration
        // delete move;

        // if(best_move_X > -1){
        //     Move *bestMove = new Move(best_move_X, best_move_Y);
        //     myBoard->doMove(bestMove, mySide);
        //     return bestMove;
        // }

        // // If found no valid move, then return nullptr
        // return nullptr;

        // Approach that beats Constant Time Player:Implement minimax
        // of depth 2. Adjust the score however by adding a gain for a 
        // corner move and edge move and subtracting a factor
        // for a move that is a buffer move (enables a corner move)
        int new_score;
        Move *move = new Move(0, 0);
        int best_move_X = -1;
        int best_move_Y = -1;
        int min_gain;
        int max_min_gain = -1000000;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {

                move->setX(i);
                move->setY(j);

                if (myBoard->checkMove(move, mySide)){

                    // For each possible move I could have made, make a copy of the board and do that move
                    Board *myBoardCopy = myBoard->copy();
                    myBoardCopy->doMove(move, mySide);
                    min_gain = 1000000;

                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) { 

                            move->setX(k);
                            move->setY(l);

                            if (myBoardCopy->checkMove(move, oppSide)){
                                //For each possible move opponent could have made after this, do that move on this copy
                                myBoardCopy->doMove(move, oppSide);
                                new_score = myBoardCopy->count(mySide) - myBoardCopy->count(oppSide);

                                // Adjust score for whether our move is a corner or buffer
                                if ( (i == 0 && j == 0)||(i == 7 && j == 0)||(i == 7 && j == 7) ||(i == 0 && j == 7)  ){
                                    new_score += CORNER_GAIN;

                                }
                                else if(  ( (i + j) <= 2  )||( (i + j) >= 13  )||( i < 2 && j > 5 ) ||( j < 2 && i > 5 ) ){
                                    new_score += BUFFER_GAIN;
                                }
                                else if( i == 0 || j == 0 || i == 7 || j == 7){
                                    new_score += EDGE_GAIN;
                                }

                                if(new_score < min_gain){
                                    min_gain = new_score;
                                }
                            }

                        }
                    }

                    if (min_gain > max_min_gain){
                        max_min_gain = min_gain;
                        best_move_X = i;
                        best_move_Y = j;
                    }

                    delete myBoardCopy;
                } 
            }
        }

        // Delete the move object we use for iteration
        delete move;

        if(best_move_X > -1){
            Move *bestMove = new Move(best_move_X, best_move_Y);
            myBoard->doMove(bestMove, mySide);
            return bestMove;
        }

        // If found no valid move, then return nullptr
        return nullptr;  

    }
    // Approach used for testminimax. Creates minimax decision tree
    // of depth 2 and chooses move purely based on this.
    else{

        int new_score;
        Move *move = new Move(0, 0);
        int best_move_X = -1;
        int best_move_Y = -1;
        int min_gain;
        int max_min_gain = -1000000;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {

                move->setX(i);
                move->setY(j);

                if (myBoard->checkMove(move, mySide)){

                    // For each possible move I could have made, make a copy of the board and do that move
                    Board *myBoardCopy = myBoard->copy();
                    myBoardCopy->doMove(move, mySide);
                    min_gain = 1000000;

                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) { 

                            move->setX(k);
                            move->setY(l);

                            if (myBoardCopy->checkMove(move, oppSide)){
                                //For each possible move opponent could have made after this, do that move on this copy
                                myBoardCopy->doMove(move, oppSide);
                                new_score = myBoardCopy->count(mySide) - myBoardCopy->count(oppSide);

                                if(new_score < min_gain){
                                    min_gain = new_score;
                                }
                            }

                        }
                    }

                    if (min_gain > max_min_gain){
                        max_min_gain = min_gain;
                        best_move_X = i;
                        best_move_Y = j;
                    }

                    delete myBoardCopy;
                } 
            }
        }

        // Delete the move object we use for iteration
        delete move;

        if(best_move_X > -1){
            Move *bestMove = new Move(best_move_X, best_move_Y);
            myBoard->doMove(bestMove, mySide);
            return bestMove;
        }

        // If found no valid move, then return nullptr
        return nullptr;
    }
}