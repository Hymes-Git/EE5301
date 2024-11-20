CC=g++
SRC_DIR=src
BUID_DIR=build
TARGET=placer

SOURCES=$(shell find $(SRC_DIR) -type f -name *.cpp)
OBJECTS=$(patsubst $(SRC_DIR)/%,$(BUID_DIR)/%,$(SOURCES:.cpp=.o))

#CFLAGS=-Wall -std=c++11 -O3
CFLAGS=-Wall -std=c++11 -g

INC=-I include

$(TARGET): $(OBJECTS)
	@echo "------------------------"
	@echo "Linking"
	@echo "------------------------"
	
	$(CC) $^ -o $(TARGET)

	@echo "Done."
	@echo "Executable = $(TARGET)"

$(BUID_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "------------------------"
	@echo "Building"
	@echo "------------------------"

	@mkdir -p $(BUID_DIR)
	
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning"
	$(RM) -rf $(BUID_DIR) $(TARGET_DIR)

.PHONY: clean

