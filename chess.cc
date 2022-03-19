#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include "board.h"
#include "boardeditor.h"
#include "game.h"
#include "player.h"
#include "strategy.h"
#include "display.h"

using namespace std;

int main() {
    ChessBoard board;
    ChessGame game{board};
    
    TextDisplay textDisplay{&board};
    GraphicDisplay graphicDisplay{&board};

    HumanPlayer human{cin};
    vector<ComputerPlayer> computers{
        {RandomMoveStrategy{}}, 
        {CaptureCheckStrategy{}}, 
        {AvoidCaptureStrategy{}}, 
        {MinimaxStrategy{1}}, {MinimaxStrategy{2}}, {MinimaxStrategy{4}}
    };

    ChessBoardEditor editor;
    editor.resetBoard(board);
    string line;
    while (getline(cin, line)) {
        istringstream in{line};
        string token;
        if (in >> token) {
            string white, black;
            if (token == "game") {
                in >> white >> black;
                if (white == "human") {
                    game.setWhite(&human);
                } else if (white.size() == 11 
                    && white.substr(0, 8) == "computer" 
                    && white[9] >= '1' && white[9] <= '6' 
                    && white[8] == '[' && white[10] == ']')
                {
                    game.setWhite(&computers[white[9] - '1']);
                } else {
                    cerr << "Unknown player: " << white << endl;
                }
                if (black == "human") {
                    game.setBlack(&human);
                } else if (black.size() == 11 
                    && black.substr(0, 8) == "computer" 
                    && black[9] >= '1' && black[9] <= '6' 
                    && black[8] == '[' && black[10] == ']')
                {
                    game.setBlack(&computers[black[9] - '1']);
                } else {
                    cerr << "Unknown player: " << black << endl;
                }
                clog << "GAME START" << endl;
                game.runGame(cout);
                clog << "GAME FINISH" << endl;
            } else if (token == "reset") {
                editor.resetBoard(board);
                clog << "BOARD RESET" << endl;
            } else if (token == "setup") {
                editor.setUpBoard(board, cin);
                clog << "BOARD SET" << endl;
            } else if (token == "load") {
                string fen;
                getline(cin, fen);
                editor.loadBoard(board, fen);
                clog << "BOARD LOADED" << endl;
            } else {
                cerr << "Invalid token: " << token << endl;
            }
        }
    }

    game.printScore(cout);

    return 0;

}
