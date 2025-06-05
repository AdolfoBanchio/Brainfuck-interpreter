# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Project files
SRCS = main.cpp ast.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = brainfuck

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)


# Phony targets
.PHONY: all clean