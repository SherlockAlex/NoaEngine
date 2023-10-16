#ifndef NOAENGINE_GLHELPER_H
#define NOAENGINE_GLHELPER_H



#if defined(_WIN64) || defined(_WIN32)||defined(__linux__)

//#include <GL/gl.h>
#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <GL/glut.h>

#endif

#endif // !NOAENGINE_GLHELPER_H

