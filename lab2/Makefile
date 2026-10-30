CXX = g++
CXXFLAGS = -Wall -fopenmp
SRC = src/main.cpp
BUILD = build
TARGET = $(BUILD)/main

all: $(TARGET)

$(TARGET): $(SRC) | $(BUILD)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)

.PHONY: all clean