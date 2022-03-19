#include "strategy.h"

using namespace std;

ChessMove AvoidCaptureStrategy::findMove(const ChessBoard& board) const {
    vector<ChessMove> legalMoves = move(findLegalMoves(board));
    vector<ChessMove> goodMoves;
    for (const ChessMove& m : legalMoves) {
        ChessBoard newBoard{board};
        newBoard.makeMove(m.start, m.dest, m.promotion);
        vector<ChessMove> newLegalMoves = move(findLegalMoves(newBoard));
        bool canBeCaptured = false;
        for (const ChessMove& nm : newLegalMoves) {
            if (isCapture(newBoard, nm)) {
                canBeCaptured = true;
                break;
            }
        }
        if (!canBeCaptured) goodMoves.push_back(m);
    }
    vector<ChessMove> betterMoves;
    for (const ChessMove& m : goodMoves) {
        if (isCheck(board, m) || isCapture(board, m)) {
            betterMoves.push_back(m);
        }
    }
    if (!betterMoves.empty()) return getRandomMove(betterMoves);
    else if (!goodMoves.empty()) return getRandomMove(goodMoves);
    else return getRandomMove(legalMoves);
}
