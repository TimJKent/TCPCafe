#include <Window/Window.h>
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#define GL_SILENCE_DEPRECATION

void Window::GLFWErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

Window::Window(const uint32_t width, const uint32_t height, const std::string& title)
: clearColor ({0.0,0.0,0.0,1.0})
{
    isValid = glfwInit();
    if(!isValid)
    {
        return;
    }

    glfwSetErrorCallback(Window::GLFWErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr){ /*return 1;*/}
       
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.FontGlobalScale = (1.5f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init("#version 130");
}

void Window::SetClearColor(const ImVec4& clearColor)
{
    this->clearColor = clearColor;
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::Close()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::BeginFrame()
{
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::DrawFrame()
{
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

std::pair<int, int> Window::GetWindowSize()
{
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);

    return {width, height};
}