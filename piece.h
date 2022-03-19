#ifndef PIECE_H
#define PIECE_H
#include <vector>
#include <memory>
#include <iostream>

const int MAX_FILE = 8;
const int MAX_RANK = 8;

struct Square {
    int file, rank;
   public:
    bool operator==(const Square& s) const noexcept;
};

std::istream& operator>>(std::istream& in, Square& s);
std::ostream& operator<<(std::ostream& out, const Square& s);

class Piece {
   protected:
     bool colour;
     Square position;

   public:
     Piece(Square p, bool c);
     // Used for deep copies
     virtual std::shared_ptr<Piece> clone() const = 0;
     virtual char getName() const noexcept = 0;
     bool getColour() const noexcept;
     void setPos(Square pos) noexcept;
     // [Virtual method] Check if the piece is allowed to move to dest
     bool isLegal(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces);
     virtual ~Piece() = default;

   private:
    virtual bool canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) = 0;
    virtual bool canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) = 0;
};

class Pawn : public Piece {
     bool canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;
     bool canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;

   public:
     Pawn(Square p, bool c);
     std::shared_ptr<Piece> clone() const override;
     bool canEnPassant(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces);
     virtual char getName() const noexcept override;
};

class King : public Piece {
     bool canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;
     bool canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;

   public:
     King(Square p, bool c);
     std::shared_ptr<Piece> clone() const override;
     bool canCastleShort(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces);
     bool canCastleLong(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces);
     virtual char getName() const noexcept override;
};

class Queen : public Piece {
     bool canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;
     bool canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;

   public:
     Queen(Square p, bool c);
     std::shared_ptr<Piece> clone() const override;
     virtual char getName() const noexcept override;
};

class Bishop : public Piece {
     bool canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;
     bool canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;

   public:
     Bishop(Square p, bool c);
     std::shared_ptr<Piece> clone() const override;
     virtual char getName() const noexcept override;
};

class Knight : public Piece {
     bool canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;
     bool canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;

   public:
     Knight(Square p, bool c);
     std::shared_ptr<Piece> clone() const override;
     virtual char getName() const noexcept override;
};

class Rook : public Piece {
     bool canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;
     bool canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) override;

   public:
     Rook(Square p, bool c);
     std::shared_ptr<Piece> clone() const override;
     virtual char getName() const noexcept override;
};

#endif
