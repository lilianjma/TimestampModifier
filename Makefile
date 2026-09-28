CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

TARGET := modify_timestamps
SRC := modify_timestamps.cpp

DIR ?=

.PHONY: all compile run incr clean

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
# Update photos using the current date
# make run DIR=/path/to/photos
# --------------------------------------------------
run: $(TARGET)
	./$(TARGET) "$(DIR)"

# --------------------------------------------------
# Increment the stored date
# make incr
# --------------------------------------------------
incr: $(TARGET)
	./$(TARGET) --incr

# --------------------------------------------------
# Increment date AND update photos
# make process DIR=/path/to/photos
# --------------------------------------------------
process: $(TARGET)
	./$(TARGET) --incr "$(DIR)"

# --------------------------------------------------
# Clean
# make clean
# --------------------------------------------------
clean:
	rm -f $(TARGET)
