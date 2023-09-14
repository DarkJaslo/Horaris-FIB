SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := .
EXE := $(BIN_DIR)/horaris.exe

SRC = $(SRC_DIR)/main.cc $(SRC_DIR)/Parser.cc $(SRC_DIR)/Horari.cc $(SRC_DIR)/Data.cc
OBJ = $(SRC:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

DEBUG = -g -O0 -fno-inline -D_GLIBCXX_DEBUG
RUN = -O3

CXX = g++
CXXFLAGS = -Wall -Wno-sign-compare $(RUN)

.PHONY: all clean
all: $(EXE) 

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR) :
	mkdir -p $@

clean: 
	rm *.o *.exe
	@rm -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)