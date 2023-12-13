#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sphere.h"
#include "mesh.h"
#include "AsteroidMesh.h"
#include "skybox.h"
#include "light.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void HierarchicalUpdate2(double dt);
    void Render();

    Camera* getCamera() { return m_camera; }
    Mesh* getSpaceship() { return m_mesh; }

private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale,
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    stack<glm::mat4> modelStack;

    Camera* m_camera;
    Shader* m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_normalMatrix;

    GLint m_positionAttrib;
    GLint m_normAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;
    GLint m_colorAttrib;

    GLint m_texNormalAttrib;
    GLint m_hasTexNorm;

    GLint m_astProjectionMatrix;
    GLint m_astViewMatrix;
    GLint m_astModelMatrix;
    GLint m_astNormalMatrix;
    GLint m_astPositionAttribLoc;
    GLint m_astNormalAttribLoc;
    GLint m_astTcAttribLoc;
    GLint m_astInstMatrixAttribLoc;

    GLint m_astHasTexture;

    GLint m_sbProjectionMatrix;
    GLint m_sbViewMatrix;

    Sphere* m_sphere;
    Sphere* m_sphere2;
    Sphere* m_sphere3;

    Light* m_light;

    SkyBox* m_skybox;

    Mesh* m_mesh;
    Sphere* Saturn;
    Sphere* Ring;
    Sphere* Jupiter;

    AsteroidMesh* m_asteroid;
    AsteroidMesh* m_asteroid2;


};

#endif /* GRAPHICS_H */