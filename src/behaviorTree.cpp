#include "behaviorTree.hpp"

namespace BT {
Node::Node(const std::string &name) : name(name) {}

Status Node::tick() { throw -1; }

std::string Node::getName() const { return name; }

Sequence::Sequence(const std::string &name, const Node **nodes, int length)
    : length(length), Node(name), counter(0) {
  this->nodes = new const Node *[length];
  for (int i = 0; i < length; i++) {
    this->nodes[i] = nodes[i];
  }
}

Sequence::~Sequence() { delete[] nodes; }

int Sequence::getLength() const { return length; }
} // namespace BT
