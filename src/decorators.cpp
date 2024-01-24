#include "decorators.hpp"
#include "node.hpp"

namespace BT {
Decorator::Decorator(const std::string &name, Node *child)
    : Node(name), child(child) {}

Inverter::Inverter(const std::string &name, Node *child)
    : Decorator(name, child) {}

Status Inverter::tick() {
  switch (child->tick()) {
  case Status::RUNNING:
    return Status::RUNNING;
  case Status::FAILURE:
    return Status::SUCCESS;
  case Status::SUCCESS:
    return Status::FAILURE;
  }
  throw -10;
}

ForceSuccess::ForceSuccess(const std::string &name, Node *child)
    : Decorator(name, child) {}

Status ForceSuccess::tick() {
  if (child->tick() == Status::RUNNING) {
    return Status::RUNNING;
  }
  return Status::SUCCESS;
}

ForceFailure::ForceFailure(const std::string &name, Node *child)
    : Decorator(name, child) {}

Status ForceFailure::tick() {
  if (child->tick() == Status::RUNNING) {
    return Status::RUNNING;
  }
  return Status::FAILURE;
}

Repeat::Repeat(const std::string &name, Node *child, int N)
    : N(N), i(0), Decorator(name, child) {}

Status Repeat::tick() {
  for (; i < N; i++) {
    state = child->tick();
    if (state == Status::RUNNING) {
      return Status::RUNNING;
    }
    if (state == Status::FAILURE) {
      i = 0;
      return Status::FAILURE;
    }
  }
  i = 0;
  return Status::SUCCESS;
}

RetryUntilSuccessful::RetryUntilSuccessful(const std::string &name, Node *child,
                                           int N)
    : N(N), i(0), Decorator(name, child) {}

Status RetryUntilSuccessful::tick() {
  for (; i < N; i++) {
    state = child->tick();
    if (state == Status::RUNNING) {
      return Status::RUNNING;
    }
    if (state == Status::SUCCESS) {
      i = 0;
      return Status::SUCCESS;
    }
  }
  i = 0;
  return Status::FAILURE;
}

KeepRunningUntilFailure::KeepRunningUntilFailure(const std::string &name,
                                                 Node *child)
    : Decorator(name, child) {}

Status KeepRunningUntilFailure::tick() {
  if (child->tick() == Status::FAILURE) {
    return Status::FAILURE;
  }
  return Status::RUNNING;
}

RunOnce::RunOnce(const std::string &name, Node *child)
    : blocked(false), Decorator(name, child) {}

Status RunOnce::tick() {
  if (blocked) {
    return state;
  }
  state = child->tick();
  if (state != Status::RUNNING) {
    blocked = true;
  }
  return state;
}
} // namespace BT
