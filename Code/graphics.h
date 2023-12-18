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

enum ObservedPlanet {
    SUN,
    MERCURY,
    VENUS,
    EARTH,
    MARS,
    JUPITER,
    SATURN,
    URANUS,
    NEPTUNE,
    STATE_COUNT
}; // sorry pluto, you didn't make the cut

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

    void toggleObservedPlanet();
private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale,
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    glm::vec3 addOffsetToPlanetPosition();

    stack<glm::mat4> modelStack;
    glm::vec3 planetPositions[STATE_COUNT]; // Will store the position of the the planets (X,Y,Z) so that we can later get the camera to follow them
    glm::mat4 shipTransform;

    bool firstBoot = true;
    ObservedPlanet currentPlanet = SUN;

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


    //Sun, Earth, Moon
    Sphere* m_sphere;
    Sphere* m_sphere2;
    Sphere* m_sphere3;

    Light* m_light;

    SkyBox* m_skybox;

    Mesh* m_mesh;
    Mesh* Haley;
    Sphere* Mercury;
    Sphere* Venus;
    Sphere* Mars;
    Sphere* Saturn;
    Sphere* Ring;
    Sphere* Jupiter;
    Sphere* Uranus;
    Sphere* Neptune;
    Sphere* Ceres;
    Sphere* Eris;
    Sphere* Haumea;
    //Asteroid Belts
    AsteroidMesh* m_asteroid;
    AsteroidMesh* m_asteroid2;


};

#endif /* GRAPHICS_H */