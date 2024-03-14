CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lncurses -lconfig
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
CONFIG_DIR = $(HOME)/.config/tuxplorer
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
TARGET = $(BIN_DIR)/tuxplorer
CONFIG_FILE = $(CONFIG_DIR)/config.cfg

# Dynamically generate the config content to account for different user home directories
define CONFIG_CONTENT
pins: {
	Desktop: "$(HOME)/Desktop",
	Home: "$(HOME)"
}
params: {
	TextEditor: "vim"
}
endef
export CONFIG_CONTENT

all: directories $(TARGET) config

directories:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)

config:
	@mkdir -p $(CONFIG_DIR)
	@echo "$$CONFIG_CONTENT" > $(CONFIG_FILE)
	@echo "Configuration file created at $(CONFIG_FILE)"

$(TARGET): $(OBJECTS)
	@echo "Linking: " $(OBJECTS) " into " $(TARGET)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling: " $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(CONFIG_DIR)

.PHONY: all clean directories config

