#include "Graphic.h"

//unsigned char noa::LOBYTE(unsigned int w) {
//    return static_cast<unsigned char>(w & 0xff);
//}
//
//unsigned int noa::BGR(unsigned int color) {
//    return (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16);
//}
//
//unsigned int noa::RGB(unsigned char r, unsigned char g, unsigned char b) {
//    return static_cast<unsigned int>((static_cast<unsigned char>(r) | (static_cast<unsigned char>(g) << 8)) | (static_cast<unsigned int>(static_cast<unsigned char>(b)) << 16));
//}
//
//unsigned char noa::GetRValue(unsigned int rgb) {
//    return LOBYTE(rgb);
//}
//
//unsigned char noa::GetGValue(unsigned int rgb) {
//    return LOBYTE(static_cast<unsigned short>(rgb) >> 8);
//}
//
//unsigned char noa::GetBValue(unsigned int rgb) {
//    return LOBYTE(rgb >> 16);
//}
//
//unsigned int noa::RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
//    return static_cast<unsigned int>((static_cast<unsigned char>(r) | (static_cast<unsigned char>(g) << 8)) | ((static_cast<unsigned int>(static_cast<unsigned char>(b)) << 16) | (static_cast<unsigned int>(static_cast<unsigned char>(a)) << 24)));
//}
//
//unsigned char noa::GetAValue(unsigned int argb) {
//    return LOBYTE(argb >> 24);
//}
//
//unsigned int noa::MULTICOLOR(unsigned int color, unsigned int mutiColor) {
//    return RGBA(static_cast<unsigned char>(GetRValue(color) * (static_cast<float>(GetRValue(mutiColor)) / 255.0)),
//                static_cast<unsigned char>(GetGValue(color) * (static_cast<float>(GetGValue(mutiColor)) / 255.0)),
//                static_cast<unsigned char>(GetBValue(color) * (static_cast<float>(GetBValue(mutiColor)) / 255.0)),
//                GetAValue(color));
//}