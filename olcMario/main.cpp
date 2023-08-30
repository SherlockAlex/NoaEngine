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
				Draw(x, y, BLUE);
			}
		}
		return true;
	}
};

int main(int argc,char * argv[])
{
	ExampleGame game;
	game.Construct(1920,1080,1920,1080,true,false,true);
	game.Start();
	return 0;
}