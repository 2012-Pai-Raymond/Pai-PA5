#include "Texture.h"

Texture::Texture(const char* fileName) {

	loadTexture(fileName);
	initializeTexture();
}

Texture::Texture(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back) {

	loadSkyBoxTex(right, left, top, bottom, front, back);
	initializeSkyBoxTexture();
}

Texture::Texture() {
	m_TextureID = -1;
	m_SkyBoxTexID = -1;
	printf("No Texture Data Provided.");
}

bool Texture::loadTexture(const char* texFile) {
	m_TextureID = SOIL_load_OGL_texture(texFile, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (!m_TextureID) {
		printf("Failed: Could not open texture file!\n");
		return false;
	}
	return true;
}

bool Texture::loadSkyBoxTex(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back) {
	m_SkyBoxTexID = SOIL_load_OGL_cubemap(right, left, top, bottom, front, back, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (!m_SkyBoxTexID) {
		printf("Failed: Could not open one or more skybox texture files!\n");
		return false;
	}
	return true;
}
bool Texture::initializeTexture() {

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);

	
	return true;
}

bool Texture::initializeSkyBoxTexture() {

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



	return true;
}



