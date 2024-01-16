#include <BT/node.hpp>
#include <gtest/gtest.h>

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
