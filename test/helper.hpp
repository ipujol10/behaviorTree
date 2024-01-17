#include <BT/node.hpp>

class ControlTestTick : public BT::Node {
public:
  ControlTestTick(const std::string &name, BT::Status status) : Node(name) {
    state = status;
  }

  BT::Status tick() override { return state; }
};
