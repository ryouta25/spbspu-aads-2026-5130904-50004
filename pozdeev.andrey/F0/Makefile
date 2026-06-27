CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Wold-style-cast -Wno-missing-field-initializers -O2

APP_EXE = app
TEST_EXE = tests

APP_SRC = main.cpp application.cpp scenario.cpp
APP_OBJ = $(APP_SRC:.cpp=.o)

TEST_SRC = test-main.cpp test-avl_tree.cpp test-scenario.cpp test-vector.cpp scenario.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)

all: $(APP_EXE) $(TEST_EXE)

$(APP_EXE): $(APP_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_EXE): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(APP_EXE) $(TEST_EXE)
