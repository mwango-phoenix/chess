#include <climits>
#include <iomanip>
#include "strategy.h"

using namespace std;

MinimaxStrategy::MinimaxStrategy(int d) : maxDepth(d) {}

ChessMove MinimaxStrategy::findMove(const ChessBoard& board) const {
    ChessMove move;
    double eval = board.getSideToMove() ? 
        maximize(board, move, 0) : 
        minimize(board, move, 0);
    clog << showpos << fixed << setprecision(2);
    clog << eval << '\n';
    return move;
}

double MinimaxStrategy::evaluate(const ChessBoard& board) const {
    double eval = 0;
    for (int f = 0; f < MAX_FILE; ++f) {
        for (int r = 0; r < MAX_RANK; ++r) {
            switch (board.getPiece({f, r})) {
                case 'K': eval += 0; break; case 'k': eval -= 0; break;
                case 'Q': eval += 9; break; case 'q': eval -= 9; break;
                case 'B': eval += 3; break; case 'b': eval -= 3; break;
                case 'N': eval += 3; break; case 'n': eval -= 3; break;
                case 'R': eval += 5; break; case 'r': eval -= 5; break;
                case 'P': eval += 1; break; case 'p': eval -= 1; break;
            }
        }
    }
    return eval;
}

double MinimaxStrategy::maximize(const ChessBoard& board, ChessMove& move, 
    int depth, double alpha, double beta) const {
    // Check if the position has a result
    ChessResult state = board.getResult();
    switch (state) {
        case ChessResult::WHITE: return INT_MAX;
        case ChessResult::BLACK: return INT_MIN;
        case ChessResult::DRAW: return 0;
        case ChessResult::LINE:
        if (depth == maxDepth) return evaluate(board);
        break;
    }
    
    vector<ChessMove> legalMoves = std::move(findLegalMoves(board));
    double maxEval = -INFINITY;
    int i = 0;
    for (const ChessMove& m : legalMoves) {
        ChessBoard newBoard{board};
        newBoard.makeMove(m.start, m.dest, m.promotion);
        ChessMove newMove;
        double eval = minimize(newBoard, newMove, depth + 1, alpha, beta);
        if (eval == maxEval) {
            if (!(rand() % ++i)) move = m; // uniformly random move if equal, see [1]
        } else if (eval > maxEval) {
            maxEval = eval;
            if (eval > alpha) {
                alpha = eval;
                if (alpha > beta) return eval;
            }
            move = m;
            i = 1;
        }
    }
    
    return maxEval;

}

double MinimaxStrategy::minimize(const ChessBoard& board, ChessMove& move, 
    int depth, double alpha, double beta) const {
    // Check if the position has a result
    ChessResult state = board.getResult();
    switch (state) {
        case ChessResult::WHITE: return INT_MAX;
        case ChessResult::BLACK: return INT_MIN;
        case ChessResult::DRAW: return 0;
        case ChessResult::LINE:
        if (depth == maxDepth) return evaluate(board);
        break;
    }
    
    vector<ChessMove> legalMoves = findLegalMoves(board);
    double minEval = INFINITY;
    int i = 0;
    for (const ChessMove& m : legalMoves) {
        ChessBoard newBoard{board};
        newBoard.makeMove(m.start, m.dest, m.promotion);
        ChessMove newMove;
        double eval = maximize(newBoard, newMove, depth + 1, alpha, beta);
        if (eval == minEval) {
            if (!(rand() % ++i)) move = m; // uniformly random move if equal, see [1]
        } else if (eval < minEval) {
            minEval = eval;
            if (eval < beta) {
                beta = eval;
                if (alpha > beta) return eval;
            }
            move = m;
            i = 1;
        }
    }
    
    return minEval;

}

/* [1]
Claim:
    For all natural number n, if for each natural number 1 <= i <= n in increasing order, 
    we assign X = i with probability 1 / i, then X follows a discrete uniform distribution on i
Proof:
    Base case:
        For n = 1, we assign X = 1 with probability 1. X is uniformly distributed on {1}.
    Inductive step:
        Assume X follows a discrete uniform distribution on 1 <= i <= n for some n, that is, 
        the probability function f(x) = P(X = x) = 1 / n for all 1 <= x <= n.
        Let X' be the value of X after possibly assigning X = n + 1 with probability 1 / (n + 1).
        The probability that X' = n + 1 is 
            g(n + 1) = P(X' = n + 1) = 1 / (n + 1);
        the probability that X' = x for 1 <= x <= n is 
            g(x) = P(X' = x) = (1 - P(X' = n + 1))P(X = x) = (1 - 1 / (n + 1))(1 / n) = 1 / (n + 1).
        Thus, the probability function g(x) = P(X' = x) = 1 / (n + 1) for all 1 <= x <= n + 1, that is, 
            X' is uniformly distributed on {1, 2, ..., n + 1}.
*/
