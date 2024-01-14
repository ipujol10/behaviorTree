#include "behaviorTree.hpp"

namespace BT {
Node::Node(const std::string &name) : name(name) {}

Status Node::tick() { throw -1; }

std::string Node::getName() const { return name; }

Status Node::status() const { return state; }

ControlNode::ControlNode(const std::string &name, Node **children, int length)
    : length(length), Node(name), counter(0) {
  this->children = new Node *[length];
  for (int i = 0; i < length; i++) {
    this->children[i] = children[i];
  }
}

ControlNode::~ControlNode() { delete[] children; }

int ControlNode::getLength() const { return length; }

Sequence::Sequence(const std::string &name, Node **children, int length)
    : ControlNode(name, children, length) {}

Status Sequence::tick() {
  if (state != Status::RUNNING) {
    counter = 0;
  }

  for (; counter < length; counter++) {
    state = children[counter]->tick();
    if (state != Status::SUCCESS) {
      return state;
    }
  }
  return Status::SUCCESS;
}

Selector::Selector(const std::string &name, Node **children, int length)
    : ControlNode(name, children, length) {}

Status Selector::tick() {
  for (counter = 0; counter < length; counter++) {
    state = children[counter]->tick();
    if (state != Status::FAILURE) {
      return state;
    }
  }
  return Status::FAILURE;
}

Parallel::Parallel(const std::string &name, Node **children, int length,
                   int threshold)
    : threshold(threshold), ControlNode(name, children, length) {}

Status Parallel::tick() {
  int success = 0, failure = 0;
  for (counter = 0; counter < length; counter++) {
    switch (children[counter]->tick()) {
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

Decorator::Decorator(const std::string &name, Node *child)
    : Node(name), child(child) {}

Inverter::Inverter(const std::string &name, Node *child)
    : Decorator(name, child) {}

Status Inverter::tick() {
  switch (child->tick()) {
  case Status::RUNNING:
    return Status::RUNNING;
  case Status::FAILURE:
    return Status::SUCCESS;
  case Status::SUCCESS:
    return Status::FAILURE;
  }
  throw -10;
}
} // namespace BT
