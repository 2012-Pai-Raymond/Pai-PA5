#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include "graphics_headers.h"
#include "Texture.h"

class SkyBox
{
public:
	SkyBox(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back);
	void Render();
private:
	void setupVertAndBuffers();
	Texture* m_texture;

	GLuint skyboxVAO;
	GLuint skyboxVBO;
};
#endif