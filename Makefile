BINS = main.exe game.exe
SDL_INCLUDE_DIR = D:/Program/3rdlibs/SDL2-2.0.22-VC/include
SDL_LIB_DIR = D:/Program/3rdlibs/SDL2-2.0.22-VC/lib/x64
SDL_LIBRARIES = -lSDL2 -lSDL2main
SDL_IMAGE_INCLUDE_DIR = D:/Program/3rdlibs/SDL2_image-2.6.0-VC/include
SDL_IMAGE_LIB_DIR = D:/Program/3rdlibs/SDL2_image-2.6.0-VC/lib/x64
SDL_IMAGE_LIBRARIES = -lSDL2_image
CXX_FLAGS = -Xlinker /subsystem:windows -lshell32

CXX := clang++
CC := clang

all:${BINS}

%.exe:%.cpp
	$(CXX) $^ -I${SDL_INCLUDE_DIR} -I${SDL_IMAGE_INCLUDE_DIR} -L${SDL_LIB_DIR} -L${SDL_IMAGE_LIB_DIR} ${SDL_IMAGE_LIBRARIES} ${SDL_LIBRARIES} ${CXX_FLAGS} -o $@ -std=c++17
