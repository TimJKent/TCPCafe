#include "Nodes/Node.h"
#include "Nodes/NodeManager.h"
#include <Nodes/Pin.h>



Node::Node()
: id(NodeManager::globalId++)
{
        
}