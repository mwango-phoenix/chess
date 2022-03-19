#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include "board.h"
#include "player.h"

class ChessGame {
    ChessPlayer *white, *black;
    ChessBoard &board;
    // Store double the score due to half-points
    int wScore = 0, bScore = 0;
    
    public:
    ChessGame(ChessBoard& b);
    void setWhite(ChessPlayer* w) noexcept;
    void setBlack(ChessPlayer* b) noexcept;
    // Broadcast a game from position
    ChessResult runGame(std::ostream& out);
    // Print description of board state
    ChessResult printState(std::ostream& out) const;
    // Print final score
    void printScore(std::ostream& out) const;

};

#endif