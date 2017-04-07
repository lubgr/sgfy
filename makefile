
BUILD ?= build
EXEC = $(BUILD)/runtests
HEADER = src/sgfy.h

CXXFLAGS ?= -std=c++11

LIBS ?= -lCppUTest

TESTSRC = $(wildcard test/*.cpp)
TESTOBJ = $(TESTSRC:test/%.cpp=$(BUILD)/%.o)

$(EXEC): $(HEADER) $(TESTOBJ) $(BUILD)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(TESTOBJ) $(LDFLAGS) $(LIBS)

$(BUILD)/%.o: test/%.cpp
	$(CXX) $(CXXFLAGS) -I src -o $@ -c $<

$(BUILD):
	mkdir $(BUILD)

clean:
	$(RM) $(EXEC) $(TESTOBJ)

.PHONY: clean
