#pragma once
#include "graphics_headers.h"
#include <SOIL2/SOIL2.h>

class Texture
{

public:
	Texture();
	Texture(const char* fileName);
	Texture(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back);
	bool loadTexture(const char* texFile);
	bool loadSkyBoxTex(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back);
	GLuint getTextureID() { return m_TextureID; }
	GLuint getSkyBoxTextureID() { return m_SkyBoxTexID; }
private:
	GLuint m_TextureID;
	GLuint m_SkyBoxTexID;

	bool initializeTexture();
	bool initializeSkyBoxTexture();

};

