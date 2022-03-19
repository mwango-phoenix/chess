#include "piece.h"

using namespace std;

// Helper functions
int abs(int n) {
        if (n >= 0) return n;
        else return -n;
}

// Square methods
bool Square::operator==(const Square &s) const noexcept {
        return file == s.file && rank == s.rank;
}

istream& operator>>(istream& in, Square& s) {
        while (in.peek() == ' ') in.ignore();
        char file = in.get();
        char rank = in.get();
        if (file >= 'a' && file < 'a' + MAX_FILE 
                && rank >= '1' && rank < '1' + MAX_RANK)
        {
                s.file = file - 'a';
                s.rank = rank - '1';
        } else {
                in.setstate(ios_base::failbit);
        }
        return in;
}

ostream& operator<<(ostream& out, const Square& s) {
        char file = s.file + 'a';
        char rank = s.rank + '1';
        if (file >= 'a' && file < 'a' + MAX_FILE 
                && rank >= '1' && rank < '1' + MAX_RANK)
        {
                out.put(file);
                out.put(rank);
        } else {
                out.put('.'); out.put('.');
        }
        return out;
}

// Piece methods
Piece::Piece(Square p, bool c) : colour{c}, position{p} {}

bool Piece::getColour() const noexcept {
        return colour;
}

void Piece::setPos(Square pos) noexcept {
        position = pos;
}

bool Piece::isLegal(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces){
        return canMove(dest, pieces) || canCapture(dest, pieces);
}

// iterator helper function to check if spaces between position and dest are empty
bool pathIsEmpty(const int fileDelta, const int rankDelta, const Square dest, const Square position, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
    int fileDir;
    int rankDir;
    // setting fileDir
    if (fileDelta > 0) fileDir = 1;
    else if (fileDelta < 0) fileDir = -1;
    else fileDir = 0;
    // setting rankDir
    if (rankDelta > 0) rankDir = 1;
    else if (rankDelta < 0)  rankDir = -1;
    else rankDir = 0;
    // looping through the squares to check for pieces between position and dest
    int fileIt = position.file + fileDir;
    int rankIt = position.rank + rankDir;
    while (fileIt != dest.file || rankIt != dest.rank) {
        if (pieces[fileIt][rankIt]) {
            return false;
        }
        fileIt += fileDir;
        rankIt += rankDir;
    }
    return true;
}

// Pawn methods
Pawn::Pawn(Square p, bool c) : Piece{p, c} {}

shared_ptr<Piece> Pawn::clone() const {
        return make_shared<Pawn>(*this);
}

bool Pawn::canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        if (!pieces[dest.file][dest.rank]) {
                if (dest.file == position.file) {
                        if (colour) {
                                if (dest.rank == position.rank + 2) {
                                        return position.rank == 1 && !pieces[dest.file][dest.rank - 1];
                                } else {
                                        return dest.rank == position.rank + 1;
                                }
                        } else {
                                if (dest.rank == position.rank - 2) {
                                        return position.rank == 6 && !pieces[dest.file][dest.rank + 1];
                                } else {
                                        return dest.rank == position.rank - 1;
                                }
                        }
                }
        }
        return false;
}

bool Pawn::canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        if (colour) {
                if ((dest.file + 1 == position.file || dest.file - 1 == position.file) && dest.rank == position.rank + 1) {
                        return pieces[dest.file][dest.rank] && !(pieces[dest.file][dest.rank]->getColour());
                }
        } else {
                if ((dest.file + 1 == position.file || dest.file - 1 == position.file) && dest.rank == position.rank - 1) {
                        return pieces[dest.file][dest.rank] && pieces[dest.file][dest.rank]->getColour();
                }
        }
        return false;
}

bool Pawn::canEnPassant(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        int fileDelta = dest.file - position.file;
        int rankDelta = dest.rank - position.rank;
        if (abs(rankDelta) == 1 && abs(fileDelta) == 1) {
                if (colour) {
                        return pieces[dest.file][dest.rank - 1] && !(pieces[dest.file][dest.rank - 1]->getColour());
                } else {
                        return pieces[dest.file][dest.rank + 1] && pieces[dest.file][dest.rank + 1]->getColour();
                }
        }
        return false;
}

char Pawn::getName() const noexcept {
        if (colour)
                return 'P';
        else
                return 'p';
}

// King methods
King::King(Square p, bool c) : Piece{p, c} {}

shared_ptr<Piece> King::clone() const {
        return make_shared<King>(*this);
}

bool King::canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        if (!pieces[dest.file][dest.rank]) {
                int fileDelta = dest.file - position.file;
                int rankDelta = dest.rank - position.rank;
                if ((fileDelta >= -1 && fileDelta <= 1) && (rankDelta >= -1 && rankDelta <= 1)) {
                        return !(dest == position);
                }
        }
        return false;
}

bool King::canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        int fileDelta = dest.file - position.file;
        int rankDelta = dest.rank - position.rank;
        if ((fileDelta >= -1 && fileDelta <= 1) && (rankDelta >= -1 && rankDelta <= 1) && !(dest == position)) {
                return pieces[dest.file][dest.rank] && (pieces[dest.file][dest.rank]->getColour() != colour);
        }
        return false;
}

bool King::canCastleShort(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        // checks if destination square is g1 if white
        if (colour && dest == Square{6, 0}) {
                // check if wKing is on e1 and wRook is on h1
                if (position == Square{4, 0} && pieces[7][0]->getName() == 'R') {
                        // checks if space between pieces is empty
                        return !(pieces[5][0] || pieces[6][0]);
                }
        //checks if destination square is g8 if black
        } else if (!colour && dest == Square{6, 7}) {
                // check if bKing is on e8 and bRook is on h8
                if (position == Square{4, 7} && pieces[7][7]->getName() == 'r') {
                        // checks if space between pieces is empty
                        return !(pieces[5][7] || pieces[6][7]);
                }
        }
        return false;
}

