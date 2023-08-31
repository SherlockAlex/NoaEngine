#include "NoaGUI.h"
#include <unordered_map>
#include "Scene.h"

namespace noa {
	extern void Debug(string msg);

	FontAsset::FontAsset(const char * fontPath)
	{
		//加载游戏的字体资源

		unordered_map<int, Font*> result;
		std::vector<PixelData> resultData;

		std::ifstream inputFile(fontPath, std::ios::binary);
		if (inputFile)
		{
			int tileCount;
			inputFile.read(reinterpret_cast<char*>(&tileCount), sizeof(int));

			Debug("font asset size:" + to_string(tileCount));

			for (int i = 0; i < tileCount; i++)
			{
				PixelData pixelData;
				inputFile.read(reinterpret_cast<char*>(&pixelData.id), sizeof(int));

				int imageCount;
				inputFile.read(reinterpret_cast<char*>(&imageCount), sizeof(int));

				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.x), sizeof(int));
				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.y), sizeof(int));
				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.width), sizeof(int));
				inputFile.read(reinterpret_cast<char*>(&pixelData.sprites.height), sizeof(int));

				for (int j = 0; j < imageCount; j++)
				{
					int pixelValue;
					inputFile.read(reinterpret_cast<char*>(&pixelValue), sizeof(int));
					pixelData.sprites.images.push_back(pixelValue);
				}

				resultData.push_back(pixelData);
			}

			inputFile.close();
		}
		else {
			Debug("can't find the font assets file");
			exit(0);
		}

		for (PixelData data : resultData)
		{
			Font* font = new Font(data.sprites);
			result[data.id] = font;
		}

		

		for (int i = 0; i < 9*11-4; i++)
		{
			char c = (char)(i + 32);
			this->fonts[c] = result[i];
			
		}
		Debug("Init font asset successfully");
		//this->fonts = result;
		//return result;
	}

	Font* FontAsset::GetFont(char c)
	{
		if (!ContainKey<char,Font*>(this->fonts,c)) 
		{
			return nullptr;
		}
		return fonts[c];
	}

}

