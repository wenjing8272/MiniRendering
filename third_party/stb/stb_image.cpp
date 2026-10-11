// stb_image.cpp —— 全项目唯一一处定义 stb_image 实现的地方
#pragma warning(push)
#pragma warning(disable : 4244 4996)   // stb 在 MSVC 下会喷警告，只在这个文件里关掉

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"   // 用项目根相对路径，和你 tinyobjloader 的风格一致

#pragma warning(pop)