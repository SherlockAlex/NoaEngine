#include "NoaEngine.h"

class Snake :public NoaGameEngine
{
public:
	Snake(int width, int height,
		GameWindowMode windowMode,
		string gameName) :NoaGameEngine(width, height,
			windowMode,
			gameName)
	{
		//¥¥Ω®”Œœ∑
	}

public:

	void Start() override
	{
		
	}

	void Update() override
	{
		
	}

};

int main(int argc,char * argv[]) 
{
	Snake snake(1920/2,1080/2,NoaGameEngine::WindowMode,"Snake");
	snake.Run();
	return 0;
}
