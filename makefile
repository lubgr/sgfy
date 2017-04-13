
BUILD ?= build
EXEC = $(BUILD)/runtests
HEADER = sgfy.h

CXXFLAGS ?= -std=c++11 -O0 -g

LIBS ?= -lCppUTest

TESTOBJ = $(BUILD)/tests.o

$(EXEC): $(TESTOBJ) $(BUILD)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(TESTOBJ) $(LDFLAGS) $(LIBS)

$(BUILD)/%.o: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -I src -o $@ -c $<

$(BUILD):
	mkdir $(BUILD)

clean:
	$(RM) $(EXEC) $(TESTOBJ)

.PHONY: clean
