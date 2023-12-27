
# Compiler options
CC = gcc
CFLAGS = -c -Wall
LINKER_LIBS = -lm -lpthread

# Directories
SRC_DIR = ./src
BUILD_DIR = ./build

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
INC_DIRS := $(addprefix -I,./src/include)

OBJ_FILES := $(addprefix $(BUILD_DIR)/,$(notdir $(SRC_FILES:.c=.o)))

# Targets
all: $(OBJ_FILES)
	$(CC) -o simulator $^ $(LINKER_LIBS)

$(OBJ_FILES): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) $< -o $@

clean:
	-rm -rf $(BUILD_DIR)/*.o
	-rm simulator

debug:
	@echo 'INC_DIRS: ' $(INC_DIRS)
	@echo 'SRC_FILES: ' $(SRC_FILES)
	@echo 'OBJ_FILES: ' $(OBJ_FILES)

.PHONY: clean debug