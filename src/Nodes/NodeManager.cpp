#include "Nodes/NodeManager.h"
#include "Nodes/PrintNode.h"

#include <iostream>

int NodeManager::globalId = 100;

NodeManager::NodeManager()
{
    nodeEditorContext = ax::NodeEditor::CreateEditor();
}

ax::NodeEditor::EditorContext* NodeManager::GetEditorContext()
{
    return nodeEditorContext;
}

void NodeManager::SpawnInputActionNode()
{
    nodes.emplace_back(std::shared_ptr<PrintNode>(new PrintNode()));
}

void NodeManager::Update()
{
    for (auto& linkInfo : links)
    {
        ax::NodeEditor::Link(linkInfo.ID, linkInfo.StartPinID, linkInfo.EndPinID);
    }

    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ax::NodeEditor::BeginCreate())
    {
        ax::NodeEditor::PinId inputPinId, outputPinId;
        if (ax::NodeEditor::QueryNewLink(&inputPinId, &outputPinId))
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

            if (inputPinId && outputPinId) // both are valid, let's accept link
            {
                // ed::AcceptNewItem() return true when user release mouse button.
                if (ax::NodeEditor::AcceptNewItem())
                {
                    // Since we accepted new link, lets add one to our list of links.
                    links.push_back({ ax::NodeEditor::LinkId(NodeManager::globalId++), inputPinId, outputPinId });

                    // Draw new link.
                    ax::NodeEditor::Link(links.back().ID, links.back().StartPinID, links.back().EndPinID);
                }

                // You may choose to reject connection between these nodes
                // by calling ed::RejectNewItem(). This will allow editor to give
                // visual feedback by changing link thickness and color.
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

            // You may reject link deletion by calling:
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