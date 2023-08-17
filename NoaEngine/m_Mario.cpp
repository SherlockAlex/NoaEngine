#include "NoaEngine.h"

class Mario :public NoaGameEngine
{
private:


public:
	Mario(int width, int height,
		GameWindowMode windowMode,
		string gameName) :NoaGameEngine(width, height,
			windowMode,
			gameName)
	{
		
	}

public:

	void Draw() 
	{
		for (int x = 0;x<pixelWidth;x++) 
		{
			for (int y = 0;y<pixelHeight;y++) 
			{

				Uint32 color = BLUE;

				//¿ªÊ¼¼ÆËã

				renderer.DrawPixel(x, y, color);

			}
		}
	}

	void Start() override
	{
		
	}

	void Update() override
	{
		Draw();
		cout << "FPS:" << 1 / deltaTime << endl;
	}

};

//int main(int argc,char * argv[]) 
//{
//	Mario mario(1920/2,1080/2,NoaGameEngine::WindowMode,"Mario");
//	mario.Run();
//	return 0;
//}
