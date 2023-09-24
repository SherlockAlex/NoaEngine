#include "NoaEngine.h"

using namespace noa;
using namespace std;

class Test :public NoaEngineGL {
public:
	Test(int width, int height, GameWindowMode windowMode, string title) :
		NoaEngineGL(width,height,windowMode,title)
	{

	}

	void Start() override 
	{

	}

	void Update() override {
		for (int x = 0;x<pixelWidth;x++) 
		{
			for (int y = 0;y<pixelHeight;y++) 
			{
				DRAWPIXEL(x,y,rand());
			}
		}

		Debug(to_string(1.0/deltaTime));

	}

};

int main() 
{
	Test test(1920 / 2, 1080 / 2, NoaEngineGL::WindowMode, "Test");
	test.Run();
}

