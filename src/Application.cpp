#include "Application.h"
#include "Window/FileDialogue.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Nodes/PrintNode.h"
#include "Nodes/PrintNode.h"
#include "Nodes/ButtonNode.h"
#include "Nodes/ToggleNode.h"
#include "Nodes/BooleanOperatorNode.h"
#include "Nodes/BooleanDisplayNode.h"
#include "Nodes/StringNode.h"
#include "Nodes/ConcatNode.h"
#include "Nodes/AddNode.h"
#include "Nodes/SubtractNode.h"
#include "Nodes/NumberNode.h"
#include "Nodes/TCPClientNode.h"
#include "Nodes/TCPServerNode.h"
#include "Nodes/TimerNode.h"
#include "Window/FileManager.h"

#include <filesystem>
#include <cmath>

Application::Application()
: window(1280, 720, "TCPCafe 0.0.1", "icon.png")
, activeMenu(MENU_NAME::NODE_EDITOR)
, ioContext()
, tcpClient(std::make_shared<TCPClient>(ioContext))
, tcpServer(std::make_shared<TCPServer>(ioContext))
, nodeManager(tcpServer, tcpClient)
{
    std::string segoeRegularPath = "C:\\Windows\\Fonts\\segoeui.ttf";
    if(FileManager::FileExists(segoeRegularPath))
    {
        font_SegoeUIRegularTiny = ImGui::GetIO().Fonts->AddFontFromFileTTF(segoeRegularPath.c_str(), 18);
        font_SegoeUIRegularSmall = ImGui::GetIO().Fonts->AddFontFromFileTTF(segoeRegularPath.c_str(), 20);
        font_SegoeUIRegularMedium = ImGui::GetIO().Fonts->AddFontFromFileTTF(segoeRegularPath.c_str(), 30);
        font_SegoeUIRegularLarge = ImGui::GetIO().Fonts->AddFontFromFileTTF(segoeRegularPath.c_str(), 50);
    }


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

    FileDialogue::Init();
    while (!window.ShouldClose())
    {
        ioContext.run();

        window.BeginFrame();
        
        DrawTitleBar();
       
        BeginMainPanel();
        switch(activeMenu)
        {
            case TCP_CLIENT: DrawTCPClientWindow(); break;
            case TCP_SERVER: DrawTCPServerWindow(); break;
            case NODE_EDITOR: DrawNodeEditor(); break;
        }
        
        AppWideShortcuts();
        EndMainPanel();
        
        window.DrawFrame();
    }

    FileDialogue::Cleanup();
    window.Close();

    return 0;
}


void Application::AppWideShortcuts()
{
    if(ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_N))
    {
        StartNewFile();
    }
    if(ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_O))
    {
        StartOpenFile();
    }
}

