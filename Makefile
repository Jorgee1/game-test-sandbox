CXX = clang++
CXXFLAGS = -w -no-pie
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image

BUILD_FOLDER = build/ubuntu

SOURCE_PATH  = src
INCLUDE_PATH = include
TEST_PATH    = test
ASSET_PATH   = asset

SDL_PATH     = /usr/include/SDL2

PROGRAM_NAME = 2D-top-down.app
TEST_NAME    = test.app

ENTRY_FILE   = main.cpp

OBJECTS  := $(filter-out $(SOURCE_PATH)/$(ENTRY_FILE), $(wildcard $(SOURCE_PATH)/*.cpp))

MAIN_OBJ = $(SOURCE_PATH)/$(ENTRY_FILE)
TEST_OBJ = $(TEST_PATH)/$(ENTRY_FILE)

### Program ##################
compile: required
	$(CXX) $(MAIN_OBJ) $(OBJECTS) -o $(BUILD_FOLDER)/$(PROGRAM_NAME) -I $(INCLUDE_PATH) $(CXXFLAGS) $(LIBS) -I$(SDL_PATH)

required:
	mkdir -p $(BUILD_FOLDER)
	cp -r $(ASSET_PATH) $(BUILD_FOLDER)


### Test Section ############

build_test: compile_test run_test

compile_test:
	$(CXX) $(TEST_OBJ) $(OBJECTS) -o $(TEST_NAME) -I $(INCLUDE_PATH) $(CXXFLAGS) $(LIBS)

run_test:
	./$(TEST_NAME)

clean_test:
	rm $(TEST_NAME)
