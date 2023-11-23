#ifndef NOAENGINE_FONTASSET_H
#define NOAENGINE_FONTASSET_H

#include <unordered_map>


namespace noa {

	struct Font;

	class FontAsset 
	{
	private:
		std::unordered_map<wchar_t, Font*> fonts;
	public:
		int size = 0;
		FontAsset(const char* ttfPath, int size);
		Font* GetFont(wchar_t c);
		Font* operator[](wchar_t c);
	};
}

#endif // !NOAENGINE_FONTASSET_H

