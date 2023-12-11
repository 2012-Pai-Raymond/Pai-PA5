#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "graphics_headers.h"

class Shader
{
public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    void AsteroidEnable();
    bool AddShader(GLenum ShaderType);
    bool AddAsteroidShader(GLenum ShaderType);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);
    GLint GetAttribLocation(const char* pAttribName);

    GLint GetAsUniformLocation(const char* pUniformName);
    GLint GetAsAttribLocation(const char* pUniformName);
    GLuint GetShaderProgram() { return m_shaderProg; }
    GLuint GetAstShaderProgram() { return m_asteroidShaderProg; }


private:
    GLuint m_shaderProg;
    GLuint m_asteroidShaderProg;

    std::vector<GLuint> m_shaderObjList;
    std::vector<GLuint> m_asteroidObjList;
};

#endif  /* SHADER_H */