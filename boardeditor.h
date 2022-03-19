#ifndef _BOARD_EDITOR_H_
#define _BOARD_EDITOR_H_

#include <iostream>
#include "board.h"

class ChessBoardEditor {
    public:
    void clearBoard(ChessBoard& board) const;
    void resetBoard(ChessBoard& board) const;
    void setUpBoard(ChessBoard& board, std::istream& in) const;
    // [bonus] Load board from FEN
    void loadBoard(ChessBoard& board, const std::string& fen) const;

};

#endif