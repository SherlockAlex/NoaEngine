#include "NoaEngine.h"

int main(int argc,char * argv[])
{
	game.Run();
	game.~NoaGameEngine();
	return 0;
}