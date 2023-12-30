
# Compiler options
CC = g++ -DDEBUG -g
CFLAGS = -c -Wall
LINKER_LIBS = -lm -lpthread -lncurses

# Directories
SRC_DIR = ./src
BUILD_DIR = ./build/debug
$(shell mkdir -p ./build/debug ./build/release)

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
INC_DIRS := $(addprefix -I,./src/include)

OBJ_FILES := $(addprefix $(BUILD_DIR)/,$(notdir $(SRC_FILES:.cpp=.o)))

# Targets
all: $(OBJ_FILES)
	$(CC) -o simulator $^ $(LINKER_LIBS)

$(OBJ_FILES): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INC_DIRS) $< -o $@

clean:
	-rm -rf ./build/
	-rm simulator

release: CC = g++
release: all

debugvars:
	@echo 'INC_DIRS: ' $(INC_DIRS)
	@echo 'SRC_FILES: ' $(SRC_FILES)
	@echo 'OBJ_FILES: ' $(OBJ_FILES)

.PHONY: clean debugvars