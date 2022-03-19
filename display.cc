#include <string>
#include "display.h"
using namespace std;

ChessDisplay::ChessDisplay (ChessBoard* board) : board{board} {}

TextDisplay::TextDisplay(ChessBoard* board) : ChessDisplay{board} {
    board->attach(this);
}

TextDisplay::~TextDisplay() {
    board->detach(this);
}

void TextDisplay::notify() {
    for (int r = MAX_RANK - 1; r >= 0; --r) {
        cout << (char)(r + '1') << ' ';
        for (int f = 0; f < MAX_FILE; ++f) {
            char name = board->getPiece({f, r});
            cout << (name ? name : ' ') << ' ';
        }
        cout << endl;
    }
    cout << ' ' << ' ';
    for (int f = 0; f < MAX_FILE; ++f) {
        cout << (char)(f + 'a') << ' ';
    }
    cout << endl;
}

GraphicDisplay::GraphicDisplay(ChessBoard* board) : ChessDisplay{board}, w{make_unique<Xwindow>(300, 300)} {
    board->attach(this);
}

GraphicDisplay::~GraphicDisplay() {
    board->detach(this);
}

void GraphicDisplay::notify() {
    // Fill in for border
    w->fillRectangle(0, 0, 126, 126);
    int border = 3;
    // 3x3 square of 5 pixels
    int squareSize = 15;
    auto colour = Xwindow::White;
    for (int y = 0; y < MAX_FILE; ++y) {
        for (int x = MAX_RANK - 1; x >= 0; --x) {
            w->fillRectangle(border + y * squareSize, border + (MAX_RANK - 1 - x) * squareSize, squareSize, squareSize, colour);
            colour = (colour == Xwindow::White ? Xwindow::Red : Xwindow::White);
            char name = board->getPiece({y, x});
            if (name) {
                string s(1, name);
                w->drawString(border + 5 + y * squareSize, border + 12 + (MAX_RANK - 1 - x) * squareSize, s);
            }
        }
        colour = (colour == Xwindow::White ? Xwindow::Red : Xwindow::White);
    }
}