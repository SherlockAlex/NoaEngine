objs=./obj/Animator.o \
	 ./obj/Audio.o \
	 ./obj/Behaviour.o \
	 ./obj/GameObject.o \
	 ./obj/InputSystem.o \
	 ./obj/NoaEngine.o \
	 ./obj/NoaMath.o \
	 ./obj/Renderer.o \
	 ./obj/Scene.o \
	 ./obj/Sprite.o \
	 ./obj/NoaAction.o \
	 ./obj/StateMachine.o \
	 ./obj/Physics.o \
	 ./obj/Camera.o \
	 ./obj/NoaGUI.o \
	 ./obj/glad.o \
	 ./obj/z_Zelda.o \
#其中d_doom.o和d_RayCastHit.o是游戏引擎的项目示例，你可以替换为你的项目编写编译的目标文件名(.cpp)同名

CXX=g++

#linux
INCDIRS+=/usr/include/SDL2
LIBDIRS+=/usr/lib

#INCDIRS+=D:\SDL2\mingw(CLion+VSC)\SDL2-2.26.0-allinone\x86_64-w64-mingw32\include
#LIBDIRS+=D:\SDL2\mingw(CLion+VSC)\SDL2-2.26.0-allinone\x86_64-w64-mingw32\lib

CFLAGS=-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -static-libstdc++ -lpthread -lX11

BUILD_DIR=./game-build

GAME_FILE=/game

./obj/%.o:./core/%.cpp
	mkdir -p obj
	$(CXX) -c -Bstatic -I$(INCDIRS) -L$(LIBDIRS) $(CFLAGS) $^ -o $@

./obj/%.o:%.cpp
	mkdir -p obj
	$(CXX) -c -Bstatic -I$(INCDIRS) -L$(LIBDIRS) $(CFLAGS) $^ -o $@

./obj/%.o:./core/src/%.c
	mkdir -p obj
	$(CXX) -c -Bstatic -I$(INCDIRS) -L$(LIBDIRS) $(CFLAGS) $^ -o $@

install:$(objs)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(objs) -Bstatic -I$(INCDIRS) -L$(LIBDIRS) $(CFLAGS) -o $(BUILD_DIR)$(GAME_FILE)

env-linux:
	sudo apt-get install libsdl2-dev libsdl2-net-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

clean:
	rm -rf ./obj/*.o
	rm -rf *.o
	rm -rf $(BUILD_DIR)$(GAME_FILE)

run:
	$(BUILD_DIR)$(GAME_FILE)
