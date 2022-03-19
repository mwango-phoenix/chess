#include "strategy.h"

using namespace std;

ChessMove ChessStrategy::getRandomMove(const vector<ChessMove>& moves) const {
    return moves[rand() % moves.size()];
}

vector<ChessMove> ChessStrategy::findLegalMoves(const ChessBoard& board) const {
    vector<ChessMove> legalMoves;
    for (int fs = 0; fs < MAX_FILE; ++fs) {
        for (int rs = 0; rs < MAX_RANK; ++rs) {
            Square pos = {fs, rs};
            if (char name = board.getPiece(pos)) {
                for (int fd =0; fd < MAX_FILE; ++fd) {
                    for (int rd = 0;rd < MAX_RANK; ++rd) {
                        Square dest = {fd, rd};
                        if (board.isLegal(pos, dest)) {
                            bool sideToMove = board.getSideToMove();
                            if (name == (sideToMove ? 'P' : 'p') 
                                && dest.rank == (sideToMove ? 7 : 0))
                            {
                                legalMoves.push_back({pos, dest, sideToMove ? 'Q' : 'q'});
                                legalMoves.push_back({pos, dest, sideToMove ? 'B' : 'b'});
                                legalMoves.push_back({pos, dest, sideToMove ? 'N' : 'n'});
                                legalMoves.push_back({pos, dest, sideToMove ? 'R' : 'r'});
                            } else {
                                legalMoves.push_back({pos, dest, 0});
                            }
                        }
                    }
                }

            }
        }
    }
    return legalMoves;
}

bool ChessStrategy::isCheck(const ChessBoard& board, ChessMove move) const {
    ChessBoard newBoard{board};
    newBoard.makeMove(move.start, move.dest, move.promotion);
    return newBoard.isInCheck();
}

bool ChessStrategy::isCapture(const ChessBoard& board, ChessMove move) const {
    return board.getPiece(move.dest);
}
