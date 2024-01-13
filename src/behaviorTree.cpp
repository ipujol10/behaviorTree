#include "behaviorTree.hpp"

namespace BT {
Node::Node(const std::string &name) : name(name) {}

Status Node::tick() { throw -1; }

std::string Node::getName() const { return name; }

Status Node::status() const { return state; }

ControlNode::ControlNode(const std::string &name, Node **nodes, int length)
    : length(length), Node(name), counter(0) {
  this->nodes = new Node *[length];
  for (int i = 0; i < length; i++) {
    this->nodes[i] = nodes[i];
  }
}

ControlNode::~ControlNode() { delete[] nodes; }

int ControlNode::getLength() const { return length; }

Sequence::Sequence(const std::string &name, Node **nodes, int length)
    : ControlNode(name, nodes, length) {}

Status Sequence::tick() {
  if (state != Status::RUNNING) {
    counter = 0;
  }

  for (; counter < length; counter++) {
    state = nodes[counter]->tick();
    if (state != Status::SUCCESS) {
      return state;
    }
  }
  return Status::SUCCESS;
}
} // namespace BT
