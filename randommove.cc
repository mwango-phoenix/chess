#include "strategy.h"

using namespace std;

ChessMove RandomMoveStrategy::findMove(const ChessBoard& board) const {
    vector<ChessMove> legalMoves = std::move(findLegalMoves(board));
    return getRandomMove(legalMoves);
}
