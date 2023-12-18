
#include "engine.h"

Engine::Engine(const char* name, int width, int height)
{
    m_WINDOW_NAME = name;
    m_WINDOW_WIDTH = width;
    m_WINDOW_HEIGHT = height;

    lastX = width / 2.0f;
    lastY = height / 2.0f;
    bool firstMouse = true;
}


Engine::~Engine()
{
    delete m_window;
    delete m_graphics;
    m_window = NULL;
    m_graphics = NULL;
}

void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stdout, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

bool Engine::Initialize()
{
    // Start a window
    m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
    if (!m_window->Initialize())
    {
        printf("The window failed to initialize.\n");
        return false;
    }

    // Start the graphics
    m_graphics = new Graphics();
    if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
    {
        printf("The graphics failed to initialize.\n");
        return false;
    }

    //glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallBack);

    glfwSetScrollCallback(m_window->getWindow(), scroll_callback);
    glfwSetKeyCallback(m_window->getWindow(), key_callback);
    glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    // No errors
    return true;
}

void Engine::Run()
{
    m_running = true;

    while (!glfwWindowShouldClose(m_window->getWindow()))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(deltaTime);
        Display(m_window->getWindow(), glfwGetTime());
        glfwPollEvents();
    }
    m_running = false;

}

void Engine::ProcessInput(float deltaTime)
{
    // Gameplay
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_H) == GLFW_PRESS)
        m_graphics->getCamera()->toggleDevMode();

    // Movement
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_graphics->getCamera()->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_graphics->getCamera()->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_graphics->getCamera()->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_graphics->getCamera()->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_graphics->getCamera()->ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        m_graphics->getCamera()->ProcessKeyboard(UP, deltaTime);
    // Speed up & speed down
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
        m_graphics->getCamera()->changeMovementSpeed(true, 2, deltaTime);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
        m_graphics->getCamera()->changeMovementSpeed(false, 2, deltaTime);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_graphics->getCamera()->boosting(true);
    else
        m_graphics->getCamera()->boosting(false);
    // Braking
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_B) == GLFW_PRESS)
        m_graphics->getCamera()->setGear(BRAKE);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_N) == GLFW_PRESS)
        m_graphics->getCamera()->setGear(NORMAL);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_L) == GLFW_PRESS) // Yes, this is a Spaceballs reference
        m_graphics->getCamera()->setGear(LUDICROUS);



    cursor_position_callback(m_window->getWindow(), xpos, ypos);
    cursor_scroll_callback(m_window->getWindow(), xoffset, yoffset);

    /*if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_X) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_Y) == GLFW_RELEASE)
         m_graphics->getCamera()->setSpeed(glm::vec3(0., 0., 0.));
         m_graphics->getCamera()->setRotation(0.0f);

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_graphics->getCamera()->setSpeed(glm::vec3(0., -.1, 0.));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_graphics->getCamera()->setSpeed(glm::vec3(0., .1, 0.));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_graphics->getCamera()->setSpeed(glm::vec3(-.1, 0., 0.));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_graphics->getCamera()->setSpeed(glm::vec3(.1, 0., 0.));


    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_X) == GLFW_PRESS)
        m_graphics->getCamera()->setRotation(0.1f);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_Y) == GLFW_PRESS)
        m_graphics->getCamera()->setRotation(-0.1f);*/
}

unsigned int Engine::getDT()
{
    //long long TimeNowMillis = GetCurrentTimeMillis();
    //assert(TimeNowMillis >= m_currentTimeMillis);
    //unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
    //m_currentTimeMillis = TimeNowMillis;
    //return DeltaTimeMillis;
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
    //timeval t;
    //gettimeofday(&t, NULL);
    //long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
    //return ret;
    return (long long)glfwGetTime();
}

void Engine::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

    glfwGetCursorPos(window, &xpos, &ypos);
    //std::cout << "Position: (" << xpos << ":" << ypos << ")" << endl;

    float xposIn = static_cast<float>(xpos);
    float yposIn = static_cast<float>(ypos);

    if (firstMouse) {
        lastX = xposIn;
        lastY = yposIn;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    m_graphics->getCamera()->ProcessMouseMovement(xoffset, yoffset);
}

void Engine::cursor_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

    m_graphics->getCamera()->ProcessMouseScroll(yoffset);
}
void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();
    m_window->Swap();

    // update graphics -> updates model
    //m_graphics->Update(time);
    m_graphics->HierarchicalUpdate2(time);
}

/*void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos) {
    //cout << xpos << " " << ypos << endl;
}*/
void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    m_graphics->getCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
}

void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
        m_graphics->getCamera()->toggleGamemode();
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
        m_graphics->toggleObservedPlanet();
}