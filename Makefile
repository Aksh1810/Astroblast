SFML_DIR = /opt/homebrew/Cellar/sfml/2.6.2

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I$(SFML_DIR)/include
LDFLAGS = -L$(SFML_DIR)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC = main.cpp game.cpp
OBJ = $(SRC:.cpp=.o)

EXEC = sfml

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run