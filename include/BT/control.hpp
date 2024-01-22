#pragma once

#include "node.hpp"
#include <gtest/gtest.h>

namespace BT {
class ControlNode : public Node {
private:
  static Node **copyChildren(Node *children[], int length);

protected:
  int counter;
  Node *const *children;
  const int length;

public:
  ControlNode(const std::string &name, Node *children[], int length);
  ~ControlNode();
  int getLength() const;

  FRIEND_TEST(ControlNodeTest, Constructor);
};

class Sequence : public ControlNode {
public:
  Sequence(const std::string &name, Node *children[], int length);
  Status tick() override final;

  FRIEND_TEST(SequenceTest, Tick);
};

class Selector : public ControlNode {
public:
  Selector(const std::string &name, Node *children[], int length);
  Status tick() override final;

  FRIEND_TEST(SelectorTest, Tick);
};

class Parallel : public ControlNode {
private:
  const int threshold;

public:
  Parallel(const std::string &name, Node *children[], int length,
           int threshold);
  Status tick() override final;
};
} // namespace BT
