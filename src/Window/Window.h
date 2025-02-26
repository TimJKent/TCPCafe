#pragma once
#include <string>
#include <stdint.h>
#include "imgui.h"
#include <utility>

struct GLFWwindow;

class Window
{
private:
    static void GLFWErrorCallback(int error, const char* description);
public:
    Window(const uint32_t width, const uint32_t height, const std::string& title);
    void BeginFrame();
    void DrawFrame();
    void Close();
    void SetClearColor(const ImVec4& clearColor);
    bool ShouldClose();
    bool IsValid(){return isValid;}
    std::pair<int, int> GetWindowSize();
    GLFWwindow* GetNativeHandle() {return window;}
private:
    bool isValid = false;
    GLFWwindow* window;
    ImVec4 clearColor;
};