#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::string ReadSourceFromFile(const std::string & filePath)
{
	std::ifstream file(filePath);
	if (file.is_open()) 
	{
		std::string line(
			std::istreambuf_iterator<char>(file)
			, (std::istreambuf_iterator<char>())
		);

		return line;
	}

	return std::string();

}

int main() {
	std::cout << ReadSourceFromFile("./vertexShader.glsl") << std::endl;
	return 0;
}