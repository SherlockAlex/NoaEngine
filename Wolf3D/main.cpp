#define NOA_GL

#include "GameEngine.h"
#include "Level00.h"
#include "Level01.h"
#include "MainMenuLevel.h"

using namespace noa;

/// <summary>
/// 游戏的场景管理
/// 一个便捷的开发方式是将场景作为一个类，然后游戏一开始就实例化出所有的场景
/// 这样的方式问题在于内存资源的浪费，因为玩家只能在当前的场景下进行活动，但是系统加载了游戏的所有资源
/// 
/// 这里采用了动态加载场景的方式，只要将场景保存为两属性字符串，分别表示tileMap在本地的路径
/// 然后Scene提供一个方法为GetTileMap()，返回所制造的地图便可，接着配合Game类进行一个场景的加载
/// 
/// 同时对场景的内存进行释放
/// 
/// </summary>



class Wolf3D :public NoaEngine
{
public:
	Wolf3D(int width, int height, WindowMode windowMode, std::string gameName) :
		NoaEngine(width, height, windowMode, gameName)
	{
		//加载第一个场景
		sceneManager.LoadScene("MainMenu");

	}

	void Start() override
	{

	}

	
	void Update() override 
	{
		if (inputSystem.GetKeyHold(KeyCode::KEY_ESC))
		{
			Quit();
		}

		//Debug::Log(ToString<float>(1.0f/Time::deltaTime));

	}

private:

	MainMenuLevel* mainMenu = MainMenuLevel::Create();
	Level00 * level00 = Level00::Create();
	Level01 * level01 = Level01::Create();

};

int main(int argc,char * argv[])
{
	Wolf3D wolf3D(1920/2, 1080/2, WindowMode::WINDOW, "Wolf-3D");
	return wolf3D.Run();
}