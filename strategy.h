#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include <cmath>
#include <vector>
#include "board.h"

class ChessStrategy {
    public:
    virtual ChessMove findMove(const ChessBoard& board) const = 0;
    protected:
    ChessMove getRandomMove(const std::vector<ChessMove>& moves) const;
    std::vector<ChessMove> findLegalMoves(const ChessBoard& board) const;
    bool isCheck(const ChessBoard& board, ChessMove move) const;
    bool isCapture(const ChessBoard& board, ChessMove move) const;

};

class RandomMoveStrategy : public ChessStrategy {
    public:
    ChessMove findMove(const ChessBoard& board) const override;

};

class CaptureCheckStrategy : public ChessStrategy {
    public:
    ChessMove findMove(const ChessBoard& board) const override;

};

class AvoidCaptureStrategy : public ChessStrategy {
    public:
    ChessMove findMove(const ChessBoard& board) const override;

};

class MinimaxStrategy : public ChessStrategy {
    const int maxDepth;

    private:
    /* Heuristic function to evaluate position
        K = 0, Q = 9, B = 3, N = 3, R = 5, P = 1
        white +, black -
    */
    double evaluate(const ChessBoard& board) const;
    // Implement minimax with alpha-beta pruning
    double maximize(const ChessBoard& board, ChessMove& move, 
        int depth, double alpha = -INFINITY, double beta = INFINITY) const;
    double minimize(const ChessBoard& board, ChessMove& move, 
        int depth, double alpha = -INFINITY, double beta = INFINITY) const;

    public:
    MinimaxStrategy(int d);
    ChessMove findMove(const ChessBoard& board) const override;

};

#endif