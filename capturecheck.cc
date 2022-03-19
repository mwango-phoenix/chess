#include "strategy.h"

using namespace std;

ChessMove CaptureCheckStrategy::findMove(const ChessBoard& board) const {
    vector<ChessMove> legalMoves = move(findLegalMoves(board));
    vector<ChessMove> goodMoves;
    for (const ChessMove& m : legalMoves) {
        if (isCheck(board, m) || isCapture(board, m)) {
            goodMoves.push_back(m);
        }
    }
    if (!goodMoves.empty()) return getRandomMove(goodMoves);
    else return getRandomMove(legalMoves);
}
