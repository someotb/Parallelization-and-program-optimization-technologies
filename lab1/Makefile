UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
    CXX = g++-16
else
    CXX = g++
endif

CXXFLAGS = -Wall -fopenmp
SRC = src/main.cpp
BUILD = build
TARGET = $(BUILD)/main

all: $(TARGET)

$(TARGET): $(SRC) | $(BUILD)
	@$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD):
	@mkdir -p $(BUILD)

run: $(TARGET)
	@./${TARGET}

clean:
	@rm -rf $(BUILD)

.PHONY: all run clean