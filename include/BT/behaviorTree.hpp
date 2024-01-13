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

/***************
 * Control Nodes
 ***************/
class Sequence final : public Node {
private:
  int counter;
  Node **nodes;
  const int length;

public:
  Sequence(const std::string &name, Node **nodes, int length);
  ~Sequence();
  int getLength() const;
  Status tick() override final;

  FRIEND_TEST(SequenceTest, Constructor);
  FRIEND_TEST(SequenceTest, Tick);
};
} // namespace BT
