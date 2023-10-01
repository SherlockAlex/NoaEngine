#include "GameEngine.h"

using namespace std;
using namespace noa;

class Mario :public NoaEngine {
public:
    Mario(int w, int h, WINDOWMODE windowMode, string name) :
        NoaEngine(w,h,windowMode,name) 
    {

    }

    void Start() override {
        renderer->DrawString("Hello World",10,10,WHITE,50);
    }

    void Update() override {

    }

};

int main(int argc,char * argv[])
{
    Mario mario = Mario(1920 / 2, 1080 / 2, WINDOWMODE::WINDOW, "Mario");
    mario.Run();
}
