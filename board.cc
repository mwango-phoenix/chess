#include "board.h"

using namespace std;

// class ChessBoard : public Subject

ChessBoard::ChessBoard() : 
    pieces(MAX_FILE, vector<shared_ptr<Piece>>(MAX_RANK))
{}

ChessBoard::ChessBoard(const ChessBoard& board) : ChessBoard() {
    for (int f = 0; f < MAX_FILE; ++f) {
        for (int r = 0; r < MAX_RANK; ++r) {
            if (board.pieces[f][r]) {
                pieces[f][r] = board.pieces[f][r]->clone();
            }
        }
    }
    sideToMove = board.sideToMove;
    wCastling = board.wCastling;
    bCastling = board.bCastling;
    enPassant = board.enPassant;
    enPassantLocation = board.enPassantLocation;
}

bool ChessBoard::isValid(Square pos) const noexcept {
    return pos.file >= 0 && pos.file < MAX_FILE
        && pos.rank >= 0 && pos.rank < MAX_RANK;
}

bool ChessBoard::getSideToMove() const noexcept {
    return sideToMove;
}

char ChessBoard::getPiece(Square pos) const noexcept {
    if (!isValid(pos)) return 0;

    shared_ptr<Piece> piece = pieces[pos.file][pos.rank];
    return piece ? piece->getName() : 0;

}

char ChessBoard::addPiece(Square pos, char name, bool notify) {
    if (!isValid(pos)) return 0;

    shared_ptr<Piece> piece = pieces[pos.file][pos.rank];
    removePiece(pos);
    bool colour = name >= 'A' && name <= 'Z';
    switch (name) {
        case 'K': case 'k':
        piece = make_shared<King>(pos, colour);
        break;
        case 'Q': case 'q':
        piece = make_shared<Queen>(pos, colour);
        break;
        case 'B': case 'b':
        piece = make_shared<Bishop>(pos, colour);
        break;
        case 'N': case 'n':
        piece = make_shared<Knight>(pos, colour);
        break;
        case 'R': case 'r':
        piece = make_shared<Rook>(pos, colour);
        break;
        case 'P': case 'p':
        piece = make_shared<Pawn>(pos, colour);
        break;
    }
    pieces[pos.file][pos.rank] = piece;

    if (notify) notifyObservers();

    return piece ? piece->getName() : 0;

}

char ChessBoard::removePiece(Square pos, bool notify) {
    if (!isValid(pos)) return 0;

    shared_ptr<Piece> piece = pieces[pos.file][pos.rank];
    if (pos.rank == 0) {
        if (pos.file == 4 || pos.file == 7) wCastling.first = false;
        if (pos.file == 4 || pos.file == 0) wCastling.second = false;
    } else if (pos.rank == 7) {
        if (pos.file == 4 || pos.file == 7) bCastling.first = false;
        if (pos.file == 4 || pos.file == 0) bCastling.second = false;
    } else if (pos.file == enPassantLocation.file && pos.rank == (sideToMove ? 4 : 3)) {
        enPassant = false;
    }
    pieces[pos.file][pos.rank].reset();

    if (notify) notifyObservers();

    return piece ? piece->getName() : 0;

}

char ChessBoard::movePiece(Square start, Square dest, bool notify) {
    if (!isValid(start) || !isValid(dest)) return 0;

    shared_ptr<Piece> piece = pieces[start.file][start.rank];
    removePiece(dest);
    removePiece(start);
    pieces[dest.file][dest.rank] = piece;
    pieces[dest.file][dest.rank]->setPos(dest);

    if (notify) notifyObservers();

    return piece ? piece->getName() : 0;

}

void ChessBoard::setSideToMove(bool colour) noexcept {
    if (colour != sideToMove) {
        enPassant = false;
        sideToMove = colour;
    }
}

