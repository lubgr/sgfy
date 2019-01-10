
include $(wildcard makefile.in Makefile.in)

BUILD ?= build
EXEC = $(BUILD)/runtests
HEADER = sgfy.h

CXXFLAGS ?= -std=c++11 -O0 -g -pedantic -Werror -Wsign-conversion -Wall -Wextra
CXX ?= g++

LIBS ?= -lCppUTest

OBJ = $(BUILD)/tests.o $(BUILD)/sgfy.o

$(EXEC): $(OBJ) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS) $(LIBS)

$(BUILD)/%.o: %.cpp $(HEADER) | $(BUILD)
	$(CXX) $(CXXFLAGS) -I src -o $@ -c $<

$(BUILD):
	mkdir -p $(BUILD)

clean:
	$(RM) $(EXEC) $(OBJ)

.PHONY: clean
