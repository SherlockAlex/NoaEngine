#include <iostream>
#include <conio.h>
#include <windows.h>
#define high 20
#define length 500
using namespace std;
int row = 19, line = 2, a[high][length] = { 0 };
void get_new(void);//开始新游戏 
void SetTitle(LPCWSTR lpTitle) {
	SetConsoleTitle(lpTitle);
}
void all_in_2(void) {
	for (int i = 0; i < high; i++) 
		for (int j = 0; j < length; j++) 
			a[i][j] = 2;
}
void DrawInMillSecond(int millsecond_of_words, string pll) {//逐字输出 
	for (int i = 0; i <= pll.size() - 1; i++) Sleep(millsecond_of_words), cout << pll[i];
	cout << endl;
}
void HideCursor(void) {
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void color(string pll) {
	if (pll == "g") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	if (pll == "b") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	if (pll == "o") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	if (pll == "r") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	if (pll == "c") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	if (pll == "p") SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE);
}
void clear(int x, int y) {
	system("cls");
	/*
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);*/
}
void rand_map(void) {
	all_in_2();
	srand(time(NULL));
	for (int i = 16; i <= 19; i++)
		for (int j = 2; j <= 499; j++) {
			int s = rand() % 8;
			if (s == 0) a[i][j] = 1;//stop
		}
	for (int i = 1; i <= 20; i++) for (int j = 1; j <= 500; j++) if (i == 1 || j == 1 || i == 20 || j == 500) a[i][j] = 3;//stop
	a[row][line] = 4, a[19][499] = 5;
}
void draw(void) {
	a[row][line] = 4;
	for (int i = row - 9; i <= row + 9; i++) {
		for (int j = line - 16; j <= line + 16; j++) {
			if (i > 0 && j > 0 && i < 21 && j < 501) {
				if (a[i][j] == 1 || a[i][j] == 3) color("r"), cout << "*", color("o");
				if (a[i][j] == 2) cout << " ";
				if (a[i][j] == 4) color("g"), cout << "*", color("o");
				if (a[i][j] == 5) color("b"), cout << "*", color("o");
			}
		}
		cout << endl;
	}
}
void drop(void) {
	while (a[row + 1][line] == 2 || a[row + 1][line] == 0) a[row + 1][line] = 4, a[row][line] = 2, row++;
}
void got(void) {
	if (_kbhit()) {
		char g = _getch();
		if (g == 'd') {
			if (a[row][line + 1] != 1 && a[row][line + 1] != 3) {
				line++;
				a[row][line - 1] = 2;
				a[row][line] = 4;
				drop();
				clear(0, 0);
				draw();
			}
		}
		if (g == 'a') {
			if (a[row][line - 1] != 1 && a[row][line - 1] != 3) {
				line--;
				a[row][line + 1] = 2;
				a[row][line] = 4;
				drop();
				clear(0, 0);
				draw();
			}
		}
		if (g == 'w') {
			clock_t start, finish;
			if (a[row - 2][line] != 1 && a[row - 2][line] != 3) {
				row -= 2;
				a[row + 2][line] = 2;
				a[row][line] = 4;
				clear(0, 0);
				draw();
				start = clock();
				finish = clock();
				while (finish - start <= 200) {
					got();
					finish = clock();
				}
				drop();
				clear(0, 0);
				draw();
			}
		}
		if (g == 'q') {
			clock_t start, finish;
			if (a[row - 1][line] != 1 && a[row - 1][line] != 3) {
				row -= 1;
				a[row + 1][line] = 2;
				a[row][line] = 4;
				clear(0, 0);
				draw();
				start = clock();
				finish = clock();
				while (finish - start <= 200) {
					got();
					finish = clock();
				}
				drop();
				clear(0, 0);
				draw();
			}
		}
		if (g == 'e') {
			clock_t start, finish;
			if (a[row - 3][line] != 1 && a[row - 3][line] != 3) {
				row -= 3;
				a[row + 3][line] = 2;
				a[row][line] = 4;
				clear(0, 0);
				draw();
				start = clock();
				finish = clock();
				while (finish - start <= 200) {
					got();
					finish = clock();
				}
				drop();
				clear(0, 0);
				draw();
			}
		}
		if (g == 'f') {
			if (a[row][line + 2] == 2) {
				a[row][line + 2] = 4;
				a[row][line] = 2;
				line += 2;
				drop();
				clear(0, 0);
				draw();
			}
		}
		if (g == 'p') {
			row = 19, line = 2;
			memset(a, 0, sizeof(a));
			get_new();
		}
	}
}
void play(void) {
	while (true)
		if (a[19][499] != 5) goto win;
		else got();
win:
	clear(0, 0);
	color("r");
	DrawInMillSecond(150, "win!");
	color("o");
}
void get_new(void) {
	clear(0, 0);
	HideCursor();
	rand_map();
	draw();
	play();
}
void menu(void) {
	DrawInMillSecond(100, "欢迎来到马里奥!");
	Sleep(250);
	clear(0, 0);
	cout << "跳过教程?(y/n)" << endl;
	char skip = _getch();
	if (skip == 'y' || skip == 'Y')
		goto skips;
	clear(0, 0);
	DrawInMillSecond(100, "规则:w跳，q小跳，e大跳，a左，d右，p重来，f穿墙。绿色的是你，红色为障碍，蓝色为终点。");
	Sleep(250);
	clear(0, 0);
	DrawInMillSecond(100, "开始吧！");
	Sleep(250);
	system("pause");
	clear(0, 0);
skips:HideCursor();
}
void change_in_english_wordline(void) {
	keybd_event(16, 0, 0, 0);
	keybd_event(16, 0, 0x2, 0);
}

int main1() {
	//SetTitle((wchar_t *)("马里奥"));
	change_in_english_wordline();
	menu();
	get_new();
}
