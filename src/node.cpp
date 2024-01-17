#include "node.hpp"

namespace BT {
Node::Node(const std::string &name) : name(name) {}

std::ostream &operator<<(std::ostream &out, const Status st) {
  const std::string names[] = {"SUCCESS", "RUNNING", "FAILURE"};
  return out << names[st];
}

Status Node::tick() { throw -1; }

std::string Node::getName() const { return name; }

Status Node::status() const { return state; }
} // namespace BT
