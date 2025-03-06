#include "Nodes/Node.h"
#include "Nodes/NodeManager.h"



Node::Node()
: id(NodeManager::globalId++)
{
        
}