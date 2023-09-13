#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main1(int argc,char * argv)
{
	ofstream file("./text");
	if (file.is_open())
	{
		char m = 0;
		while (m<128)
		{
			file << m;
			cout << m << endl;
			m = m + 1;
		}

	}
	file.close();
	cout << "写入文件成功" << endl;
	return 0;
}
