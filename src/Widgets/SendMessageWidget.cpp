#include "Widgets/SendMessageWidget.h"

SendMessageWidget::SendMessageWidget(const std::string& id, std::function<void(const std::string&)> onButtonClick)
: id(id)
, onButtonClick(onButtonClick)
{

}

void SendMessageWidget::Draw()
{
    ImGui::InputText(std::string("##"+id).c_str(), &messageData);
    ImGui::SameLine();
    ImGui::PushID(id.c_str());
    if(ImGui::Button("Send"))
    {
        onButtonClick(messageData);
    }
    ImGui::PopID();
}