void ChessBoard::setCastling(bool colour, bool castleSide) noexcept {
    shared_ptr<Piece> piece = pieces[4][colour ? 0 : 7];
    if (dynamic_pointer_cast<King>(piece) && piece->getColour() == colour) {
        piece = pieces[castleSide ? 7 : 0][colour ? 0 : 7];
        if (dynamic_pointer_cast<Rook>(piece) && piece->getColour() == colour) {
            pair<bool, bool> &castling = 
                colour ? wCastling : bCastling;
            (castleSide ? castling.first : castling.second) = true;
        }
    }
}

void ChessBoard::setEnPassant(Square pos) noexcept {
    if (!isValid(pos)) return;

    if (pos.rank == (sideToMove ? 5 : 2)) {
        shared_ptr<Piece> piece = pieces[pos.file][sideToMove ? 4 : 3];
        if (dynamic_pointer_cast<Pawn>(piece) && piece->getColour() != sideToMove) {
            enPassant = true;
            enPassantLocation = pos;
        }
    }

}

ChessResult ChessBoard::getResult() const {
    if (!hasMaterial()) { // [bonus] Insufficient material
        return ChessResult::DRAW;
    } else if (hasLegalMove()) {
        return ChessResult::LINE;
    } else if (isInCheck()) {
        return sideToMove ? ChessResult::BLACK : ChessResult::WHITE;
    } else {
        return ChessResult::DRAW;
    }
}

bool ChessBoard::hasLegalMove() const {
    for (int fs = 0; fs < MAX_FILE; ++fs) {
        for (int rs = 0; rs < MAX_RANK; ++rs) {
            shared_ptr<Piece> piece = pieces[fs][rs];
            if (piece && piece->getColour() == sideToMove) {
                Square pos = {fs, rs};
                for (int fd = 0; fd < MAX_FILE; ++fd) {
                    for (int rd = 0; rd < MAX_RANK; ++rd) {
                        if (isLegal(pos, {fd, rd})) return true;
                    }
                }
            }
        }
    }
    return false;
}

