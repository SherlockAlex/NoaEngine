objs=./*.cpp ./*.h

CXX=g++

#linux
INCDIRS+=/usr/include/SDL2
LIBDIRS+=/usr/lib

CFLAGS=-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -static-libstdc++ -lpthread

BUILD_DIR=./game-build

GAME_FILE=/game

install:$(objs)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(objs) -Bstatic -I$(INCDIRS) -L$(LIBDIRS) $(CFLAGS) -o $(BUILD_DIR)$(GAME_FILE)

env-linux:
	sudo apt-get install libsdl2-dev libsdl2-net-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

clean:
	rm -rf $(BUILD_DIR)$(GAME_FILE)

run:
	$(BUILD_DIR)$(GAME_FILE)
