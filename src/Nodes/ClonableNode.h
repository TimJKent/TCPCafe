#pragma once

#include "Nodes/Node.h"

template <typename Derived>
class ClonableNode : public Node {
public:
    ClonableNode() : Node(){}
    std::shared_ptr<Node> Clone() const override {
        return std::make_shared<Derived>(static_cast<const Derived&>(*this));
    }
};