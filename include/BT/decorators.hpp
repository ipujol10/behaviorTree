#pragma once

#include "node.hpp"

namespace BT {
class Decorator : public Node {
protected:
  Node *const child;

public:
  Decorator(const std::string &name, Node *child);
};

class Inverter final : public Decorator {
public:
  Inverter(const std::string &name, Node *child);
  Status tick() override;
};

class ForceSuccess final : public Decorator {
public:
  ForceSuccess(const std::string &name, Node *child);
  Status tick() override;
};

class ForceFailure final : public Decorator {
public:
  ForceFailure(const std::string &name, Node *child);
  Status tick() override;
};

class Repeat final : public Decorator {
private:
  const int N;
  int i;
  bool blocked;

public:
  Repeat(const std::string &name, Node *child, int N);
  Status tick() override;
};
} // namespace BT
