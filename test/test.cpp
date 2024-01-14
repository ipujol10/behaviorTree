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
TEST(ControlNodeTest, Constructor) {
  int N = 5;
  Node **nodes = new Node *[N];
  for (int i = 0; i < N; i++) {
    std::stringstream nodeName;
    nodeName << "Node " << i;
    nodes[i] = new Node(nodeName.str());
  }
  ControlNode control("Control Node", nodes, N);

  EXPECT_EQ(control.counter, 0);
  EXPECT_EQ(control.getName(), "Control Node");
  EXPECT_EQ(control.getLength(), 5);
  for (int i = 0; i < control.getLength(); i++) {
    std::stringstream nodeName;
    nodeName << "Node " << i;
    EXPECT_EQ(control.children[i]->getName(), nodeName.str());
  }
  delete[] nodes;
}

class ControlTestTick : public Node {
public:
  ControlTestTick(const std::string &name, Status status) : Node(name) {
    state = status;
  }

  Status tick() override { return state; }
};

TEST(SequenceTest, Tick) {
  Node *successNodes[] = {new ControlTestTick("Succ 0", Status::SUCCESS),
                          new ControlTestTick("Succ 1", Status::SUCCESS),
                          new ControlTestTick("Succ 2", Status::SUCCESS)};
  Sequence success("Success", successNodes, 3);

  Node *runningNodes[] = {new ControlTestTick("Run 0", Status::SUCCESS),
                          new ControlTestTick("Run 1", Status::RUNNING),
                          new ControlTestTick("Run 2", Status::SUCCESS)};
  Sequence running("Running", runningNodes, 3);

  Node *failureNodes[] = {new ControlTestTick("Fail 0", Status::SUCCESS),
                          new ControlTestTick("Fail 1", Status::SUCCESS),
                          new ControlTestTick("Fail 2", Status::FAILURE)};
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
}

TEST(SelectorTest, Tick) {
  Node *successNodes0[] = {new ControlTestTick("Succ 0", Status::SUCCESS),
                           new ControlTestTick("Succ 1", Status::SUCCESS)};
  Selector success0("Success 0", successNodes0, 2);

  Node *successNodes1[] = {new ControlTestTick("Fail 0", Status::FAILURE),
                           new ControlTestTick("Succ 0", Status::SUCCESS)};
  Selector success1("Success 1", successNodes1, 2);

  Node *runningNodes[] = {new ControlTestTick("Fail 0", Status::FAILURE),
                          new ControlTestTick("Run 0", Status::RUNNING),
                          new ControlTestTick("Succ 0", Status::SUCCESS)};
  Selector running("Running", runningNodes, 3);

  Node *failureNodes[] = {new ControlTestTick("Fail 0", Status::FAILURE),
                          new ControlTestTick("Fail 1", Status::FAILURE),
                          new ControlTestTick("Fail 2", Status::FAILURE)};
  Selector failure("Failure", failureNodes, 3);

  EXPECT_EQ(success0.tick(), Status::SUCCESS);
  EXPECT_EQ(success0.counter, 0);
  EXPECT_EQ(success0.tick(), Status::SUCCESS);
  EXPECT_EQ(success0.counter, 0);

  EXPECT_EQ(success1.tick(), Status::SUCCESS);
  EXPECT_EQ(success1.counter, 1);
  EXPECT_EQ(success1.tick(), Status::SUCCESS);
  EXPECT_EQ(success1.counter, 1);

  EXPECT_EQ(running.tick(), Status::RUNNING);
  EXPECT_EQ(running.counter, 1);
  EXPECT_EQ(running.tick(), Status::RUNNING);
  EXPECT_EQ(running.counter, 1);

  EXPECT_EQ(failure.tick(), Status::FAILURE);
  EXPECT_EQ(failure.counter, 3);
  EXPECT_EQ(failure.tick(), Status::FAILURE);
  EXPECT_EQ(failure.counter, 3);
}

TEST(ParallelTest, Tick) {
  Node *successNodes0[] = {new ControlTestTick("Succ 0", Status::SUCCESS),
                           new ControlTestTick("Succ 1", Status::SUCCESS),
                           new ControlTestTick("Succ 2", Status::SUCCESS),
                           new ControlTestTick("Succ 3", Status::SUCCESS),
                           new ControlTestTick("Succ 4", Status::SUCCESS)};
  Parallel success0("Success 0", successNodes0, 5, 3);

  Node *successNodes1[] = {new ControlTestTick("Succ 0", Status::SUCCESS),
                           new ControlTestTick("Succ 1", Status::SUCCESS),
                           new ControlTestTick("Succ 2", Status::SUCCESS),
                           new ControlTestTick("Fail 0", Status::FAILURE),
                           new ControlTestTick("Runn 0", Status::RUNNING)};
  Parallel success1("Success 1", successNodes1, 5, 3);

  Node *runningNodes[] = {new ControlTestTick("Succ 0", Status::SUCCESS),
                          new ControlTestTick("Succ 1", Status::SUCCESS),
                          new ControlTestTick("Fail 0", Status::FAILURE),
                          new ControlTestTick("Fail 1", Status::FAILURE),
                          new ControlTestTick("Runn 0", Status::RUNNING)};
  Parallel running("Running", runningNodes, 5, 3);

  Node *failureNodes[] = {new ControlTestTick("Succ 0", Status::SUCCESS),
                          new ControlTestTick("Fail 0", Status::FAILURE),
                          new ControlTestTick("Fail 1", Status::FAILURE),
                          new ControlTestTick("Fail 2", Status::FAILURE),
                          new ControlTestTick("Runn 0", Status::RUNNING)};
  Parallel failure("Failure", failureNodes, 5, 3);

  EXPECT_EQ(success0.tick(), Status::SUCCESS);
  EXPECT_EQ(success0.tick(), Status::SUCCESS);

  EXPECT_EQ(success1.tick(), Status::SUCCESS);
  EXPECT_EQ(success1.tick(), Status::SUCCESS);

  EXPECT_EQ(running.tick(), Status::RUNNING);
  EXPECT_EQ(running.tick(), Status::RUNNING);

  EXPECT_EQ(failure.tick(), Status::FAILURE);
}

TEST(InverterTest, Tick) {
  Inverter success("Success", new ControlTestTick("Fail", Status::FAILURE));
  EXPECT_EQ(success.tick(), Status::SUCCESS);

  Inverter running("Running", new ControlTestTick("Runn", Status::RUNNING));
  EXPECT_EQ(running.tick(), Status::RUNNING);

  Inverter failure("Failure", new ControlTestTick("Succ", Status::SUCCESS));
  EXPECT_EQ(failure.tick(), Status::FAILURE);
}
} // namespace BT
