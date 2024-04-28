# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -D_REENTRANT

# Libraries (using wildcard for linking)
LDLIBS = -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -Wl,-rpath,/usr/local/lib64 -Wl,--enable-new-dtags

# Source directory and object files
SRC_DIR = ./src/
SRCS = $(wildcard $(SRC_DIR)*.cpp)
OBJS = $(SRCS:.cpp=.o)

# Phony targets
.PHONY: clean all

# Main executable target
all: main

# Build the executable
main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

# Compile individual object files
$(OBJS): $(SRC_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean target to remove object files
clean:
	rm -f $(OBJS)
