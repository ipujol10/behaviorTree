#include "control.hpp"

namespace BT {
ControlNode::ControlNode(const std::string &name, Node *children[], int length)
    : length(length), Node(name), counter(0),
      children(copyChildren(children, length)) {}

ControlNode::~ControlNode() { delete[] children; }

Node **ControlNode::copyChildren(Node *children[], int length) {
  Node **out = new Node *[length];
  for (int i = 0; i < length; i++) {
    out[i] = children[i];
  }
  return out;
}

int ControlNode::getLength() const { return length; }

Sequence::Sequence(const std::string &name, Node *children[], int length)
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

Selector::Selector(const std::string &name, Node *children[], int length)
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

Parallel::Parallel(const std::string &name, Node *children[], int length,
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
} // namespace BT
