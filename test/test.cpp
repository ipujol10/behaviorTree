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
  const Node **nodes = new const Node*[N];
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
} // namespace BT
