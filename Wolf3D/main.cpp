#include "GameEngine.h"
#include "Level00.h"
#include "Level01.h"

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



class Wolf3D :public NoaGameEngine
{
public:
	Wolf3D(int width, int height, NoaGameEngine::GameWindowMode windowMode, string gameName) :
		NoaGameEngine(width, height, windowMode, gameName)
	{
		//���ص�һ������
		sceneManager.LoadScene("NewGame");

	}

	void Start() override
	{
		
	}

	
	void Update() override 
	{
		if (inputSystem.GetKeyHold(KeyESC))
		{
			Quit();
		}
	}

private:

	Level00 * level00 = Level00::Create();
	Level01 * level01 = Level01::Create();

};

int main(int argc,char * argv[])
{
	Wolf3D wolf3D(1920 / 2, 1080 / 2, NoaGameEngine::WindowMode, "3D");
	wolf3D.Run();
	return 0;
}