#define NOA_GL

#include "GameEngine.h"
#include "Level00.h"
#include "Level01.h"
#include "MainMenuLevel.h"
#include "WolfResource.h"

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
		wolfResource = std::make_shared<WolfResource>();

		this->mainMenu = sceneManager.CreateScene("MainMenu");
		this->mainMenu->onLoad += MainMenuDelegate::OnLoad;

		this->level00 = sceneManager.CreateScene("NewGame");
		this->level00->onLoad += Level00Delegate::OnLoad;

		this->level01 = sceneManager.CreateScene("SecondFloor");
		this->level01->onLoad += Level01Delegate::OnLoad;

		//加载第一个场景
		sceneManager.LoadScene("MainMenu");

	}
	
	void Update() override 
	{
		if (inputSystem.GetKeyHold(KeyCode::KEY_ESC))
		{
			Quit();
		}

	}

private:

	Scene* mainMenu = nullptr;
	Scene* level00 = nullptr;
	Scene* level01 = nullptr;

};

int main(int argc,char * argv[])
{
	Wolf3D wolf3D(1920/2, 1080/2, WindowMode::WINDOW, "Wolf-3D");
	return wolf3D.Run();
}