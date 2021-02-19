
CC=g++

# CFLAGS=-std=c99 -Wall -Wextra -pedantic -g -I .
CFLAGS=-std=c++11 -Wall -Wextra -pedantic -g -O2

SOURCE_DIR=src
BUILD_DIR=build
RUN_FILE_NAME=run.exe
SRC_FILES:=$(wildcard $(SOURCE_DIR)/*.cpp)
OBJ_FILES:=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

default: build

build_dir:
	@mkdir -p $(BUILD_DIR) || true;

build: build_dir $(BUILD_DIR)/$(RUN_FILE_NAME)

$(BUILD_DIR)/$(RUN_FILE_NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -lsimlib -lm 

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(SOURCE_DIR)/%.hpp $(SOURCE_DIR)/global.hpp
	$(CC) $(CFLAGS) -lsimlib -lm -c -o $@ $<

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(SOURCE_DIR)/global.hpp
	$(CC) $(CFLAGS) -lsimlib -lm -c -o $@ $<

run:
	make r ARGS="-s a"
r:
	@./$(BUILD_DIR)/$(RUN_FILE_NAME) ${ARGS} 

vrun:
	@valgrind ./$(BUILD_DIR)/$(RUN_FILE_NAME) ${ARGS}

clean: 
	rm -rf $(BUILD_DIR) || true

