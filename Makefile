CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lstdc++fs

# Directories
SRC_DIR = .
CONTEMPORARY_DIR = contemporary
CONVENTIONAL_DIR = conventional
BUILD_DIR = build
BIN_DIR = bin

# Source files
MAIN_SRC = $(SRC_DIR)/main.cpp
DATA_GENERATOR_SRC = $(SRC_DIR)/data_generator.cpp
CONTEMPORARY_SRC = $(wildcard $(CONTEMPORARY_DIR)/*.cpp)
CONVENTIONAL_SRC = $(wildcard $(CONVENTIONAL_DIR)/*.cpp)
ALL_SRC = $(MAIN_SRC) $(DATA_GENERATOR_SRC) $(CONTEMPORARY_SRC) $(CONVENTIONAL_SRC)

# Object files
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(ALL_SRC)))

# Executable
EXECUTABLE = $(BIN_DIR)/sorting_algorithms

# Default target
all: directories $(EXECUTABLE)

# Create necessary directories
directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

# Link object files to create executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile main source file
$(BUILD_DIR)/main.o: $(MAIN_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile data generator source file
$(BUILD_DIR)/data_generator.o: $(DATA_GENERATOR_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile contemporary algorithm source files
$(BUILD_DIR)/%.o: $(CONTEMPORARY_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Compile conventional algorithm source files
$(BUILD_DIR)/%.o: $(CONVENTIONAL_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the executable
run: all
	$(EXECUTABLE)

.PHONY: all directories clean run
