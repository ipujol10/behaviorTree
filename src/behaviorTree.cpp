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

Selector::Selector(const std::string &name, Node **nodes, int length)
    : ControlNode(name, nodes, length) {}

Status Selector::tick() {
  for (counter = 0; counter < length; counter++) {
    state = nodes[counter]->tick();
    if (state != Status::FAILURE) {
      return state;
    }
  }
  return Status::FAILURE;
}

Parallel::Parallel(const std::string &name, Node **nodes, int length,
                   int threshold)
    : threshold(threshold), ControlNode(name, nodes, length) {}

Status Parallel::tick() {
  int success = 0, failure = 0;
  for (counter = 0; counter < length; counter++) {
    switch (nodes[counter]->tick()) {
    case Status::SUCCESS:
      success++;
      break;
    case Status::FAILURE:
      failure++;
      break;
    case Status::RUNNING:
      break;
    }
  }

  if (success >= threshold) {
    return Status::SUCCESS;
  }
  if (failure > (length - threshold)) {
    return Status::FAILURE;
  }
  return Status::RUNNING;
}
} // namespace BT
