#include <ctime>
#include "NoaEngine.h"

using namespace std;

namespace noa {

	float deltaTime = 0;
    int pixelWidth = 0;
    int pixelHeight = 0;

	void Debug(string msg)
	{
		//下面显示时间的部分的代码可能编译会出现异常，注意在编译的预处理请加上 _CRT_SECURE_NO_WARNINGS
		std::time_t current_time = std::time(nullptr);

		// 将当前时间转换为字符串格式
		char time_string[100];
		std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
		cout << "[INFO " << time_string << "]:" << msg << endl;
	}

}

