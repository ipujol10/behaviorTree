#include "helper.hpp"
#include <BT/control.hpp>
#include <gtest/gtest.h>

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
} // namespace BT

TEST(ParallelTest, Tick) {
  BT::Node *successNodes0[] = {
      new ControlTestTick("Succ 0", BT::Status::SUCCESS),
      new ControlTestTick("Succ 1", BT::Status::SUCCESS),
      new ControlTestTick("Succ 2", BT::Status::SUCCESS),
      new ControlTestTick("Succ 3", BT::Status::SUCCESS),
      new ControlTestTick("Succ 4", BT::Status::SUCCESS)};
  BT::Parallel success0("Success 0", successNodes0, 5, 3);

  BT::Node *successNodes1[] = {
      new ControlTestTick("Succ 0", BT::Status::SUCCESS),
      new ControlTestTick("Succ 1", BT::Status::SUCCESS),
      new ControlTestTick("Succ 2", BT::Status::SUCCESS),
      new ControlTestTick("Fail 0", BT::Status::FAILURE),
      new ControlTestTick("Runn 0", BT::Status::RUNNING)};
  BT::Parallel success1("Success 1", successNodes1, 5, 3);

  BT::Node *runningNodes[] = {
      new ControlTestTick("Succ 0", BT::Status::SUCCESS),
      new ControlTestTick("Succ 1", BT::Status::SUCCESS),
      new ControlTestTick("Fail 0", BT::Status::FAILURE),
      new ControlTestTick("Fail 1", BT::Status::FAILURE),
      new ControlTestTick("Runn 0", BT::Status::RUNNING)};
  BT::Parallel running("Running", runningNodes, 5, 3);

  BT::Node *failureNodes[] = {
      new ControlTestTick("Succ 0", BT::Status::SUCCESS),
      new ControlTestTick("Fail 0", BT::Status::FAILURE),
      new ControlTestTick("Fail 1", BT::Status::FAILURE),
      new ControlTestTick("Fail 2", BT::Status::FAILURE),
      new ControlTestTick("Runn 0", BT::Status::RUNNING)};
  BT::Parallel failure("Failure", failureNodes, 5, 3);

  EXPECT_EQ(success0.tick(), BT::Status::SUCCESS);
  EXPECT_EQ(success0.tick(), BT::Status::SUCCESS);

  EXPECT_EQ(success1.tick(), BT::Status::SUCCESS);
  EXPECT_EQ(success1.tick(), BT::Status::SUCCESS);

  EXPECT_EQ(running.tick(), BT::Status::RUNNING);
  EXPECT_EQ(running.tick(), BT::Status::RUNNING);

  EXPECT_EQ(failure.tick(), BT::Status::FAILURE);
  EXPECT_EQ(failure.tick(), BT::Status::FAILURE);
}
