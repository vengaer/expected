CXX ?= g++
BIN = expected_test

SRC = $(wildcard tests/*.cc)
OBJ := $(addsuffix .o, $(basename $(SRC)))

INC = -I tests/ -I include

export CPPFLAGS

CXXFLAGS := $(CXXFLAGS) -std=c++17 -Wall -Wextra -pedantic -Weffc++ -Wshadow -Wunknown-pragmas $(INC)

$(BIN): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean run manual lint static_check

clean:
	rm -f $(OBJ) $(BIN)

run: $(BIN)
	./$(BIN)

manual: CPPFLAGS+=-D EXPECTED_MANUAL_TEST
manual: $(BIN)

lint:
	clang-tidy tests/main.cc -header-filter=.* -extra-arg=-std=c++17 -- $(INC) -D EXPECTED_MANUAL_TEST

static_check:
	cppcheck --enable=all --inconclusive --language=c++ include/expected.h
