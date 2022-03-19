CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -MMD

SRC = piece.cc board.cc subject.cc display.cc window.cc player.cc strategy.cc randommove.cc capturecheck.cc avoidcapture.cc minimax.cc boardeditor.cc game.cc
OBJ = $(SRC:.cc=.o)
DEP = $(SRC:.cc=.d)
EXE = chess test

$(EXE): %: %.o $(OBJ)
	$(CXX) $^ -o $@ -lX11
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP)

.PHONY: clean
clean:
	rm $(OBJ) $(DEP) $(EXE:=.o) $(EXE:=.d) $(EXE)
