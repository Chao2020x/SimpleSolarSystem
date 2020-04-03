#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "stb/stb_image.h"
#include <vector>
#include <glad/glad.h>

class Texture
{
public:
	static unsigned int LoadTextureFromFile(const char* path);
	static unsigned int load_cubemap(std::vector<std::string> faces);
};

#endif // !__TEXTURE_H__
