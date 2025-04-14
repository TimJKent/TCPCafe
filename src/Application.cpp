#include "Application.h"
#include "Window/FileDialogue.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Nodes/PrintNode.h"
#include "Nodes/PrintNode.h"
#include "Nodes/ButtonNode.h"
#include "Nodes/StringNode.h"
#include "Nodes/ConcatNode.h"
#include "Nodes/AddNode.h"
#include "Nodes/SubtractNode.h"
#include "Nodes/NumberNode.h"
#include "Nodes/TCPClientNode.h"
#include "Nodes/TCPServerNode.h"
#include "Nodes/TimerNode.h"

#include <filesystem>

Application::Application()
: window(1280, 720, "TCPCafe 0.0.1", "icon.png")
, activeMenu(MENU_NAME::TCP_CLIENT)
, ioContext()
, tcpClient(std::make_shared<TCPClient>(ioContext))
, tcpServer(std::make_shared<TCPServer>(ioContext))
, nodeManager(tcpServer, tcpClient)
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
        EndMainPanel();

       window.DrawFrame();
    }

    FileDialogue::Cleanup();
    window.Close();

    return 0;
}

void Application::DrawTitleBar()
{
    ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 0, 11 ) );

    static bool isUserDraggingWindow = false;
    static ImVec2 dragOffset = {0.0f,0.0f};

    

    if(ImGui::BeginMainMenuBar())
    {
       // ImGui::SetCursorPosX(40);

        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Import","n", false))
            {
                std::string path = FileDialogue::GetPathForOpen();
                if(!path.empty())
                {
                    nodeManager.QueueLoadFromFile(path);
                    activeFileName = path;
                    UpdateWindowTitle();
                }
            }
            if(ImGui::MenuItem("Export","n", false))
            {
                std::string path = FileDialogue::GetPathForSave(activeFileName);
                if(!path.empty())
                {
                    nodeManager.SerializeToFile(path);
                }
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

        if(activeMenu == MENU_NAME::NODE_EDITOR)
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
        
        //unsigned int my_image_texture = 0;
        //bool ret = window.GetIconAsOpenGLTexture(&my_image_texture);
        //ImVec2 storedCursorPos = ImGui::GetCursorPos();
        //ImGui::SetCursorPos({10,10});
        //ImGui::Image((ImTextureID)(intptr_t)my_image_texture, ImVec2( 20, 20));

        std::filesystem::path activePath = activeFileName;
        
        std::string filename = activePath.filename().string();
        filename.resize((int)std::min((double)filename.size(), (double)30));
        
        ImGui::SetCursorPosX(ImGui::GetWindowWidth()-11*filename.size()-4);
        ImGui::Text(filename.c_str());
        ImGui::EndMainMenuBar();
    }

    

   //if(ImGui::IsMouseDragging(ImGuiMouseButton_Left))
   //{
   //    if(!isUserDraggingWindow)
   //    {
   //        dragOffset = window.GetWindowMousePosition();
   //        isUserDraggingWindow = true;
   //    }
   //    ImVec2 globalMousePos = window.GetScreenMousePosition();
   //    ImVec2 newWindowPosition = {globalMousePos.x - dragOffset.x, globalMousePos.y - dragOffset.y};

   //    window.SetWindowPosition(newWindowPosition);
   //}else
   //{
   //    isUserDraggingWindow = false;
   //}

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
        if (ImGui::MenuItem("Timer"))
        {
            spawnedNode =nodeManager.SpawnNode<TimerNode>(0);
        }
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
        ImGui::EndMenu();
    }

    if(ImGui::BeginMenu("Output")){
        if (ImGui::MenuItem("Print"))
        {
            spawnedNode =nodeManager.SpawnNode<PrintNode>(0);
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


void Application::UpdateWindowTitle()
{
    std::string newWindowTitle = "TCPCafe 0.0.1 - " + activeFileName;
    window.SetWindowTitle(newWindowTitle);
}