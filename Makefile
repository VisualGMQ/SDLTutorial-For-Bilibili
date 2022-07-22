BINS:main.exe game.exe

all:${BINS}

main.exe: main.cpp
	g++ $^ -ID:/Program/3rdlibs/SDL2-2.0.22-VC/include -LD:/Program/3rdlibs/SDL2-2.0.22-VC/lib/x64 -lSDL2main -lSDL2 -o $@ -std=c++17 -mwindows

game.exe: game.cpp
	g++ $^ -ID:/Program/3rdlibs/SDL2-2.0.22-VC/include -LD:/Program/3rdlibs/SDL2-2.0.22-VC/lib/x64 -lSDL2main -lSDL2 -o $@ -std=c++17 -mwindows

.PHONY:clean
clean:
	rm ${BINS}
run:
	./main.exe
