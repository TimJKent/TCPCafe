#pragma once
#include "Widgets/IWidget.h"
#include <string>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>

class SendMessageWidget : public IWidget
{
public:
    enum TIME_UNITS
    {
        SECONDS, MILLISECONDS
    };
public:
    SendMessageWidget(const std::string& id, std::function<void(const std::string&)> onButtonClick);
    ~SendMessageWidget();

    void Draw();
    void RunRepRate();
private:
    bool repRate = false;
    std::string messageData;
    std::string id;
    int rate = 1;
    std::function<void(const std::string&)> onButtonClick;
    TIME_UNITS selectedUnits = SECONDS;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastRepRateCall;
    std::chrono::milliseconds repRateInMs = std::chrono::milliseconds(1000);
    std::thread repRateThread;
    std::atomic<bool> runRepRateThread;
};