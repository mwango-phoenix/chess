#ifndef _BOARD_H_
#define _BOARD_H_

#include <utility>
#include <memory>
#include <vector>
#include "piece.h"
#include "subject.h"

struct ChessMove {
    Square start, dest; char promotion;
};

enum struct ChessResult {LINE, WHITE, BLACK, DRAW};

class ChessBoard : public Subject {
    // Represent the physical board
    std::vector<std::vector<std::shared_ptr<Piece>>> pieces;
    // Store game states
    bool sideToMove = true;
    std::pair<bool, bool> wCastling = {false, false}, bCastling = {false, false};
    bool enPassant = false;
    Square enPassantLocation;

    public:
    ChessBoard();
    ChessBoard(const ChessBoard& board); // [bonus]
    bool isValid(Square pos) const noexcept;
    char getPiece(Square pos) const noexcept;
    bool getSideToMove() const noexcept;
    ChessResult getResult() const;
    bool isLegal(Square start, Square dest) const;
    bool hasMaterial() const; // [bonus]
    bool hasLegalMove() const;
    bool isValid() const;
    bool isInCheck() const;
    bool makeMove(Square start, Square dest, char promotion = 0);
    void setSideToMove(bool colour) noexcept;
    void setCastling(bool colour, bool castleSide) noexcept;
    void setEnPassant(Square pos) noexcept;
    char addPiece(Square pos, char name, bool notify = false);
    char removePiece(Square pos, bool notify = false);
    char movePiece(Square start, Square dest, bool notify = false);

    private:
    bool isInCheck(const std::vector<std::vector<std::shared_ptr<Piece>>>& pieces) const;

};

#endif