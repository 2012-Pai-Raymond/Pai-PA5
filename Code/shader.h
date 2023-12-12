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
    void SkyBoxEnable();
    bool AddShader(GLenum ShaderType);
    bool AddAsteroidShader(GLenum ShaderType);
    bool AddSkyBoxShader(GLenum ShaderType);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);
    GLint GetAttribLocation(const char* pAttribName);

    GLint GetAsUniformLocation(const char* pUniformName);
    GLint GetAsAttribLocation(const char* pUniformName);

    GLint GetSbUniformLocation(const char* pUniformName);
    GLint GetSbAttribLocation(const char* pAttribName);

    GLuint GetShaderProgram() { return m_shaderProg; }
    GLuint GetAstShaderProgram() { return m_asteroidShaderProg; }


private:
    GLuint m_shaderProg;
    GLuint m_asteroidShaderProg;
    GLuint m_skyboxProg;

    std::vector<GLuint> m_shaderObjList;
    std::vector<GLuint> m_asteroidObjList;
    std::vector<GLuint> m_skyboxObjList;
};

#endif  /* SHADER_H */