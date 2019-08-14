CXX ?= g++
BIN = expected_test

SRC = $(wildcard tests/*.cc)
OBJ := $(addsuffix .o, $(basename $(SRC)))

INC = -I tests/ -I include

export CPPFLAGS

CXXFLAGS := $(CXXFLAGS) -std=c++17 -Wall -Wextra -pedantic -Weffc++ $(INC)

$(BIN): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean run manual

clean:
	rm -f $(OBJ) $(BIN)

run: $(BIN)
	./$(BIN)

manual: CPPFLAGS+=-D EXPECTED_MANUAL_TEST
manual: $(BIN)
