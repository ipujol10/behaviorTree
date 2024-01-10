#pragma once

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
};
} // namespace BT
