#pragma once

#include "node.hpp"

namespace BT {
class Decorator : public Node {
protected:
  Node *child;

public:
  Decorator(const std::string &name, Node *child);
};

class Inverter final : public Decorator {
public:
  Inverter(const std::string &name, Node *child);
  Status tick() override final;
};
} // namespace BT
