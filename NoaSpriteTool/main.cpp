#include "graphics.h"
#include <vector>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef unsigned int Uint32;

//图片载入器

vector<Uint32> image;

void save(vector<Uint32> data, const char* filename) {
	ofstream outfile(filename, ios::binary);
	if (!outfile) {
		cout << "无法打开文件！" << endl;
		return;
	}

	// 将data中的数据写入文件
	Uint32 size = data.size();
	outfile.write(reinterpret_cast<char*>(&size), sizeof(size));
	outfile.write(reinterpret_cast<char*>(data.data()), sizeof(Uint32) * size);

	outfile.close();
}

vector<Uint32> load(const char* filename) {
	ifstream infile(filename, ios::binary);
	if (!infile) {
		cout << "无法打开文件！" << endl;
		return {};
	}

	// 读取data的大小
	Uint32 size = 0;
	infile.read(reinterpret_cast<char*>(&size), sizeof(size));

	// 读取vector内容
	vector<Uint32> data(size);
	infile.read(reinterpret_cast<char*>(data.data()), sizeof(Uint32) * size);

	infile.close();

	return data;
}

int main(int argc, char* argv[]) {
	if (argc < 3)
	{
		cout << "请输入源文件和输出文件" << endl;
		return 0;
	}
	IMAGE img1;
	int imagHight, imagWidth;
	const char* outputFile = argv[2];

	int len = strlen(argv[1]);

	wchar_t* inputFile = (wchar_t*)malloc(sizeof(wchar_t) * len);

	for (int i = 0; i < len; i++)
	{
		inputFile[i] = argv[1][i];
	}
	inputFile[len] = '\0';
	loadimage(&img1, inputFile);//获取图片
	imagHight = img1.getheight();
	imagWidth = img1.getwidth();          //获取图片高度和宽度
	initgraph(imagWidth, imagHight, SHOWCONSOLE);//设置绘图窗口和图片一样大 并且显示控制台窗口
	putimage(0, 0, &img1);//显示图片

	COLORREF color;
	//FILE* rgbFile = fopen("./5.txt", "a");
	for (int x = 0; x < imagWidth; x++)
	{
		for (int y = 0; y < imagHight; y++)
		{
			color = getpixel(x, y);
			image.push_back(color);
		}

	}

	save(image, outputFile);

	//fclose(rgbFile);

	cout << "图片写入完成:" << string((const char*)inputFile) << endl;

	return 0;
}
