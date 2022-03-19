#include <string>
#include <sstream>
#include "player.h"

using namespace std;

// class HumanPlayer : public ChessPlayer

HumanPlayer::HumanPlayer(istream& i) : ins{i} {}

ChessMove HumanPlayer::play(const ChessBoard& board, bool& resign) const {
    string line;
    while (getline(ins, line)) {
        istringstream in{line};
        string token;
        if (in >> token) {
            if (token == "move") {
                ChessMove move;
                if (in >> move.start >> move.dest) {
                    if (board.isLegal(move.start, move.dest)) {
                        if (in >> move.promotion) {
                            bool sideToMove = board.getSideToMove();
                            if (move.promotion != (sideToMove ? 'Q' : 'q')
                                && move.promotion != (sideToMove ? 'B' : 'b')
                                && move.promotion != (sideToMove ? 'N' : 'n')
                                && move.promotion != (sideToMove ? 'R' : 'r'))
                            {
                                cerr << "Invalid promotion: " << move.promotion << endl;
                                continue;
                            }
                        }
                        return move;
                    } else {
                        cerr << "Illegal move: " 
                            << board.getPiece(move.start) 
                            << move.start << '-' << move.dest << endl;
                    }
                }
            } else if (token == "resign") {
                resign = true;
                return {};
            } else {
                cerr << "Invalid token: " << token << endl;
            }
        }
    }
    return {};
}

// class ComputerPlayer : public ChessPlayer

ComputerPlayer::ComputerPlayer(const ChessStrategy& s) : strategy{s} {}

ChessMove ComputerPlayer::play(const ChessBoard& board, bool& resign) const {
    return strategy.findMove(board);
}
