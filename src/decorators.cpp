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
} // namespace BT
