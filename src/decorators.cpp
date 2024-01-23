#include "decorators.hpp"

namespace BT {
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

ForceSuccess::ForceSuccess(const std::string &name, Node *child)
    : Decorator(name, child) {}

Status ForceSuccess::tick() {
  if (child->tick() == Status::RUNNING) {
    return Status::RUNNING;
  }
  return Status::SUCCESS;
}

ForceFailure::ForceFailure(const std::string &name, Node *child)
    : Decorator(name, child) {}

Status ForceFailure::tick() {
  if (child->tick() == Status::RUNNING) {
    return Status::RUNNING;
  }
  return Status::FAILURE;
}

Repeat::Repeat(const std::string &name, Node *child, int N)
    : N(N), i(0), blocked(false), Decorator(name, child) {}

Status Repeat::tick() {
  if (blocked) {
    return state;
  }

  state = child->tick();
  if (state == Status::RUNNING) {
    return state;
  }

  i++;
  if (i >= N || state == Status::FAILURE) {
    blocked = true;
  }
  return state;
}
} // namespace BT
