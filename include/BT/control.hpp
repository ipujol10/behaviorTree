#pragma once

#include "node.hpp"
#include <gtest/gtest.h>

namespace BT {
class ControlNode : public Node {
protected:
  int counter;
  Node **children;
  const int length;

public:
  ControlNode(const std::string &name, Node **children, int length);
  ~ControlNode();
  int getLength() const;

  FRIEND_TEST(ControlNodeTest, Constructor);
};

class Sequence final : public ControlNode {
public:
  Sequence(const std::string &name, Node **children, int length);
  Status tick() override;

  FRIEND_TEST(SequenceTest, Tick);
};

class Selector final : public ControlNode {
public:
  Selector(const std::string &name, Node **children, int length);
  Status tick() override;

  FRIEND_TEST(SelectorTest, Tick);
};

class Parallel final : public ControlNode {
private:
  const int threshold;

public:
  Parallel(const std::string &name, Node **children, int length, int threshold);
  Status tick() override;
};
} // namespace BT
