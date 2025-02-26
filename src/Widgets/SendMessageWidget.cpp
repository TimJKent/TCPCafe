#include "Widgets/SendMessageWidget.h"

SendMessageWidget::SendMessageWidget(const std::string& id, std::function<void(const std::string&)> onButtonClick)
: id(id)
, onButtonClick(onButtonClick)
{
    runRepRateThread = true;
    repRateThread = std::thread(&SendMessageWidget::RunRepRate, this);
}

SendMessageWidget::~SendMessageWidget()
{
    runRepRateThread = false;
    repRateThread.join();
}

void SendMessageWidget::RunRepRate()
{
    while(runRepRateThread)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if(repRate)
        {
            if(selectedUnits == SECONDS)
            {
                repRateInMs = std::chrono::milliseconds(rate * 1000);
            }
            else
            {
                repRateInMs = std::chrono::milliseconds(rate);
            }

            if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastRepRateCall) > repRateInMs)
            {
                onButtonClick(messageData);
                lastRepRateCall = std::chrono::high_resolution_clock::now();
            }
        }
    }
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
    ImGui::SameLine();
    if(ImGui::RadioButton("RepRate", repRate))
    {
        repRate = !repRate;
        lastRepRateCall = std::chrono::high_resolution_clock::now();
    }

    if(repRate)
    {
        std::string preview = selectedUnits == SECONDS ? "s" : "ms";
        ImGui::SetNextItemWidth(60);
        ImGui::SameLine();
        if(ImGui::BeginCombo("##units", preview.c_str()))
        {
            if(ImGui::Selectable("Seconds"))
            {
                if(selectedUnits == MILLISECONDS){rate /= 1000;}
                selectedUnits = SECONDS;
                lastRepRateCall = std::chrono::high_resolution_clock::now();
            }
            if(ImGui::Selectable("Milliseconds"))
            {
                if(selectedUnits == SECONDS){rate *= 1000;}
                selectedUnits = MILLISECONDS;
                lastRepRateCall = std::chrono::high_resolution_clock::now();
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(160);
        ImGui::InputInt("##Rate", &rate, 1, 100);
        if(rate <=1){rate = 1;}
    }

    ImGui::PopID();
}