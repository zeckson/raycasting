SRC_DIR = src
BUILD_DIR = build
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
HEADER_FILES = $(wildcard $(SRC_DIR)/*.h) $(wildcard $(SRC_DIR)/**/*.h)
OBJ_NAME = Raycasting
SDL_LIBRARY_INCLUDE = $(shell pkg-config SDL2 --cflags)
SDL_CORE_LIBRARY = $(shell pkg-config SDL2 --libs)
SDL_IMAGE_LIBRARY = $(shell pkg-config SDL2_image --libs)
SDL_TTF_LIBRARY = $(shell pkg-config SDL2_ttf --libs)
SDL_LIBRARY = $(SDL_CORE_LIBRARY) $(SDL_IMAGE_LIBRARY) $(SDL_TTF_LIBRARY)
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g -mmacosx-version-min=13.0

.PHONY: compile clean run all

compile: clean
	mkdir $(BUILD_DIR)
	echo $(SDL_LIBRARY)
	$(CC) $(COMPILER_FLAGS) $(SDL_LIBRARY) $(SDL_LIBRARY_INCLUDE) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

clean:
	rm -rf $(BUILD_DIR)

run: compile
	$(BUILD_DIR)/${OBJ_NAME}

all: compile
