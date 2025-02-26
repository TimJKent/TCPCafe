#include "Application.h"
#include "misc/cpp/imgui_stdlib.h"

Application::Application()
: window(1280, 720, "TCPCafe 0.0.1")
, activeMenu(MENU_NAME::TCP_CLIENT)
, ioContext()
, tcpServer(ioContext)
{
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
        tcpServer.Run();
        tcpClient.Run();
        
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
    AppendString(serverReceive, tcpServer.ConsumeRXData());
    if(tcpClient.IsConnected())
    {
        ImGui::Text("Send Message");
        ImGui::InputText("##Message1", &clientMessage1);
        ImGui::SameLine();
        ImGui::PushID("Send1");
        if(ImGui::Button("Send"))
        {
            AppendString(clientSend, clientMessage1);
            tcpClient.SendMessageA(clientMessage1);
        }
        ImGui::PopID();
        ImGui::InputText("##Message2", &clientMessage2);
        ImGui::SameLine();
        ImGui::PushID("Send2");
        if(ImGui::Button("Send"))
        {
            AppendString(clientSend, clientMessage2);
            tcpClient.SendMessageA(clientMessage2);
        }
        ImGui::PopID();
        
        ImGui::InputText("##Message3", &clientMessage3);
        ImGui::SameLine();
        ImGui::PushID("Send3");
        if(ImGui::Button("Send"))
        {
            AppendString(clientSend, clientMessage3);
            tcpClient.SendMessageA(clientMessage3);
        }
        ImGui::PopID();
    }
}

void Application::DrawTCPServerWindow()
{
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
        ImGui::InputText("##Message1", &serverMessage1);
        ImGui::SameLine();
        ImGui::PushID("Send1");
        if(ImGui::Button("Send"))
        {
            AppendString(serverSend, serverMessage1);
            tcpServer.SendMessageA(serverMessage1);
        }
        ImGui::PopID();
        ImGui::InputText("##Message2", &serverMessage2);
        ImGui::SameLine();
        ImGui::PushID("Send2");
        if(ImGui::Button("Send"))
        {
            AppendString(serverSend, serverMessage2);
            tcpServer.SendMessageA(serverMessage2);
        }
        ImGui::PopID();
        
        ImGui::InputText("##Message3", &serverMessage3);
        ImGui::SameLine();
        ImGui::PushID("Send3");
        if(ImGui::Button("Send"))
        {
            AppendString(serverSend, serverMessage3);
            tcpServer.SendMessageA(serverMessage3);
        }
        ImGui::PopID();
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