#include <iomanip>
#include "game.h"

using namespace std;

// class ChessGame

ChessGame::ChessGame(ChessBoard& b) : board{b} {}

void ChessGame::setWhite(ChessPlayer* w) noexcept {
    white = w;
}

void ChessGame::setBlack(ChessPlayer* b) noexcept {
    black = b;
}

ChessResult ChessGame::runGame(ostream& out) {
    if (!white || !black) return board.getResult();
    
    board.notifyObservers();

    ChessResult state = printState(out);
    while (state == ChessResult::LINE) {
        ChessPlayer& player = board.getSideToMove() ? *white : *black;
        // Get a legal move from player
        bool resign = false;
        ChessMove move = player.play(board, resign);
        if (!resign) {
            clog << board.getPiece(move.start) 
                << move.start << '-' << move.dest << endl;
            bool legal = board.makeMove(move.start, move.dest, move.promotion);
            if (!legal) {
                resign = true;
                cerr << "Illegal move" << endl;
            }
        }
        if (resign) {
            if (board.getSideToMove()) {
                state = ChessResult::BLACK;
                out << "White resigns. ";
                out << "Black wins! " << endl;
            } else {
                state = ChessResult::WHITE;
                out << "Black resigns. ";
                out << "White wins! " << endl;
            }
            break;
        }
        state = printState(out);
    }

    switch (state) {
        case ChessResult::WHITE: ++++wScore; break;
        case ChessResult::BLACK: ++++bScore; break;
        case ChessResult::DRAW: ++wScore; ++bScore; break;
        case ChessResult::LINE: break;
    }

    return state;

}

ChessResult ChessGame::printState(ostream& out) const {
    ChessResult state = board.getResult();
    switch (state) {
        case ChessResult::WHITE:
        out << "Checkmate! ";
        out << "White wins! " << endl;
        break;
        case ChessResult::BLACK:
        out << "Checkmate! ";
        out << "Black wins! " << endl;
        break;
        case ChessResult::DRAW:
        if (board.hasMaterial()) {
            out << "Stalemate! ";
        } else {
            out << "Insufficient material. ";
        }
        out << "Draw. " << endl;
        break;
        case ChessResult::LINE:
        if (board.getSideToMove()) {
            out << "White to move. ";
        } else {
            out << "Black to move. ";
        }
        if (board.isInCheck()) {
            out << (board.getSideToMove() ? 
                "White is in check. " : "Black is in check. ");
        }
        out << endl;
        break;
    }
    return state;
}

void ChessGame::printScore(ostream& out) const {
    out << "Final score: " << endl;
    out << defaultfloat;
    out << fixed << setprecision(1);
    out << "White " << (float)wScore / 2 << '\t' 
        << "Black " << (float)bScore / 2 << endl;
}
