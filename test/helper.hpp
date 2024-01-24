#include <BT/node.hpp>

class ControlTestTick : public BT::Node {
public:
  ControlTestTick(const std::string &name, BT::Status status) : Node(name) {
    state = status;
  }

  BT::Status tick() override { return state; }
};

class SeqTestTick : public BT::Node {
private:
  BT::Status succession[50];
  int i;
  const int length;

public:
  SeqTestTick(const std::string &name, BT::Status succession[], int N)
      : length(N), i(N), Node(name) {
    for (int k = 0; k < N; k++) {
      this->succession[k] = succession[k];
    }
  }

  BT::Status tick() override {
    i++;
    if (i >= length) {
      i = 0;
    }
    return succession[i];
  }
};
