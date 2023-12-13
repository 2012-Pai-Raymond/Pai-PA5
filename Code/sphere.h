#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"
#include "Texture.h"

class Sphere :
    public Object
{
public:
    Sphere();


    void Render(GLint positionAttribLoc, GLint colorAttribLoc);
    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTextureLoc);
    void Render(GLint posAttribLoc, GLint normAttribLoc, GLint tcAttribLoc, GLint nmAttribLoc, GLint hasTextureLoc, GLint hasTexNormalLoc);

    glm::mat4 GetModel() { return model; }
    void Update(glm::mat4 matModel);

    Sphere(int prec);
    Sphere(int prec, const char* fname);
    Sphere(int prec, const char* fname, const char* f1name);

    int getNumVertices();
    int getNumIndices();
    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();

    GLuint getTextureID() { return m_texture->getTextureID(); }
    GLuint getTexNormalID() { return m_normal->getTextureID(); }

    void invertNormals() {for (auto& vert : Vertices) {vert.normal *= -1.F;}}

    bool hasTex;
    bool hasTexNorm;

private:
    glm::vec3 pivotLocation;
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    //GLuint TB;
    //GLuint NB;
    GLuint IB;
    Texture* m_texture;
    Texture* m_normal;

    GLuint vao;

    float angle;

    void setupVertices();
    void setupBuffers();
    void setupModelMatrix(glm::vec3 pivotLoc, float angle, float scale);

    int numVertices;
    int numIndices;
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    void init(int);
    float toRadians(float degrees);

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors

 

};
#endif