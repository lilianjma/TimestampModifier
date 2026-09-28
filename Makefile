CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

TARGET := modify_timestamps
SRC := modify_timestamps.cpp

DIR ?=

.PHONY: all compile run incr decr runall deprocess clean

# --------------------------------------------------
# Default: compile only
# make
# --------------------------------------------------
all: compile

# --------------------------------------------------
# Compile
# make compile
# --------------------------------------------------
compile: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# --------------------------------------------------
# Update photos using current stored date
# make run DIR=/path/to/photos
# --------------------------------------------------
run: $(TARGET)
	./$(TARGET) "$(DIR)"

# --------------------------------------------------
# Increment stored date
# make incr
# --------------------------------------------------
incr: $(TARGET)
	./$(TARGET) --incr

# --------------------------------------------------
# Decrement stored date
# make decr
# --------------------------------------------------
decr: $(TARGET)
	./$(TARGET) --decr

# --------------------------------------------------
# Increment date AND update photos
# make runall DIR=/path/to/photos
# --------------------------------------------------
runall: $(TARGET)
	./$(TARGET) --incr "$(DIR)"

# --------------------------------------------------
# Decrement date AND update photos
# make deprocess DIR=/path/to/photos
# --------------------------------------------------
deprocess: $(TARGET)
	./$(TARGET) --decr "$(DIR)"

# --------------------------------------------------
# Clean
# make clean
# --------------------------------------------------
clean:
	rm -f $(TARGET)
