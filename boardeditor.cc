#include <string>
#include <sstream>
#include "boardeditor.h"

using namespace std;

// class ChessBoardEditor

void ChessBoardEditor::clearBoard(ChessBoard& board) const {
    for (int f = 0; f < MAX_FILE; ++f) {
        for (int r = 0; r < MAX_RANK; ++r) {
            board.removePiece({f, r});
        }
    }
}

void ChessBoardEditor::resetBoard(ChessBoard& board) const {
    clearBoard(board);
    
    board.addPiece({0, 0}, 'R'); board.addPiece({0, 7}, 'r');
    board.addPiece({1, 0}, 'N'); board.addPiece({1, 7}, 'n');
    board.addPiece({2, 0}, 'B'); board.addPiece({2, 7}, 'b');
    board.addPiece({3, 0}, 'Q'); board.addPiece({3, 7}, 'q');
    board.addPiece({4, 0}, 'K'); board.addPiece({4, 7}, 'k');
    board.addPiece({5, 0}, 'B'); board.addPiece({5, 7}, 'b');
    board.addPiece({6, 0}, 'N'); board.addPiece({6, 7}, 'n');
    board.addPiece({7, 0}, 'R'); board.addPiece({7, 7}, 'r');
    for (int f = 0; f < MAX_FILE; ++f) {
        board.addPiece({f, 1}, 'P'); board.addPiece({f, 6}, 'p');
    }
    // White to move, no castle rights, no en passant by default
    board.setSideToMove(true);
    board.setCastling(true, true);
    board.setCastling(true, false);
    board.setCastling(false, true);
    board.setCastling(false, false);

    board.notifyObservers();

}

void ChessBoardEditor::setUpBoard(ChessBoard& board, istream& in) const {
    board.notifyObservers();

    char c;
    while (in >> c) {
        string token;
        Square position;
        char name;
        string colour, castling;
        switch (c) {
            case '+':
            in >> name >> position;
            board.addPiece(position, name, true);
            break;
            case '-':
            in >> position;
            name = board.removePiece(position, true);
            break;
            case '=':
            in >> colour;
            if (colour == "white") {
                board.setSideToMove(true);
            } else if (colour == "black") {
                board.setSideToMove(false);
            } else {
                cerr << "Invalid colour: " << colour << endl;
            }
            break;
            case '@': // [bonus] En passant setting
            in >> position;
            board.setEnPassant(position);
            break;
            case '#': // [bonus] Castling rights setting
            in >> colour >> castling;
            if (colour == "white") {
                if (castling == "short") {
                    board.setCastling(true, true);
                } else if (castling == "long") {
                    board.setCastling(true, false);
                } else {
                    cerr << "Invalid castling: " << castling << endl;
                }
            } else if (colour == "black") {
                if (castling == "short") {
                    board.setCastling(false, true);
                } else if (castling == "long") {
                    board.setCastling(false, false);
                } else {
                    cerr << "Invalid castling: " << castling << endl;
                }
            } else {
                cerr << "Invalid colour: " << colour << endl;
            }
            break;
            default:
            in.unget();
            in >> token;
            if (token == "done") {
                if (board.isValid()) return;
                else {
                    cerr << "Invalid position" << endl;
                }
            } else if (token == "reset") {
                resetBoard(board);
            } else if (token == "clear") {
                clearBoard(board);
                board.notifyObservers();
            } else {
                cerr << "Invalid token: " << token << endl;
            }
            break;
        }
    }

}

void ChessBoardEditor::loadBoard(ChessBoard& board, const string& fen) const {
    clearBoard(board);

    istringstream in{fen};
    char c;
    for (int r = MAX_FILE - 1; r >= 0; --r) {
        int f = 0;
        for (in >> c; in && c != '/' && c != ' '; c = in.get()) {
            if (c >= '1' && c <= '8') {
                f += c - '0';
            } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                board.addPiece({f, r}, c);
                ++f;
            }
        }
    }

    in >> c;
    if (c == 'w') board.setSideToMove(true);
    else if (c == 'b') board.setSideToMove(false);

    for (in >> c; in && c != ' '; c = in.get()) {
        switch (c) {
            case 'K': board.setCastling(true, true); break;
            case 'Q': board.setCastling(true, false); break;
            case 'k': board.setCastling(false, true); break;
            case 'q': board.setCastling(false, false); break;
        }
    }

    Square location;
    if (in >> location)
        board.setEnPassant(location);

    board.notifyObservers();

}
