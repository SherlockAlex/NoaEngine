#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <chrono>

using namespace std;

int main() {
	auto tp1 = chrono::system_clock::now();
	chrono::duration<float> elapsedTime;
	auto tp2 = chrono::system_clock::now();

	while (1) {
		tp1 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime;
		tp2 = chrono::system_clock::now();
		float deltaTime = elapsedTime.count();

		for (int i = 0; i < 1920*1080;i++) {

		}
		cout << deltaTime << endl;

		tp1 = tp2;

	}
	return 0;
}