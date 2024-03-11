CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lncurses
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
TARGET = $(BIN_DIR)/tuxplorer

all: directories $(TARGET)

directories:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

$(TARGET): $(OBJECTS)
	@echo "Linking: " $(OBJECTS) " into " $(TARGET)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling: " $<
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean directories
