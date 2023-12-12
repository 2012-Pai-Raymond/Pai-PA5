#include "camera.h"

Camera::Camera()
{
    Position = glm::vec3(0.0f, 0.0f, 10.0f);
    WorldUp = glm::vec3(0.0, 1.0, 0.0f);
    Front = glm::vec3(0.0, 0, -0.1f);

    MovementSpeed = SPEED;
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

    //std::cout << "Yaw: " << Yaw << std::endl;

    float velocity = MovementSpeed * deltaTime;
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
    if (Zoom > 45.0f) {
        Zoom - 45.0f;
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
        Front + Position + glm::vec3(0.0, 0.0, 0.0),
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

    //std::cout << Front.x << "|| " << Front.y << "||" << Front.z << std::endl;
    projection = glm::perspective(glm::radians(Zoom), float(width) / float(height), 0.001f, 10000.0f);
    //view = glm::translate(view, m_speed);
    //view = glm::rotate(view, m_rotSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
}

