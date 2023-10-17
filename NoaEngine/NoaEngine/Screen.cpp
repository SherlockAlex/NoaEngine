#include "Screen.h"

int noa::Screen::width = 0;
int noa::Screen::height = 0;
noa::Vector<int> noa::Screen::hardwareScreenScale = noa::Vector<int>(0, 0);
uint32_t* noa::Screen::pixelBuffer = nullptr;
