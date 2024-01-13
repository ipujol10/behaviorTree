#include <BT/behaviorTree.hpp>
#include <gtest/gtest.h>
#include <sstream>

TEST(NodeTest, Constructor) {
  BT::Node node("Constructor");
  EXPECT_EQ(node.getName(), "Constructor");
}

TEST(NodeTest, Tick) {
  BT::Node node("Tick");
  EXPECT_THROW(
      {
        try {
          node.tick();
        } catch (int e) {
          EXPECT_EQ(e, -1);
          throw;
        }
      },
      int);
}

namespace BT {
TEST(SequenceTest, Constructor) {
  int N = 5;
  Node **nodes = new Node *[N];
  for (int i = 0; i < N; i++) {
    std::stringstream nodeName;
    nodeName << "Node " << i;
    nodes[i] = new Node(nodeName.str());
  }
  Sequence s("Sequence", nodes, N);

  EXPECT_EQ(s.counter, 0);
  EXPECT_EQ(s.getName(), "Sequence");
  EXPECT_EQ(s.getLength(), 5);
  for (int i = 0; i < s.getLength(); i++) {
    std::stringstream nodeName;
    nodeName << "Node " << i;
    EXPECT_EQ(s.nodes[i]->getName(), nodeName.str());
  }
  delete[] nodes;
}

class SeqTestTick : public Node {
public:
  SeqTestTick(const std::string &name, Status status) : Node(name) {
    state = status;
  }

  Status tick() override { return state; }
};

TEST(SequenceTest, Tick) {
  Node **successNodes = new Node *[3];
  successNodes[0] = new SeqTestTick("Succ 0", Status::SUCCESS);
  successNodes[1] = new SeqTestTick("Succ 1", Status::SUCCESS);
  successNodes[2] = new SeqTestTick("Succ 2", Status::SUCCESS);
  Sequence success("Success", successNodes, 3);

  Node **runningNodes = new Node *[3];
  runningNodes[0] = new SeqTestTick("Run 0", Status::SUCCESS);
  runningNodes[1] = new SeqTestTick("Run 1", Status::RUNNING);
  runningNodes[2] = new SeqTestTick("Run 2", Status::SUCCESS);
  Sequence running("Running", runningNodes, 3);

  Node **failureNodes = new Node *[3];
  failureNodes[0] = new SeqTestTick("Fail 0", Status::SUCCESS);
  failureNodes[1] = new SeqTestTick("Fail 1", Status::SUCCESS);
  failureNodes[2] = new SeqTestTick("Fail 2", Status::FAILURE);
  Sequence failure("Failure", failureNodes, 3);

  EXPECT_EQ(success.tick(), Status::SUCCESS);
  EXPECT_EQ(success.counter, 3);
  EXPECT_EQ(success.tick(), Status::SUCCESS);
  EXPECT_EQ(success.counter, 3);
  EXPECT_EQ(success.status(), Status::SUCCESS);

  EXPECT_EQ(running.tick(), Status::RUNNING);
  EXPECT_EQ(running.counter, 1);
  EXPECT_EQ(running.tick(), Status::RUNNING);
  EXPECT_EQ(running.counter, 1);
  EXPECT_EQ(running.status(), Status::RUNNING);

  EXPECT_EQ(failure.tick(), Status::FAILURE);
  EXPECT_EQ(failure.counter, 2);
  EXPECT_EQ(failure.tick(), Status::FAILURE);
  EXPECT_EQ(failure.counter, 2);
  EXPECT_EQ(failure.status(), Status::FAILURE);

  delete[] successNodes;
  delete[] runningNodes;
  delete[] failureNodes;
}
} // namespace BT
