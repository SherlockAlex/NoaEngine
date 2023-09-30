#include <ctime>
#include "NoaEngine.h"

using namespace std;

namespace noa {

	float deltaTime = 0;
    int pixelWidth = 0;
    int pixelHeight = 0;

	void Debug(string msg)
	{
		//������ʾʱ��Ĳ��ֵĴ�����ܱ��������쳣��ע���ڱ����Ԥ��������� _CRT_SECURE_NO_WARNINGS
		std::time_t current_time = std::time(nullptr);

		// ����ǰʱ��ת��Ϊ�ַ�����ʽ
		char time_string[100];
		std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
		cout << "[INFO " << time_string << "]:" << msg << endl;
	}

}

