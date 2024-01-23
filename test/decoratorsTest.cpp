#include "helper.hpp"
#include <BT/decorators.hpp>
#include <gtest/gtest.h>

TEST(InverterTest, Tick) {
  BT::Inverter success("Success",
                       new ControlTestTick("Fail", BT::Status::FAILURE));
  EXPECT_EQ(success.tick(), BT::Status::SUCCESS);

  BT::Inverter running("Running",
                       new ControlTestTick("Runn", BT::Status::RUNNING));
  EXPECT_EQ(running.tick(), BT::Status::RUNNING);

  BT::Inverter failure("Failure",
                       new ControlTestTick("Succ", BT::Status::SUCCESS));
  EXPECT_EQ(failure.tick(), BT::Status::FAILURE);
}

TEST(ForceSuccessTest, Tick) {
  BT::ForceSuccess success0("Success0",
                            new ControlTestTick("Succ", BT::Status::SUCCESS));
  EXPECT_EQ(success0.tick(), BT::Status::SUCCESS);

  BT::ForceSuccess running("Running",
                           new ControlTestTick("Runn", BT::Status::RUNNING));
  EXPECT_EQ(running.tick(), BT::Status::RUNNING);

  BT::ForceSuccess success1("Success1",
                            new ControlTestTick("Fail", BT::Status::FAILURE));
  EXPECT_EQ(success1.tick(), BT::Status::SUCCESS);
}
