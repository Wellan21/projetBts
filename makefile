# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Source files
SRCS = clientSocket.cpp cmodelisateur.cpp main.cpp

# Header files
HDRS = clientSocket.h cmodelisateur.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = demonstrateur

# Default target
all: $(EXEC)

# Link object files to create executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove compiled files
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean
