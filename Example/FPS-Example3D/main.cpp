#define NOA_GL

#include "GameEngine.h"
#include "Level00.h"
#include "Level01.h"
#include "MainMenuLevel.h"
#include "WolfResource.h"

using namespace noa;

/// <summary>
/// ��Ϸ�ĳ�������
/// һ����ݵĿ�����ʽ�ǽ�������Ϊһ���࣬Ȼ����Ϸһ��ʼ��ʵ���������еĳ���
/// �����ķ�ʽ���������ڴ���Դ���˷ѣ���Ϊ���ֻ���ڵ�ǰ�ĳ����½��л������ϵͳ��������Ϸ��������Դ
/// 
/// ��������˶�̬���س����ķ�ʽ��ֻҪ����������Ϊ�������ַ������ֱ��ʾtileMap�ڱ��ص�·��
/// Ȼ��Scene�ṩһ������ΪGetTileMap()������������ĵ�ͼ��ɣ��������Game�����һ�������ļ���
/// 
/// ͬʱ�Գ������ڴ�����ͷ�
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

		//���ص�һ������
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