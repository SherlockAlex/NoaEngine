#define OLC_PGE_APPLICATION
#include "D:\olcPixelGameEngine\olcPixelGameEngine.h"

using namespace olc;

class ExampleGame:public PixelGameEngine {
public:
	bool OnUserCreate() override
	{
		return true;
	}
	// Called every frame, and provides you with a time per frame value
	bool OnUserUpdate(float fElapsedTime) override
	{
		for (int x = 0;x<ScreenWidth();x++) 
		{
			for (int y = 0;y<ScreenHeight();y++) 
			{
				Draw(x, y, RED);
			}
		}
		return true;
	}
};

int main(int argc,char * argv[])
{
	ExampleGame game;
	game.Construct(1920 / 4, 1080 / 4,1920/4,1080/4);
	game.Start();
	return 0;
}