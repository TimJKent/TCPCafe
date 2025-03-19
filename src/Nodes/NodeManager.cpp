#include "Nodes/NodeManager.h"
#include "Nodes/Node.h"
#include "Nodes/Pin.h"

#include <iostream>
#include <algorithm>

int NodeManager::globalId = 100;

NodeManager::NodeManager()
{
    ax::NodeEditor::Config config;
    config.NavigateButtonIndex = 2;
    nodeEditorContext = ax::NodeEditor::CreateEditor(&config);
}

ax::NodeEditor::EditorContext* NodeManager::GetEditorContext()
{
    return nodeEditorContext;
}

void NodeManager::Update()
{
    for(auto& node : nodes)
    {
        node->Update();
    }

    for (auto& linkInfo : links)
    {
        ax::NodeEditor::Link(linkInfo.ID, linkInfo.StartPinID, linkInfo.EndPinID);
        
        std::shared_ptr<Pin> inputPin = GetPinFromId(linkInfo.StartPinID);
        std::shared_ptr<Pin> outputPin = GetPinFromId(linkInfo.EndPinID);

        inputPin->active = outputPin->active;
        inputPin->any = outputPin->any;
    }

    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ax::NodeEditor::BeginCreate())
    {
        ax::NodeEditor::PinId inputPinId, outputPinId;
        if (ax::NodeEditor::QueryNewLink(&outputPinId, &inputPinId))
        {
            // QueryNewLink returns true if editor want to create new link between pins.
            //
            // Link can be created only for two valid pins, it is up to you to
            // validate if connection make sense. Editor is happy to make any.
            //
            // Link always goes from input to output. User may choose to drag
            // link from output pin or input pin. This determine which pin ids
            // are valid and which are not:
            //   * input valid, output invalid - user started to drag new ling from input pin
            //   * input invalid, output valid - user started to drag new ling from output pin
            //   * input valid, output valid   - user dragged link over other pin, can be validated

            bool accept = false;
            std::shared_ptr<Pin> inputPin = GetPinFromId(inputPinId);
            std::shared_ptr<Pin> outputPin = GetPinFromId(outputPinId);

            if(inputPin->pinKind == ax::NodeEditor::PinKind::Input  && outputPin->pinKind == ax::NodeEditor::PinKind::Output)
            {
                bool matchesType  = inputPin->pinType == outputPin->pinType || (inputPin->pinType == Pin::PinType::Any && outputPin->pinType != Pin::PinType::Trigger);
                bool pinIsntFull = !inputPin->active || inputPin->pinType == Pin::PinType::Trigger;
                if(matchesType && pinIsntFull)
                {
                    accept = true;
                }
            }
            
            if (accept)
            {
                // ed::AcceptNewItem() return true when user release mouse button.
                if (ax::NodeEditor::AcceptNewItem())
                {
                    // Since we accepted new link, lets add one to our list of links.
                    links.push_back({ ax::NodeEditor::LinkId(NodeManager::globalId++), inputPinId, outputPinId });
                    
                    // Draw new link.
                    ax::NodeEditor::Link(links.back().ID, links.back().StartPinID, links.back().EndPinID);
                }
            }
            else
            {
                ax::NodeEditor::RejectNewItem(ImColor(255, 0, 0), 1.0f);
            }
        }
        ax::NodeEditor::EndCreate(); // Wraps up object creation action handling.
    }

    // Handle deletion action
    if (ax::NodeEditor::BeginDelete())
    {
        ax::NodeEditor::NodeId deletedNodeId;
        while (ax::NodeEditor::QueryDeletedNode(&deletedNodeId))
        {
            // If you agree that link can be deleted, accept deletion.
            if (ax::NodeEditor::AcceptDeletedItem())
            {
                // Then remove link from your data.
                for (auto& node : nodes)
                {
                    if (node->id == deletedNodeId)
                    {
                        nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
                        break;
                    }
                }       
            }

            // You may reject node deletion by calling:
            // ed::RejectDeletedItem();
        }

        // There may be many links marked for deletion, let's loop over them.
        ax::NodeEditor::LinkId deletedLinkId;
        while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId))
        {
            // If you agree that link can be deleted, accept deletion.
            if (ax::NodeEditor::AcceptDeletedItem())
            {
                // Then remove link from your data.
                for (auto& link : links)
                {
                    if (link.ID == deletedLinkId)
                    {
                        std::shared_ptr<Pin> inputPin = GetPinFromId(link.EndPinID);
                        std::shared_ptr<Pin> outputPin = GetPinFromId(link.StartPinID);
                            
                        if(inputPin)
                        {
                            inputPin->any.reset();
                            inputPin->active = false;
                        }
                        if(outputPin)
                        {
                            outputPin->any.reset();
                            outputPin->active = false;
                        }

                        links.erase(std::remove(links.begin(), links.end(), link), links.end());
                        break;
                    }
                }
            }

            // You may reject link deletion by calling:
            // ed::RejectDeletedItem();
        }

        ax::NodeEditor::EndDelete(); // Wrap up deletion action
    }   
}

std::shared_ptr<Pin> NodeManager::GetPinFromId(ax::NodeEditor::PinId pinId)
{
    for (auto& node : nodes)
    {
        for(std::shared_ptr<Pin> pin : node->GetPins())
        {
            if(pin->id == pinId)
            {
                return pin;
            }
        }
    }
    return nullptr;
}