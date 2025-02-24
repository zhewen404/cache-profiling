# makefile for the project
CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -g

BIN     := bin
SRC     := src
INCLUDE := src
LIB     := lib
LIBRARIES   := 
EXECUTABLE  := main

SRC_DIR := src
EXCLUDE_DIR := src/test
# Collect all .cc files except those in the excluded directory
SRC_FILES := $(shell find $(SRC_DIR) -type f \( -name "*.cc" -o -name "*.hh" \) ! -path "$(EXCLUDE_DIR)/*")
#$(SRC)/*/*.cc $(SRC)/*/*.hh $(SRC)/*/*/*.cc $(SRC)/*/*/*.hh

ifeq ($(TEST),)
    FINAL_SRC_FILES := $(SRC_FILES)
else
    FINAL_SRC_FILES := $(SRC_FILES) $(EXCLUDE_DIR)/$(TEST).cc
endif

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	@echo "🚀 Executing..."
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(FINAL_SRC_FILES) 
	@echo "🚧 Building..."
	mkdir -p $(BIN)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@echo "🧹 Clearing..."
	-rm $(BIN)/*

