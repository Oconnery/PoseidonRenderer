#pragma once

#include <GL/glew.h>

// __debugbreak is specific to msvc and it won't work on clang or gcc or other compilers. 
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);