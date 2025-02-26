#include "Application.h"
#include "misc/cpp/imgui_stdlib.h"

Application::Application()
: window(1280, 720, "TCPCafe 0.0.1")
, activeMenu(MENU_NAME::TCP_CLIENT)
, ioContext()
, tcpClient(ioContext)
, tcpServer(ioContext)
{
    tcpClientSendMessage1 = std::make_unique<SendMessageWidget>("tcpCTX1", [&](const std::string& message){SendMessageFromClient(message);});
    tcpClientSendMessage2 = std::make_unique<SendMessageWidget>("tcpCTX2", [&](const std::string& message){SendMessageFromClient(message);});
    tcpClientSendMessage3 = std::make_unique<SendMessageWidget>("tcpCTX3", [&](const std::string& message){SendMessageFromClient(message);});

    tcpServerSendMessage1 = std::make_unique<SendMessageWidget>("tcpSTX1", [&](const std::string& message){SendMessageFromServer(message);});
    tcpServerSendMessage2 = std::make_unique<SendMessageWidget>("tcpSTX2", [&](const std::string& message){SendMessageFromServer(message);});
    tcpServerSendMessage3 = std::make_unique<SendMessageWidget>("tcpSTX3", [&](const std::string& message){SendMessageFromServer(message);});
}

void AppendString(std::string& stringToAppend, const std::string& addition)
{
    for(auto& c : addition)
    {
        stringToAppend += c;
        if(stringToAppend.size() % 56 == 0)
        {
            stringToAppend += '\n';
        }
    }
}

int Application::Run()
{
    if(!window.IsValid()){return 1;}

    while (!window.ShouldClose())
    {
        ioContext.run();

        window.BeginFrame();
        
        DrawMainMenu();
       
        BeginMainPanel();
        switch(activeMenu)
        {
            case TCP_CLIENT: DrawTCPClientWindow(); break;
            case TCP_SERVER: DrawTCPServerWindow(); break;
        }
        EndMainPanel();

       window.DrawFrame();
    }

    window.Close();

    return 0;
}

void Application::DrawMainMenu()
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::MenuItem("Client","m", activeMenu == MENU_NAME::TCP_CLIENT))
        {
            activeMenu = MENU_NAME::TCP_CLIENT;
        }
        if(ImGui::MenuItem("Server","n", activeMenu == MENU_NAME::TCP_SERVER))
        {
            activeMenu = MENU_NAME::TCP_SERVER;
        }
        ImGui::EndMainMenuBar();
    }
}

void Application::DrawTCPClientWindow()
{
    int ipInputSize = 15;
    int portInputSize = 5;

    static std::string ipBuf  = "127.0.0.1";
    static std::string portBuf  = "65535";

    if(ImGui::BeginTable("##table", 3, ImGuiTabBarFlags_None, {600.f,100.f}))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("IP Address");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Port");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);

        ImGui::InputText("##ipAddressInputText", &ipBuf, ipInputSize);
        ImGui::TableSetColumnIndex(1);
        ImGui::InputText("##portInputText", &portBuf, portInputSize);

        ImGui::TableSetColumnIndex(2);

        std::string connectText = tcpClient.IsConnected() ? "Disconnect" : "Connect";

        if(ImGui::Button(connectText.c_str()))
        {
            if(!tcpClient.IsConnected())
            {
                tcpClient.Connect(ipBuf, std::stoi(portBuf));
            }
            else
            {
                tcpClient.Disconnect();
            }
        }
        
    ImGui::EndTable();
    }

    if(ImGui::BeginTable("##dataTable", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Receive");
        ImGui::SameLine();
        ImGui::PushID("RECEIVECLEAR");
        if(ImGui::Button("Clear"))
        {
            clientReceive.clear();
        }
        ImGui::PopID();
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Send");
        ImGui::SameLine();
        ImGui::PushID("SENDCLEAR");
        if(ImGui::Button("Clear"))
        {
            clientSend.clear();
        }
        ImGui::PopID();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::InputTextMultiline("##Receive", &clientReceive, {600.0f, 400.0f}, ImGuiInputTextFlags_ReadOnly );
        ImGui::TableSetColumnIndex(1);
        ImGui::InputTextMultiline("##Send", &clientSend, {600.0f, 400.0f}, ImGuiInputTextFlags_ReadOnly);
        ImGui::EndTable();
    }
    
    AppendString(clientReceive, tcpClient.ConsumeRXData());
    if(tcpClient.IsConnected())
    {
        ImGui::Text("Send Message");
        tcpClientSendMessage1->Draw();
        tcpClientSendMessage2->Draw();
        tcpClientSendMessage3->Draw();
    }
}

void Application::DrawTCPServerWindow()
{
    AppendString(serverReceive, tcpServer.ConsumeRXData());
    int portInputSize = 5;

    static std::string portBuf  = "65535";

    if(ImGui::BeginTable("##table", 2, ImGuiTabBarFlags_None, {600.f,100.f}))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Port");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::InputText("##portInputText", &portBuf, portInputSize);

        ImGui::TableSetColumnIndex(1);

        std::string startListenText = tcpServer.IsListening() ? "Stop" : "Listen";

        if(ImGui::Button(startListenText.c_str()))
        {
            if(tcpServer.IsListening())
            {
                tcpServer.StopListen();
            }
            else
            {
                tcpServer.Listen(std::stoi(portBuf));
            }
        }
        ImGui::EndTable();
    }

    
    if(ImGui::BeginTable("##dataTable", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Receive");
        ImGui::SameLine();
        ImGui::PushID("RECEIVECLEAR");
        if(ImGui::Button("Clear"))
        {
            serverReceive.clear();
        }
        ImGui::PopID();
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Send");
        ImGui::SameLine();
        ImGui::PushID("SENDCLEAR");
        if(ImGui::Button("Clear"))
        {
            serverSend.clear();
        }
        ImGui::PopID();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::InputTextMultiline("##Receive", &serverReceive, {600.0f, 400.0f}, ImGuiInputTextFlags_ReadOnly );
        ImGui::TableSetColumnIndex(1);
        ImGui::InputTextMultiline("##Send", &serverSend, {600.0f, 400.0f}, ImGuiInputTextFlags_ReadOnly);
        ImGui::EndTable();
    }

    if(tcpServer.IsListening())
    {
        ImGui::Text("Send Message");
        tcpServerSendMessage1->Draw();
        tcpServerSendMessage2->Draw();
        tcpServerSendMessage3->Draw();
    }
}

void Application::BeginMainPanel()
{
    auto[width, height] = window.GetWindowSize();
    ImGui::SetNextWindowSize({(float)width,(float)height});
    ImGui::SetNextWindowPos({0,25});
    
    bool open = true;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoDecoration;
    ImGui::Begin("TCPCafe", &open, flags);
}

void Application::EndMainPanel()
{
    ImGui::End();
}

void Application::SendMessageFromServer(const std::string& message)
{
    if(tcpServer.IsListening())
    {
        AppendString(serverSend, message);
        tcpServer.SendMessageA(message);
    }
}

void Application::SendMessageFromClient(const std::string& message)
{
    if(tcpClient.IsConnected())
    {
        AppendString(clientSend, message);
        tcpClient.SendMessageA(message);
    }
}