bool King::canCastleLong(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        if (colour && dest == Square{2, 0}) {
                // check if wKing is on e1 and wRook is on a1
                if (position == Square{4, 0} && pieces[0][0]->getName() == 'R') {
                        // checks if space between pieces is empty
                        return !(pieces[1][0] || pieces[2][0] || pieces[3][0]);
                }
        } else if (!colour && dest == Square{2, 7}) {
                // check if bKing is on e8 and bRook is on a8
                if (position == Square{4, 7} && pieces[0][7]->getName() == 'r') {
                        // checks if space between pieces is empty
                        return !(pieces[1][7] || pieces[2][7] || pieces[3][7]);
                }
        }
        return false;
}

char King::getName() const noexcept {
        if (colour)
                return 'K';
        else
                return 'k';
}

// Queen methods
Queen::Queen(Square p, bool c) : Piece{p, c} {}

shared_ptr<Piece> Queen::clone() const {
        return make_shared<Queen>(*this);
}

bool Queen::canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        if (!pieces[dest.file][dest.rank]) {
                int fileDelta = dest.file - position.file;
                int rankDelta = dest.rank - position.rank;
                // either moves diagonally, vertically, or horizontally
                if ((abs(fileDelta) == abs(rankDelta) || fileDelta == 0 || rankDelta == 0) && !(dest == position)) {
                        return pathIsEmpty(fileDelta, rankDelta, dest, position, pieces);
                }
        }
        return false;
}

bool Queen::canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
    if (pieces[dest.file][dest.rank] && (pieces[dest.file][dest.rank]->getColour() != colour)) {
        int fileDelta = dest.file - position.file;
        int rankDelta = dest.rank - position.rank;
        if ((abs(fileDelta) == abs(rankDelta) || fileDelta == 0 || rankDelta == 0) && !(dest == position)) {
            return pathIsEmpty(fileDelta, rankDelta, dest, position, pieces);
        }   
    }
    return false;
}


char Queen::getName() const noexcept {
        if (colour)
                return 'Q';
        else
                return 'q';
}

// Rook methods
Rook::Rook(Square p, bool c) : Piece{p, c} {}

shared_ptr<Piece> Rook::clone() const {
        return make_shared<Rook>(*this);
}

bool Rook::canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        if (!pieces[dest.file][dest.rank]) {
                int fileDelta = dest.file - position.file;
                int rankDelta = dest.rank - position.rank;
                // either moves vertically or horizontally
                if ((fileDelta == 0 || rankDelta == 0) && !(dest == position)) {
                        return pathIsEmpty(fileDelta, rankDelta, dest, position, pieces);
                }
        }
        return false;
}

bool Rook::canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
    if (pieces[dest.file][dest.rank] && (pieces[dest.file][dest.rank]->getColour() != colour)) {
        int fileDelta = dest.file - position.file;
        int rankDelta = dest.rank - position.rank;
        if ((fileDelta == 0 || rankDelta == 0) && !(dest == position)) {
                return pathIsEmpty(fileDelta, rankDelta, dest, position, pieces);
        }
    }
    return false;
}

char Rook::getName() const noexcept {
        if (colour)
                return 'R';
        else
                return 'r';
}

// Bishop methods
Bishop::Bishop(Square p, bool c) : Piece{p, c} {}

shared_ptr<Piece> Bishop::clone() const {
        return make_shared<Bishop>(*this);
}

bool Bishop::canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        if (!pieces[dest.file][dest.rank]) {
                int fileDelta = dest.file - position.file;
                int rankDelta = dest.rank - position.rank;
                // checks if destination is diagonal to current position
                if (abs(fileDelta) == abs(rankDelta) && !(dest == position)) {
                        return pathIsEmpty(fileDelta, rankDelta, dest, position, pieces);
                }
        }
        return false;
}

bool Bishop::canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
    if (pieces[dest.file][dest.rank] && (pieces[dest.file][dest.rank]->getColour() != colour)) {
        int fileDelta = dest.file - position.file;
        int rankDelta = dest.rank - position.rank;
        if (abs(fileDelta) == abs(rankDelta) && !(dest == position)) {
                return pathIsEmpty(fileDelta, rankDelta, dest, position, pieces);
        }
    }
    return false;
}

char Bishop::getName() const noexcept {
        if (colour)
                return 'B';
        else
                return 'b';
}

// Knight methods
Knight::Knight(Square p, bool c) : Piece{p, c} {}

shared_ptr<Piece> Knight::clone() const {
        return make_shared<Knight>(*this);
}

bool Knight::canMove(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        if (!pieces[dest.file][dest.rank]) {
                int fileDelta = dest.file - position.file;
                int rankDelta = dest.rank - position.rank;
                return (abs(fileDelta) == 2 && abs(rankDelta) == 1) || (abs(fileDelta) == 1 && abs(rankDelta) == 2);
        }
        return false;
}

bool Knight::canCapture(Square dest, const std::vector<std::vector<std::shared_ptr<Piece>>> &pieces) {
        int fileDelta = dest.file - position.file;
        int rankDelta = dest.rank - position.rank;
        if ((abs(fileDelta) == 2 && abs(rankDelta) == 1) || (abs(fileDelta) == 1 && abs(rankDelta) == 2)) {
                return pieces[dest.file][dest.rank] && (pieces[dest.file][dest.rank]->getColour() != colour);
        }
        return false;
}

char Knight::getName() const noexcept {
        if (colour)
                return 'N';
        else
                return 'n';
}