void Application::DrawTitleBar()
{
    ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 0, 11 ) );

    if(ImGui::BeginMainMenuBar())
    {
       // ImGui::SetCursorPosX(40);

        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("New File","Ctrl+N", false))
            {
                StartNewFile();
            }
            if(ImGui::MenuItem("Open File","Ctrl+O", false))
            {
               StartOpenFile();
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View"))
        {
            if(ImGui::MenuItem("TCP Client","m", activeMenu == MENU_NAME::TCP_CLIENT))
            {
                activeMenu = MENU_NAME::TCP_CLIENT;
            }
            if(ImGui::MenuItem("TCP Server","n", activeMenu == MENU_NAME::TCP_SERVER))
            {
                activeMenu = MENU_NAME::TCP_SERVER;
            }
            if(ImGui::MenuItem("Node Editor","n", activeMenu == MENU_NAME::NODE_EDITOR))
            {
                activeMenu = MENU_NAME::NODE_EDITOR;
            }

            ImGui::EndMenu();
        }

        if(activeMenu == MENU_NAME::NODE_EDITOR && session.IsActive())
        {
            nodeManager.SetEditorActive(true);
            if(ImGui::BeginMenu("Tools"))
            {
                if(ImGui::MenuItem("Select All", "Ctrl+A"))
                {
                    nodeManager.SelectAll();
                }
                if(ImGui::MenuItem("Clear Selection", "ESC"))
                {
                    nodeManager.UnselectAll();
                }
                if(ImGui::MenuItem("Duplicate Selected", "Ctrl+D"))
                {
                    nodeManager.DuplicateSelected();
                }
                ImGui::Separator();
                if(ImGui::MenuItem("Recenter Viewport", "Ctrl+R"))
                {
                    nodeManager.DoRecenter();
                }

                nodeManager.SetEditorActive(false);
    
                ImGui::EndMenu();
            }


            if(ImGui::BeginMenu("Nodes"))
            {
                std::shared_ptr<Node> spawnedNode = DrawNodeSpawnList();
                if(spawnedNode)
                {
                    ax::NodeEditor::SetNodePosition(spawnedNode->id, ax::NodeEditor::ScreenToCanvas({(float)window.GetWindowSize().first/2.0f, (float)window.GetWindowSize().second/2.0f}));
                }
                ImGui::EndMenu();
            }
        }
        
        std::filesystem::path activePath = session.GetActiveFileName();
        
        std::string filename = activePath.filename().string();
        filename.resize((int)std::min((double)filename.size(), (double)64));
      
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(filename.c_str()).x-8);
        ImGui::Text(filename.c_str());
        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleVar();
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

        std::string connectText = tcpClient->IsConnected() ? "Disconnect" : "Connect";

        if(ImGui::Button(connectText.c_str()))
        {
            if(!tcpClient->IsConnected())
            {
                tcpClient->Connect(ipBuf, std::stoi(portBuf));
            }
            else
            {
                tcpClient->Disconnect();
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
    
    AppendString(clientReceive, tcpClient->ConsumeRXData());
    if(tcpClient->IsConnected())
    {
        ImGui::Text("Send Message");
        tcpClientSendMessage1->Draw();
        tcpClientSendMessage2->Draw();
        tcpClientSendMessage3->Draw();
    }
}

void Application::DrawTCPServerWindow()
{
    AppendString(serverReceive, tcpServer->ConsumeRXData());
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

        std::string startListenText = tcpServer->IsListening() ? "Stop" : "Listen";

        if(ImGui::Button(startListenText.c_str()))
        {
            if(tcpServer->IsListening())
            {
                tcpServer->StopListen();
            }
            else
            {
                tcpServer->Listen(std::stoi(portBuf));
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

    if(tcpServer->IsListening())
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
    ImGui::SetNextWindowSize({(float)width,(float)height-42});
    ImGui::SetNextWindowPos({0,42});
    
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
    if(tcpServer->IsListening())
    {
        AppendString(serverSend, message);
        tcpServer->SendMessageA(message);
    }
}

void Application::SendMessageFromClient(const std::string& message)
{
    if(tcpClient->IsConnected())
    {
        AppendString(clientSend, message);
        tcpClient->SendMessageA(message);
    }
}

std::shared_ptr<Node> Application::DrawNodeSpawnList()
{    
    std::shared_ptr<Node> spawnedNode;
    if(ImGui::BeginMenu("Input"))
    {
        if (ImGui::MenuItem("Button"))
        {
            spawnedNode = nodeManager.SpawnNode<ButtonNode>(0);
        }
        if (ImGui::MenuItem("Toggle"))
        {
            spawnedNode = nodeManager.SpawnNode<ToggleNode>(0);
        }
        if (ImGui::MenuItem("Timer"))
        {
            spawnedNode =nodeManager.SpawnNode<TimerNode>(0);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("String"))
        {
            spawnedNode =nodeManager.SpawnNode<StringNode>(0);
        }
        if (ImGui::MenuItem("Number"))
        {
            spawnedNode =nodeManager.SpawnNode<NumberNode>(0);
        }
        ImGui::EndMenu();
    }

    if(ImGui::BeginMenu("Math")){
        if (ImGui::MenuItem("Concatenate"))
        {
            spawnedNode =nodeManager.SpawnNode<ConcatNode>(0);
        }
        ImGui::Separator();

        if (ImGui::MenuItem("Add"))
        {
            spawnedNode =nodeManager.SpawnNode<AddNode>(0);
        }
        if (ImGui::MenuItem("Subtract"))
        {
            spawnedNode =nodeManager.SpawnNode<SubtractNode>(0);
        }
        if (ImGui::MenuItem("Boolean Operator"))
        {
            spawnedNode = nodeManager.SpawnNode<BooleanOperatorNode>(0);
        }
            
        ImGui::EndMenu();
    }

    if(ImGui::BeginMenu("Output")){
        if (ImGui::MenuItem("Print"))
        {
            spawnedNode =nodeManager.SpawnNode<PrintNode>(0);
        }
        if(ImGui::MenuItem("Boolean Display"))
        {
            spawnedNode  = nodeManager.SpawnNode<BooleanDisplayNode>(0);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("TCP Client"))
        {
            spawnedNode =nodeManager.SpawnNode<TCPClientNode>(0, tcpClient);
        }
        if (ImGui::MenuItem("TCP Server"))
        {
            spawnedNode = nodeManager.SpawnNode<TCPServerNode>(0, tcpServer);
        }
        
        ImGui::EndMenu();
    }

    return spawnedNode;
}


void Application::DrawNodeEditor()
{

    if(session.IsActive())
    {
        ed::SetCurrentEditor(nodeManager.GetEditorContext());
        ed::Begin("My Editor", ImVec2(0.0, 0.0f));

        // Start drawing nodes.
        int nodePushID = 0;
        for (auto& node : nodeManager.GetNodes())
        {
            std::string id = "Node_" + std::to_string(nodePushID++);
            ImGui::PushID(id.c_str());
            node->Draw();
            ImGui::PopID();
        }

        //we need to only do stuff node related in this loop. Buttons will queue things up to happen in here

        nodeManager.Update();
        if(ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_D))
        {
            nodeManager.DuplicateSelected();
        }
        if(ImGui::Shortcut(ImGuiKey_Escape))
        {
            nodeManager.UnselectAll();  
        }
        if(ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_R))
        {
            nodeManager.DoRecenter();
        }
        if(ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_A))
        {
            if(nodeManager.GetSelectedNodes().size() > 0)
            {
                nodeManager.UnselectAll();
            }else
            {
                nodeManager.SelectAll();
            }
        }

        auto openPopupPosition = ImGui::GetMousePos();
        ed::End();
        ed::SetCurrentEditor(nullptr);
        std::shared_ptr<Node> spawnedNode;
        if (ImGui::BeginPopup("Create New Node"))
        {
            ImGui::Text("Add Node");
            ImGui::Separator();
            spawnedNode = DrawNodeSpawnList();

            ImGui::EndPopup();
        }

        if(spawnedNode)
        {
            ed::SetCurrentEditor(nodeManager.GetEditorContext());
            ed::SetNodePosition(spawnedNode->id, openPopupPosition);
            ed::SetCurrentEditor(nullptr);
        }
    
        if(ImGui::IsItemHovered())
        {
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
            {
                ImGui::OpenPopup("Create New Node");
            }
        }
    }
    else
    {
        float windowWidth = (float)window.GetWindowSize().first;
        float windowHeight = (float)window.GetWindowSize().second;
        float mainSectionSize =  (float)std::max(600.0f, windowWidth/1.25f);
         mainSectionSize =  (float)std::min(1000.0f, mainSectionSize);
        float otherSectionSize = std::max(20.0f, (windowWidth - mainSectionSize)/2.0f);
        ImGui::SetCursorPosY(std::max(30.0f, windowHeight/10.0f));
        if(ImGui::BeginTable("MainPageTable", 3))
        {
            ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthFixed, mainSectionSize);
            ImGui::TableSetupColumn("Column3", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(1);

            //Title
            ImGui::PushFont(font_SegoeUIRegularLarge);
            ImGui::Text("TCPCafe");
            ImGui::PopFont();
            
            ImGui::NewLine();
            
            if(ImGui::BeginTable("ActionsTable", 2))
            {
                ImGui::TableNextColumn();
                ImGui::PushFont(font_SegoeUIRegularMedium);
                ImGui::Text("Start");
                ImGui::PopFont();
                
                ImGui::PushFont(font_SegoeUIRegularSmall);
                if(ImGui::TextLink("New File..."))
                {
                    StartNewFile();
                }
                if(ImGui::TextLink("Open File..."))
                {
                    StartOpenFile();
                }
                ImGui::PopFont();

                
                ImGui::TableNextColumn();
                
                ImGui::PushFont(font_SegoeUIRegularMedium);
                ImGui::Text("Recent");
                ImGui::PopFont();
                
                int idCounter = 0;
                for(auto& recentName : session.GetRecentFiles())
                {
                    std::filesystem::path path = recentName;
                    ImGui::PushID(idCounter++);
                    if(ImGui::TextLink(path.stem().string().c_str()))
                    {
                        OpenFile(path.string());
                    }
                    ImGui::SameLine(0, 12.0f);
                    ImGui::Text(recentName.c_str());
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }

            ImGui::EndTable();
        }
    }
}

void Application::StartNewFile()
{
    std::string path = FileDialogue::GetPathForSave(session.GetActivePathAndFileName());
    if(!path.empty())
    {
        session.SetActivePath(path);
        nodeManager.SerializeToFile(path);
        session.AddRecentFile(path);
        UpdateWindowTitle();
    }
}

void Application::StartOpenFile()
{
    std::string path = FileDialogue::GetPathForOpen();
    if(!path.empty())
    {
        OpenFile(path);
    }
}


void Application::UpdateWindowTitle()
{
    std::string newWindowTitle = "TCPCafe 0.0.1 - " + session.GetActiveFileName();
    window.SetWindowTitle(newWindowTitle);
}

void Application::OpenFile(const std::string& path)
{
    nodeManager.QueueLoadFromFile(path);
    session.SetActivePath(path);
    session.AddRecentFile(path);
    UpdateWindowTitle();
}