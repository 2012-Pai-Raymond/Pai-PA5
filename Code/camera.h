#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum Gamemodes {
    DEV,
    EXPLORATION,
    OBSERVATION
};

enum Speedmodes {
    BRAKE,
    NORMAL,
    LUDICROUS
};

const float YAW = -90.0f;
const float PITCH = -30.0f;
const float SPEED = 30.f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Gamemodes gamemodeType = EXPLORATION;

    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    void setSpeed(glm::vec3 spd) { m_speed = spd; }
    void setRotation(float rotSpd) { m_rotSpeed = rotSpd; }

    void changeMovementSpeed(bool incOrDec, float amount, float deltaTime);
    void setGear(Speedmodes changeTo);
    void boosting(bool willBoost);

    void toggleGamemode();
    void toggleDevMode();
    Gamemodes getGamemode() { return gamemodeType; }

    void Update();

    glm::vec3 getFront() { return Front; }

    glm::vec3 getPosition() { return Position; }
    void addPosition(glm::vec3 pos);
    void setPosition(glm::vec3 pos);

    float getYaw() { return Yaw; }
    float getPitch() { return Pitch; }

    void setCameraPosInExplor() { cameraPosInExplor = Position;  }
    void resetCameraPos() { Position = cameraPosInExplor;  }
    void setYawInExplor() { YawInExplor = Yaw; }
    void resetYaw() { Yaw = YawInExplor; }
    void setPitchInExplor() { PitchInExplor = Pitch; }
    void resetPitch() { Pitch = PitchInExplor; }
private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 m_speed = glm::vec3(0., 0., 0.);
    glm::mat4 localTransform = glm::mat4(1.0f);

    glm::vec3 cameraPosInExplor;
    float YawInExplor;
    float PitchInExplor;

    float boostSpeed;
    bool isBoosting = false;
    float m_rotSpeed = 0.0f;

    int width;
    int height;
    void updateCameraVectors();
};

#endif /* CAMERA_H */
