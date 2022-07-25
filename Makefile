BINS = main.exe game.exe
SDL_INCLUDE_DIR = E:/3rdpart/SDL2-2.0.22-VC/include
SDL_LIB_DIR = E:/3rdpart/SDL2-2.0.22-VC/lib/x64
SDL_LIBRARIES = -lSDL2 -lSDL2main
CXX_FLAGS = -Xlinker /subsystem:windows -lshell32

CXX := clang++
CC := clang

all:${BINS}

%.exe:%.cpp
	$(CXX) $^ -I${SDL_INCLUDE_DIR} -L${SDL_LIB_DIR} ${SDL_LIBRARIES} ${CXX_FLAGS} -o $@ -std=c++17