#include "camera.h"

Camera::Camera()
{
    Position = glm::vec3(0.0f, 20.0f, 50.0f);
    WorldUp = glm::vec3(0.0, 1.0, 0.0f);
    Front = glm::vec3(0.0, 0.0, 0.0);

    MovementSpeed = SPEED;
    boostSpeed = SPEED;
    Pitch = PITCH;
    Yaw = YAW;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;


    updateCameraVectors();
}

Camera::~Camera()
{

}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    if (gamemodeType == OBSERVATION) return; // Disable axis movement in Observation mode, the camera will just observe the planet rotating
    // 
    float velocity = MovementSpeed * deltaTime; 
    if (isBoosting) velocity = boostSpeed * deltaTime; // If we are boosting, use the higher speed

    // Depending on the specififed direction, we'll appropaitely modify the position
    if (direction == FORWARD) {
        Position += Front * velocity;
    }
    if (direction == BACKWARD) {
        Position -= Front * velocity;
    }
    if (direction == LEFT) {
        Position -= Right * velocity;
    }
    if (direction == RIGHT) {
        Position += Right * velocity;
    }
    if (direction == UP) {
        Position += Up * velocity;
    }
    if (direction == DOWN) {
        Position -= Up * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}
void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f) {
        Zoom = 1.0f;
    }
    if (Zoom > 70.0f) {
        Zoom = 70.0f;
    }
}
bool Camera::Initialize(int w, int h)
{
    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    /*view = glm::lookAt(glm::vec3(0.0, 10.0, -16.0), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up*/

    view = glm::lookAt(Position,
        Front + Position,
        WorldUp
    );
    

    projection = glm::perspective(glm::radians(90.f), //the FoV typically 90 degrees is good which is what this is set to
        float(w) / float(h), //Aspect Ratio, so Circles stay Circular
        0.1f, //Distance to the near plane, normally a small value like this
        100000.0f); //Distance to the far plane, 
    width = w;
    height = h;

    glm::vec3 scale = { .01,.01,.01 };
    glm::vec3 offsetY = { 0, -5, 0 };
    localTransform = glm::translate(glm::mat4(1.0f), offsetY);
    localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
    // Honestly not sure what localTransform does here. Might be a feature I axed at some point.
    // But I'd rather die than remove it and risk breaking something
    

    return true;
}

glm::mat4 Camera::GetProjection()
{
    return projection;
}

glm::mat4 Camera::GetView()
{
    return view;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
void Camera::Update() {
  
    view = glm::lookAt(Position,
        Front + Position,
        WorldUp
    );

    //Position = glm::rotateX(Position, glm::radians(0.1f));
    //WorldUp = glm::rotateX(WorldUp, glm::radians(0.1f));


    //std::cout << Front.x << "|| " << Front.y << "||" << Front.z << std::endl;
    projection = glm::perspective(glm::radians(Zoom), float(width) / float(height), 0.01f, 10000.0f);
    //view = glm::translate(view, m_speed);
    //view = glm::rotate(view, m_rotSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::changeMovementSpeed(bool incOrDec, float amount, float deltaTime) {
    // Increase or decrease movement speed depending on the inputs
    amount *= 10;
    float deltaSpeed = amount * deltaTime;
    if (incOrDec) {
        MovementSpeed += deltaSpeed;
    }
    else {
        if ((MovementSpeed -= deltaSpeed) < 5)
            MovementSpeed = 5;
        else
            MovementSpeed -= deltaSpeed;
    }
}

void Camera::setGear(Speedmodes changeTo) {
    // Proving the correct enum value changes Movement speed to pre-defined values, quicker than using the up/down keys
    switch (changeTo) {
        case BRAKE:
            MovementSpeed = 0;
            break;
        case NORMAL:
            MovementSpeed = SPEED;
            break;
        case LUDICROUS: // Yes, this is a spaceballs reference
            MovementSpeed = 200;
            break;
        default:
            std::cout << "Invalid speed type" << std::endl;
            break;
    }
}

void Camera::toggleGamemode() {
    // Depending on which mode we are switching into, change the gamemodetype var, reset movement speed, and move the camera slightly
    glm::vec3 cameraMoveOffset = { 0, -1.1f, -0.5f };
    if (gamemodeType == DEV) { // Set to exploration
        gamemodeType = EXPLORATION;
        MovementSpeed = SPEED;
        addPosition(-cameraMoveOffset);
    }
    else if (gamemodeType == EXPLORATION) { // Set to Observation
        gamemodeType = OBSERVATION;
        MovementSpeed = SPEED;
        addPosition(cameraMoveOffset);
    }
    else if (gamemodeType == OBSERVATION) { // Set to Exploration (dev mode normally inaccessible)
        gamemodeType = EXPLORATION;
        MovementSpeed = SPEED;
        resetCameraPos();
        resetPitch();
        resetYaw();
        addPosition(-cameraMoveOffset);
    }
}

void Camera::toggleDevMode() { // Mode used for debugging purposes
    gamemodeType = DEV;
    MovementSpeed = 100;
}

void Camera::boosting(bool willBoost) { // If the left shift key is being held down, willBoost will continously remain true, until you let go
    if (willBoost) {
        boostSpeed = MovementSpeed + 100;
        isBoosting = true;
    }
    else {
        boostSpeed = MovementSpeed;
        isBoosting = false;
    }
}

void Camera::addPosition(glm::vec3 pos) {
    Position += pos;
}

void Camera::setPosition(glm::vec3 pos) {
    Position = pos;
}