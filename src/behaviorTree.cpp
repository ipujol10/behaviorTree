#include "behaviorTree.hpp"

namespace BT {
Node::Node(const std::string &name) : name(name) {}

Status Node::tick() { throw -1; }

std::string Node::getName() const { return name; }
} // namespace BT
