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

    Gamemodes gamemodeType = EXPLORATION; // Stores the current gamemode (Exploration, Observation, & Dev)

    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    void setSpeed(glm::vec3 spd) { m_speed = spd; } // Unused
    void setRotation(float rotSpd) { m_rotSpeed = rotSpd; } // Unused

    void changeMovementSpeed(bool incOrDec, float amount, float deltaTime); // Used for moving the camera forward, left/right, backwards (and even up/down)
    void setGear(Speedmodes changeTo); // Used to set MovementSpeed to some hard coded values (like to zero for braking)
    void boosting(bool willBoost); // Temporarily increases MovemenetSpeed to create a boost effect

    void toggleGamemode(); // Cycles from one gamemode to the next (Exp->Obv). Dev mode will cycle to Expo mode but you cannot cycle back
    void toggleDevMode(); // Enables dev mode explortion (spaceship stops following camera but continues to render where it last was)
    Gamemodes getGamemode() { return gamemodeType; } // Returns the current gamemode, mainly used by Graphics class

    void Update();

    glm::vec3 getFront() { return Front; }

    glm::vec3 getPosition() { return Position; }
    void addPosition(glm::vec3 pos); // Adds the given position to the camera's current position
    void setPosition(glm::vec3 pos); // Sets the camera's current positon to the given position

    float getYaw() { return Yaw; }
    float getPitch() { return Pitch; }

    // These functions are used to save pos, yaw, & pitch when exiting Exploration mode as well as restoring them when returning to Exp mode.
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
