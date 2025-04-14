#include <Window/Window.h>
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GL_SILENCE_DEPRECATION

void Window::GLFWErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

Window::Window(const uint32_t width, const uint32_t height, const std::string& title, const std::string& iconName)
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

    

    iconData.pixels = stbi_load(iconName.c_str(), &iconData.width, &iconData.height, 0, 4);
    if(iconData.pixels)
    {
        glfwSetWindowIcon(window, 1, &iconData); 
    }

    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = GetIconData().pixels;
    if (image_data == NULL)
    {
        loadedWindowIcon = false;
    }
    else
    {
        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GetIconData().width, GetIconData().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        loadedWindowIcon = true;
        windowIconTextureId = image_texture;
    }
}

void Window::SetWindowPosition(const ImVec2& newWindowPos)
{
    glfwSetWindowPos(window, (int)newWindowPos.x, (int)newWindowPos.y);
}

ImVec2 Window::GetScreenMousePosition() const
{
    double mouseX, mouseY;
    int windowX, windowY;

    glfwGetCursorPos(window, &mouseX, &mouseY);
    glfwGetWindowPos(window, &windowX, &windowY);
    
    return {(float)(windowX + (int)floor(mouseX)), (float)(windowY + (int)floor(mouseY))};
}

ImVec2 Window::GetWindowMousePosition() const
{
    double mouseX, mouseY;

    glfwGetCursorPos(window, &mouseX, &mouseY);
    
    return {(float)floor(mouseX), (float)floor(mouseY)};
}

Window::~Window()
{
    if(iconData.pixels)
    {
        stbi_image_free(iconData.pixels);
    }
}

GLFWimage& Window::GetIconData()
{
    return iconData;
}


bool Window::GetIconAsOpenGLTexture(uint32_t* out_texture)
{
    *out_texture = windowIconTextureId;
    return loadedWindowIcon;
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

void Window::SetWindowTitle(const std::string& newWindowTitle)
{
    glfwSetWindowTitle(window, newWindowTitle.c_str());
}