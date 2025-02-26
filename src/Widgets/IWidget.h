#pragma once
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

class IWidget
{
    virtual void Draw() = 0;
};