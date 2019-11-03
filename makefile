CXX = cl

LINK = linker.exe
BASE_DIR = $(pwd)
GLOBAL_LIBRARY_PATH = D:\Programacion\Librerias

SDL_PATH = $(GLOBAL_LIBRARY_PATH)\SDL2-2.0.10
SDL_TTF_PATH = $(GLOBAL_LIBRARY_PATH)\SDL2_ttf-2.0.15
SDL_IMAGE_PATH = $(GLOBAL_LIBRARY_PATH)\SDL2_image-2.0.5

WINDOW_CLASS_PATH  = .\window
TEXTURE_CLASS_PATH = .\texture

INCLUDE_PATHS = /I$(TEXTURE_CLASS_PATH) /I$(WINDOW_CLASS_PATH) /I$(SDL_PATH)\include /I$(SDL_TTF_PATH)\include /I$(SDL_IMAGE_PATH)\include
LIBRARY_PATHS = /LIBPATH:$(SDL_PATH)\lib\x64 /LIBPATH:$(SDL_TTF_PATH)\lib\x64 /LIBPATH:$(SDL_IMAGE_PATH)\lib\x64

SDL_LIB = SDL2main.lib SDL2.lib SDL2_ttf.lib SDL2_image.lib

LINKER_FLAGS = $(SDL_LIB) /link /SUBSYSTEM:CONSOLE $(LIBRARY_PATHS)

BUILD_FOLDER = build

PROGRAM_NAME = $(BUILD_FOLDER)\2D-top-down.exe
OBJECTS = main.cpp $(TEXTURE_CLASS_PATH)\texture.cpp $(WINDOW_CLASS_PATH)\window.cpp

main:
	$(CXX) /EHsc $(OBJECTS) /Fe$(PROGRAM_NAME) $(INCLUDE_PATHS) $(LINKER_FLAGS) 
