#include "Nodes/Node.h"

int Node::globalId = 100;

Node::Node()
: id(Node::globalId++)
{
    
}