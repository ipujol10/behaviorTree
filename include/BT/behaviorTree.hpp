#pragma once

#include <gtest/gtest.h>
#include <string>

namespace BT {
enum Status {
  SUCCESS,
  FAILURE,
  RUNNING,
};

class Node {
protected:
  Status state;
  const std::string name;

public:
  Node(const std::string &name);
  virtual Status tick();
  std::string getName() const;
  Status status() const;
};

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
  Status tick() override final;

  FRIEND_TEST(SequenceTest, Tick);
};

class Selector final : public ControlNode {
public:
  Selector(const std::string &name, Node **children, int length);
  Status tick() override final;

  FRIEND_TEST(SelectorTest, Tick);
};

class Parallel final : public ControlNode {
private:
  const int threshold;

public:
  Parallel(const std::string &name, Node **children, int length, int threshold);
  Status tick() override final;
};

class Decorator: public Node {
protected:
  Node *child;

public:
  Decorator(const std::string &name, Node *child);
};

class Inverter final: public Decorator {
public:
  Inverter(const std::string &name, Node *child);
  Status tick() override final;
};
} // namespace BT
