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
} // namespace BT
