#pragma once

#include <iostream>
#include <string>

namespace BT {
enum Status {
  SUCCESS,
  RUNNING,
  FAILURE,
};
std::ostream &operator<<(std::ostream &out, const Status st);

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
} // namespace BT