bool ChessBoard::hasMaterial() const {
    int count = 0;
    for (int f = 0; f < MAX_FILE; ++f) {
        for (int r = 0; r < MAX_RANK; ++r) {
            shared_ptr<Piece> piece = pieces[f][r];
            if (piece) {
                if (++count > 3) return true;
                if (dynamic_pointer_cast<Pawn>(piece) 
                    || dynamic_pointer_cast<Rook>(piece) 
                    || dynamic_pointer_cast<Queen>(piece))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ChessBoard::isValid() const {
    // No pawns on the back ranks
    for (int f = 0; f < MAX_FILE; ++f) {
        if (dynamic_pointer_cast<Pawn>(pieces[f][0]) 
            || dynamic_pointer_cast<Pawn>(pieces[f][7]))
        {
            return false;
        }
    }

    bool wKing = false, bKing = false;
    Square kingPos;
    for (int f = 0; f < MAX_FILE; ++f) {
        for (int r = 0; r < MAX_RANK; ++r) {
            shared_ptr<Piece> piece = pieces[f][r];
            if (dynamic_pointer_cast<King>(piece)) {
                if (piece->getColour() != sideToMove) {
                    kingPos = {f, r};
                }
                bool &king = piece->getColour() ? wKing : bKing;
                if (king) return false;
                king = true;
            }
        }
    }
    if (!wKing || !bKing) return false;
    for (int f = 0; f < MAX_FILE; ++f) {
        for (int r = 0; r < MAX_RANK; ++r) {
            shared_ptr<Piece> piece = pieces[f][r];
            if (piece && piece->isLegal(kingPos, pieces)) {
                return false;
            }
        }
    }

    return true;

}

bool ChessBoard::isInCheck() const {
    return isInCheck(pieces);
}

bool ChessBoard::isInCheck(const vector<vector<shared_ptr<Piece>>>& pieces) const {
    // Find the king
    for (int fd = 0; fd < MAX_FILE; ++fd) {
        for (int rd = 0; rd < MAX_RANK; ++rd) {
            shared_ptr<Piece> piece = pieces[fd][rd];
            if (dynamic_pointer_cast<King>(piece) && piece->getColour() == sideToMove) {
                Square kingPos = {fd, rd};
                for (int fs = 0; fs < MAX_FILE; ++fs) {
                    for (int rs = 0; rs < MAX_RANK; ++rs) {
                        piece = pieces[fs][rs];
                        if (piece && piece->isLegal(kingPos, pieces)) {
                            return true;
                        }
                    }
                }
                return false;
            }
        }
    }
    return false;
}

bool ChessBoard::isLegal(Square start, Square dest) const {
    if (!isValid(start) || !isValid(dest)) return false;

    shared_ptr<Piece> piece = pieces[start.file][start.rank];
    if (!piece || piece->getColour() != sideToMove) return false;

    vector<vector<shared_ptr<Piece>>> newPieces{pieces};
    if (piece->isLegal(dest, pieces)) {
        newPieces[dest.file][dest.rank] = move(newPieces[start.file][start.rank]);
    } else {
        // Check for special moves
        if (auto pawn = dynamic_pointer_cast<Pawn>(piece)) {
            // Check for en passant
            if (enPassant && dest == enPassantLocation) {
                if (pawn->canEnPassant(dest, pieces)) {
                    newPieces[dest.file][start.rank].reset();
                    newPieces[dest.file][dest.rank] = move(newPieces[start.file][start.rank]);
                    return !isInCheck(newPieces);
                }
            }
        } else if (auto king = dynamic_pointer_cast<King>(piece)) {
            // Check for castling
            const pair<bool, bool>& castling = 
                sideToMove ? wCastling : bCastling;
            if ((castling.first || castling.second) && !isInCheck()) {
                if (castling.first && king->canCastleShort(dest, pieces)) {
                    for (int f = start.file + 1; f <= dest.file; ++f) {
                        newPieces[f][start.rank] = move(newPieces[f - 1][start.rank]);
                        if (isInCheck(newPieces)) return false;
                    }
                    return true;
                } else if (castling.second && king->canCastleLong(dest, pieces)) {
                    for (int f = start.file - 1; f >= dest.file; --f) {
                        newPieces[f][start.rank] = move(newPieces[f + 1][start.rank]);
                        if (isInCheck(newPieces)) return false;
                    }
                    return true;
                }
            }
        }
        return false;
    }

    return !isInCheck(newPieces);

}

bool ChessBoard::makeMove(Square start, Square dest, char promotion) {
    if (!isLegal(start, dest)) return false;

    shared_ptr<Piece> piece = pieces[start.file][start.rank];
    if (auto pawn = dynamic_pointer_cast<Pawn>(piece)) {
        if (dest.rank == (sideToMove ? 7 : 0)) {
            // Handle promotion
            if (!promotion) promotion = (sideToMove ? 'Q' : 'q');
            if (promotion == (sideToMove ? 'Q' : 'q')
                || promotion == (sideToMove ? 'N' : 'n') 
                || promotion == (sideToMove ? 'B' : 'b') 
                || promotion == (sideToMove ? 'R' : 'r'))
            {
                addPiece(start, promotion);
            } else return false;
        } else if (dest.file != start.file && !pieces[dest.file][dest.rank]) {
            // Handle en passant
            removePiece({dest.file, start.rank});
        }
    } else if (auto king = dynamic_pointer_cast<King>(piece)) {
        if (king->canCastleShort(dest, pieces)) {
            // Handle short castle
            movePiece({7, dest.rank}, {dest.file - 1, dest.rank});
        } else if (king->canCastleLong(dest, pieces)) {
            // Handle long castle
            movePiece({0, dest.rank}, {dest.file + 1, dest.rank});
        }
    }
    movePiece(start, dest);

    enPassant = false;
    if (dynamic_pointer_cast<Pawn>(piece)) {
        if (dest.rank - start.rank == (sideToMove ? 2 : -2)) {
            enPassant = true;
            enPassantLocation = {start.file, start.rank + (sideToMove ? 1 : -1)};
        }
    }
    sideToMove = !sideToMove;

    notifyObservers();

    return true;

}
