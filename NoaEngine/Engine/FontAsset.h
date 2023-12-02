#ifndef NOAENGINE_FONTASSET_H
#define NOAENGINE_FONTASSET_H

#include <unordered_map>

#include "Core.h"

namespace noa {

	struct NOA_API Font;

	class NOA_API FontAsset
	{
	public:
		FontAsset(const char* ttfPath, int size);
		Font* GetFont(wchar_t c);
		Font* operator[](wchar_t c);
	private:
		std::unordered_map<wchar_t, Font*> fonts;
	public:
		int size = 0;
	};
}

#endif // !NOAENGINE_FONTASSET_H

