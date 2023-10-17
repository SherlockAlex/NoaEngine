#ifndef NOAENGINE_GRAPHIC_H
#define NOAENGINE_GRAPHIC_H

#include "NoaMath.h"

namespace noa {

	#undef LOBYTE
	#undef BGR
	#undef RGB
	#undef GetRValue
	#undef GetGValue
	#undef GetBValue
	#undef RGBA
	#undef GetAValue
	#undef MULTICOLOR

	inline unsigned char LOBYTE(unsigned int w) {
		return static_cast<unsigned char>(w & 0xff);
	}

	inline unsigned int BGR(unsigned int color) {
		return (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16);
	}

	inline unsigned int RGB(unsigned char r, unsigned char g, unsigned char b) {
		return static_cast<unsigned int>((static_cast<unsigned char>(r) | (static_cast<unsigned char>(g) << 8)) | (static_cast<unsigned int>(static_cast<unsigned char>(b)) << 16));
	}

	inline unsigned char GetRValue(unsigned int rgb) {
		return LOBYTE(rgb);
	}

	inline unsigned char GetGValue(unsigned int rgb) {
		return LOBYTE(static_cast<unsigned short>(rgb) >> 8);
	}

	inline unsigned char GetBValue(unsigned int rgb) {
		return LOBYTE(rgb >> 16);
	}

	inline unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		return static_cast<unsigned int>((static_cast<unsigned char>(r) | (static_cast<unsigned char>(g) << 8)) | ((static_cast<unsigned int>(static_cast<unsigned char>(b)) << 16) | (static_cast<unsigned int>(static_cast<unsigned char>(a)) << 24)));
	}

	inline unsigned char GetAValue(unsigned int argb) {
		return LOBYTE(argb >> 24);
	}

	inline unsigned int MULTICOLOR(unsigned int color, unsigned int mutiColor) {
		return RGBA(static_cast<unsigned char>(GetRValue(color) * (static_cast<float>(GetRValue(mutiColor)) / 255.0)),
					static_cast<unsigned char>(GetGValue(color) * (static_cast<float>(GetGValue(mutiColor)) / 255.0)),
					static_cast<unsigned char>(GetBValue(color) * (static_cast<float>(GetBValue(mutiColor)) / 255.0)),
					GetAValue(color));
	}

	enum Color : unsigned int {
		BLACK = 0xFF000000,
		BLUE = 0xFFAA0000,
		GREEN = 0xFF00AA00,
		CYAN = 0xFFAAAA00,
		RED = 0xFF0000AA,
		MAGENTA = 0xFFAA00AA,
		BROWN = 0xFF0055AA,
		LIGHTGRAY = 0xFFAAAAAA,
		DARKGRAY = 0xFF555555,
		LIGHTBLUE = 0xFFFF5555,
		LIGHTGREEN = 0xFF55FF55,
		LIGHTCYAN = 0xFFFFFF55,
		LIGHTRED = 0xFF5555FF,
		LIGHTMAGENTA = 0xFFFF55FF,
		YELLOW = 0xFF55FFFF,
		WHITE = 0xFFFFFFFF,
		ALPHA = 0x00000000
	};
	
	#define ERRORCOLOR noa::RGBA(152,0,136,0)

	#define BRIGHTER(color, brightnessIncrease) (noa::RGB(min(255, noa::GetRValue(color) + brightnessIncrease), min(255, noa::GetGValue(color) + brightnessIncrease), min(255, noa::GetBValue(color) + brightnessIncrease)))
	#define DARKER(color, darknessIncrease) (noa::RGB(max(0, noa::GetRValue(color) - darknessIncrease), max(0, noa::GetGValue(color) - darknessIncrease), max(0, noa::GetBValue(color) - darknessIncrease)))


}

#endif // !NOAENGINE_GRAPHIC