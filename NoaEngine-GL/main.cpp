#include "NoaEngine.h"
#include <iostream>

int main(int argc,char * argv)
{
	noa::NoaEngine game(1920/2,1080/2,noa::NoaEngine::FullScreen,"example game");
	game.Run();

	return 0;
}