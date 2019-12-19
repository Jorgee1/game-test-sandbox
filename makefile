CXX = g++
CXXFLAGS = -w -no-pie
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image

BUILD_FOLDER = build/ubuntu

SOURCE_PATH = src
INCLUDE_PATH = include
TEST_PATH = test

FONT_PATH = fonts
MAP_PATH  = map

PROGRAM_NAME = $(BUILD_FOLDER)/2D-top-down

OBJECTS = $(SOURCE_PATH)/main.cpp $(SOURCE_PATH)/menu.cpp $(SOURCE_PATH)/texture.cpp $(SOURCE_PATH)/window.cpp $(SOURCE_PATH)/overworld.cpp


compile: required 
	$(CXX) $(OBJECTS) -o $(PROGRAM_NAME) -I $(INCLUDE_PATH) $(CXXFLAGS) $(LIBS)

run: compile
	./$(PROGRAM_NAME)

required:
	mkdir -p $(BUILD_FOLDER)
	cp -r $(FONT_PATH) $(BUILD_FOLDER)
	cp -r $(MAP_PATH) $(BUILD_FOLDER)
