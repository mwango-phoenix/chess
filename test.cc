// Unit test: Piece, ChessBoard, ChessBoardEditor

#include <string>
#include <sstream>
#include <iostream>
#include "board.h"
#include "boardeditor.h"
#include "display.h"

using namespace std;

void print(const ChessBoard& board) {
    for (int r = MAX_RANK - 1; r >= 0; --r) {
        cout << (char)(r + '1') << ' ';
        for (int f = 0; f < MAX_FILE; ++f) {
            char name = board.getPiece({f, r});
            cout << (name ? name : ' ') << ' ';
        }
        cout << endl;
    }
    cout << ' ' << ' ';
    for (int f = 0; f < MAX_FILE; ++f) {
        cout << (char)(f + 'a') << ' ';
    }
    cout << endl;

    switch (board.getResult()) {
        case ChessResult::LINE:
        cout << "In game" << endl;
        break;
        case ChessResult::WHITE:
        cout << "White wins" << endl;
        break;
        case ChessResult::BLACK:
        cout << "Black wins" << endl;
        break;
        case ChessResult::DRAW:
        cout << "Draw" << endl;
    }

    int count = 0;
    for (int fs = 0; fs < MAX_FILE; ++fs) {
        for (int rs = 0; rs < MAX_RANK; ++rs) {
            Square start = {fs, rs};
            for (int fd = 0; fd < MAX_FILE; ++fd) {
                for (int rd = 0; rd < MAX_RANK; ++rd) {
                    Square dest = {fd, rd};
                    if (board.isLegal(start, dest)) {
                        cout << start << '-' << dest << '\t';
                        if ((++count %= 10) == 0) cout << '\n';
                    }
                }
            }
        }
    }

    cout << endl;

}

int main() {
    ChessBoard board;
    ChessBoardEditor editor;

    TextDisplay textDisplay{&board};
    GraphicDisplay graphicDisplay{&board};

    string fen;
    while (getline(cin, fen)) {
        editor.loadBoard(board, fen);
        //print(board);

        string line;
        while (cin.peek() == ':') {
            getline(cin, line);
            istringstream in{line};
            in.ignore();

            Square start, dest;
            char promotion = 0;
            in >> start >> dest;
            if (!(in >> promotion)) {
                if (board.makeMove(start, dest)) {
                    //print(board);
                }
            } else {
                if (board.makeMove(start, dest, promotion)) {
                    //print(board);
                }
            }

        }

        while (cin.peek() == '\n') cin.ignore();

    }

    return 0;

}
