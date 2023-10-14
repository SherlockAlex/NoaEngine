#define NOA_GL

#include "GameEngine.h"
#include "Level00.h"
#include "Level01.h"
#include "MainMenuLevel.h"

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
		//���ص�һ������
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