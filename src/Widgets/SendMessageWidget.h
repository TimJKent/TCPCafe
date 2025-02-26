#pragma once
#include "Widgets/IWidget.h"
#include <string>
#include <functional>

class SendMessageWidget : public IWidget
{
public:
    SendMessageWidget(const std::string& id, std::function<void(const std::string&)> onButtonClick);
    void Draw() final;
private:
    std::string messageData;
    std::string id;
    std::function<void(const std::string&)> onButtonClick;
};