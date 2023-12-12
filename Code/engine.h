#ifndef ENGINE_H
#define ENGINE_H


#include <assert.h>
#include "window.h"
#include "graphics.h"

//void cursorPositionCallBack(GLFWwindow*, double xpos, double ypos);
static Graphics* m_graphics;

class Engine
{
public:
    Engine(const char* name, int width, int height);

    ~Engine();
    bool Initialize();
    void Run();
    void ProcessInput(float deltaTime);
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    void Display(GLFWwindow*, double);
    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void cursor_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
    // Window related variables
    Window* m_window;

    const char* m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;

    bool m_running;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    double xpos;
    double ypos;

    double xoffset;
    double yoffset;

    float lastX;
    float lastY;
    bool firstMouse = true;
};

#endif // ENGINE_H
