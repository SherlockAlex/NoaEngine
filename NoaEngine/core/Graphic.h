#ifndef NOAENGINE_GRAPHIC_H
#define NOAENGINE_GRAPHIC_H

#include <SDL2/SDL.h>

namespace noa {
	// Color constant
	#define	BLACK			0x000000
	#define	BLUE			0xAA0000
	#define	GREEN			0x00AA00
	#define	CYAN			0xAAAA00
	#define	RED				0x0000AA
	#define	MAGENTA			0xAA00AA
	#define	BROWN			0x0055AA
	#define	LIGHTGRAY		0xAAAAAA
	#define	DARKGRAY		0x555555
	#define	LIGHTBLUE		0xFF5555
	#define	LIGHTGREEN		0x55FF55
	#define	LIGHTCYAN		0xFFFF55
	#define	LIGHTRED		0x5555FF
	#define	LIGHTMAGENTA	0xFF55FF
	#define	YELLOW			0x55FFFF
	#define	WHITE			0xFFFFFF

	
	
	//È¡µÍÎ»
	#define LOBYTE(w)           ((Uint8)(w&0xff))
	
	// Color conversion macro
	#define BGR(color)	((((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16))
	
	#define RGB(r,g,b)  ((Uint32)(((Uint8)(r)|((Uint8)((Uint8)(g))<<8))|(((unsigned long)(Uint8)(b))<<16)))
	
	#define GetRValue(rgb)      (LOBYTE(rgb))
	#define GetGValue(rgb)      (LOBYTE(((unsigned short)(rgb)) >> 8))
	#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

	#define MULTICOLOR(color,mutiColor) (RGB(GetRValue(color)* (GetRValue(mutiColor) / 255.0), GetGValue(color)* (GetGValue(mutiColor) / 255.0), GetBValue(color)* (GetBValue(mutiColor) / 255.0)))
	#define ERRORCOLOR RGB(152,0,136)

	typedef Uint32 ColorRef;

	#define BRIGHTER(color,brightnessIncrease)(RGB(min(255, GetRValue(color) + brightnessIncrease), min(255, GetGValue(color) + brightnessIncrease), min(255, GetBValue(color) + brightnessIncrease)))
	#define DARKER(color,darknessIncrease)(RGB(max(0, GetRValue(color) - darknessIncrease), max(0, GetGValue(color) - darknessIncrease), max(0, GetBValue(color) - darknessIncrease)))
}

#endif // !NOAENGINE_GRAPHIC
