#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream>
#include "board.h"
#include "strategy.h"

class ChessPlayer {
    public:
    /* Modify start and dest to represent a move request
        Move will be verified and performed in ChessGame
        Assume board.getSideToMove is the side player is on
        Use board.getPiece to look at the board
        Return true unless player resigns the game
    */
    virtual ChessMove play(const ChessBoard& board, bool& resign) const = 0;
    virtual ~ChessPlayer() = default;

};

class HumanPlayer : public ChessPlayer {
    std::istream& ins;

    public:
    HumanPlayer(std::istream& i);
    virtual ChessMove play(const ChessBoard& board, bool& resign) const override;
    virtual ~HumanPlayer() = default;

};

class ComputerPlayer : public ChessPlayer {
    // [Strategy]
    const ChessStrategy& strategy;

    public:
    ComputerPlayer(const ChessStrategy& s);
    virtual ChessMove play(const ChessBoard& board, bool& resign) const override;
    virtual ~ComputerPlayer() = default;

};

#endif