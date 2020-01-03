@echo off

set BUILD_FOLDER=build\windows
set GAME_NAME=2D-top-down.exe
set CPP_FILES=src\main.cpp src\action.cpp src\window.cpp src\texture.cpp src\dialog_box.cpp src\entity.cpp src\menu.cpp src\overworld.cpp

set PATH_SDL=%CPP_LIBRARIES%\SDL2-2.0.10
set PATH_TTF=%CPP_LIBRARIES%\SDL2_ttf-2.0.15
set PATH_IMG=%CPP_LIBRARIES%\SDL2_image-2.0.5

set LIBS=SDL2main.lib SDL2.lib SDL2_ttf.lib SDL2_image.lib
set LIB_PATH=/LIBPATH:%PATH_SDL%\lib\x64 /LIBPATH:%PATH_TTF%\lib\x64 /LIBPATH:%PATH_IMG%\lib\x64
set INCLUDES=/I. /I.\include /I%PATH_SDL%\include /I%PATH_TTF%\include /I%PATH_IMG%\include

set FLAGS=/EHsc

mkdir %BUILD_FOLDER%

cl %FLAGS% %CPP_FILES% /Fe%BUILD_FOLDER%\%GAME_NAME% %INCLUDES% %LIBS% /link %LIB_PATH% /SUBSYSTEM:WINDOWS




xcopy /s libraries\*.dll %BUILD_FOLDER%\ /Y

mkdir %BUILD_FOLDER%\asset
xcopy /s asset %BUILD_FOLDER%\asset /Y



cd %BUILD_FOLDER%
%game_name%
cd ..
cd ..

del *.obj
