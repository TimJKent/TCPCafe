#pragma once
#include <string>
#include <stdint.h>
#include "imgui.h"
#include <utility>
#include <GLFW/glfw3.h>


class Window
{
private:
    static void GLFWErrorCallback(int error, const char* description);
public:
    Window(const uint32_t width, const uint32_t height, const std::string& title, const std::string& iconName);
    ~Window();
    void BeginFrame();
    void DrawFrame();
    void Close();
    void SetClearColor(const ImVec4& clearColor);
    bool ShouldClose();
    bool IsValid(){return isValid;}
    GLFWimage& GetIconData();
    std::pair<int, int> GetWindowSize();
    bool GetIconAsOpenGLTexture(unsigned int* out_texture);
    GLFWwindow* GetNativeHandle() {return window;}
private:
    bool isValid = false;
    GLFWwindow* window;
    ImVec4 clearColor;
    GLFWimage iconData;
};