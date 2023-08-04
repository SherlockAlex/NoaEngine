#include "graphics.h"
#include <vector>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef unsigned int Uint32;

//ͼƬ������

vector<Uint32> image;

void save(vector<Uint32> data, const char* filename) {
	ofstream outfile(filename, ios::binary);
	if (!outfile) {
		cout << "�޷����ļ���" << endl;
		return;
	}

	// ��data�е�����д���ļ�
	Uint32 size = data.size();
	outfile.write(reinterpret_cast<char*>(&size), sizeof(size));
	outfile.write(reinterpret_cast<char*>(data.data()), sizeof(Uint32) * size);

	outfile.close();
}

vector<Uint32> load(const char* filename) {
	ifstream infile(filename, ios::binary);
	if (!infile) {
		cout << "�޷����ļ���" << endl;
		return {};
	}

	// ��ȡdata�Ĵ�С
	Uint32 size = 0;
	infile.read(reinterpret_cast<char*>(&size), sizeof(size));

	// ��ȡvector����
	vector<Uint32> data(size);
	infile.read(reinterpret_cast<char*>(data.data()), sizeof(Uint32) * size);

	infile.close();

	return data;
}

int main(int argc, char* argv[]) {
	if (argc < 3)
	{
		cout << "������Դ�ļ�������ļ�" << endl;
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
	loadimage(&img1, inputFile);//��ȡͼƬ
	imagHight = img1.getheight();
	imagWidth = img1.getwidth();          //��ȡͼƬ�߶ȺͿ��
	initgraph(imagWidth, imagHight, SHOWCONSOLE);//���û�ͼ���ں�ͼƬһ���� ������ʾ����̨����
	putimage(0, 0, &img1);//��ʾͼƬ

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

	cout << "ͼƬд�����:" << string((const char*)inputFile) << endl;

	return 0;
}
