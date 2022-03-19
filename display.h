#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "board.h"
#include "observer.h"
#include "window.h"
#include <memory>

// [Observer]

class ChessDisplay : public Observer {
    protected:
      ChessBoard* board; 
    public:
      ChessDisplay(ChessBoard* board);
};

class TextDisplay : public ChessDisplay {
    public:
      TextDisplay( ChessBoard* board );
      ~TextDisplay();
      void notify() override;

};

class GraphicDisplay : public ChessDisplay {
    std::unique_ptr<Xwindow> w;
    public:
      GraphicDisplay( ChessBoard* board );
      ~GraphicDisplay();
      void notify() override;
};

#endif