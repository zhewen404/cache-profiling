# makefile for the project
CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -g

BIN     := bin
SRC     := src
INCLUDE := src
LIB     := lib
LIBRARIES   := 
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	@echo "🚀 Executing..."
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*/*.cc $(SRC)/*/*.hh $(SRC)/*/*/*.cc $(SRC)/*/*/*.hh
	@echo "🚧 Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@echo "🧹 Clearing..."
	-rm $(BIN)/*

