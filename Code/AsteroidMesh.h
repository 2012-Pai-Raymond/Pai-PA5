#ifndef ASTEROIDMESH_H
#define ASTEROIDMESH_H

#include <vector>
#include "graphics_headers.h"
#include "Texture.h"

class AsteroidMesh
{
	public:

		AsteroidMesh();
		AsteroidMesh(glm::vec3 pivot, const char* fname, const char* tname, unsigned int amountSet, float radiusSet, float setOffSet);

		~AsteroidMesh();
		void Update(glm::mat4 model);
		void Render(GLint postionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTex);

		glm::mat4 GetModel();
		bool InitBuffers();
		bool loadModelFromFile(const char* path);

		bool hasTex;
		GLuint getTextureID() { return m_texture->getTextureID(); }

	private:
		glm::vec3 pivotLocation;
		glm::mat4 model;
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		GLuint VB;
		GLuint IB;
		GLuint IM;

		GLuint vao;

		Texture* m_texture;

		unsigned int amount;
		float angle;
		float radius;
		float offset;
};

#endif

