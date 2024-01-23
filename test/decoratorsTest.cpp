#include "BT/node.hpp"
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

TEST(ForceFailureTest, Tick) {
  BT::ForceFailure failure0("Failure0",
                            new ControlTestTick("Succ", BT::Status::SUCCESS));
  EXPECT_EQ(failure0.tick(), BT::Status::FAILURE);

  BT::ForceFailure running("Running",
                           new ControlTestTick("Runn", BT::Status::RUNNING));
  EXPECT_EQ(running.tick(), BT::Status::RUNNING);

  BT::ForceFailure failure1("failure1",
                            new ControlTestTick("Fail", BT::Status::FAILURE));
  EXPECT_EQ(failure1.tick(), BT::Status::FAILURE);
}

TEST(RepeatTest, Tick) {
  BT::Status succ0[] = {BT::Status::SUCCESS, BT::Status::SUCCESS};
  BT::Repeat success0("Success0", new SuccessionTestTick("Succ0", succ0, 2), 3);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(success0.tick(), BT::Status::SUCCESS);
  }

  BT::Status succ1[] = {BT::Status::SUCCESS, BT::Status::SUCCESS,
                        BT::Status::SUCCESS, BT::Status::FAILURE};
  BT::Repeat success1("Success1", new SuccessionTestTick("Succ1", succ1, 4), 3);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(success1.tick(), BT::Status::SUCCESS);
  }

  BT::Status succ2[] = {BT::Status::SUCCESS, BT::Status::SUCCESS,
                        BT::Status::RUNNING, BT::Status::SUCCESS,
                        BT::Status::FAILURE};
  BT::Repeat success2("Success2", new SuccessionTestTick("Succ2", succ2, 5), 3);
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(success2.tick(), BT::Status::SUCCESS);
  }
  EXPECT_EQ(success2.tick(), BT::Status::RUNNING);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(success2.tick(), BT::Status::SUCCESS);
  }

  BT::Status fail0[] = {BT::Status::FAILURE, BT::Status::SUCCESS};
  BT::Repeat failure0("Failure0", new SuccessionTestTick("Fail0", fail0, 2), 3);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(failure0.tick(), BT::Status::FAILURE);
  }

  BT::Status fail1[] = {BT::Status::SUCCESS, BT::Status::SUCCESS,
                        BT::Status::FAILURE};
  BT::Repeat failure1("Failure1", new SuccessionTestTick("Fail1", fail1, 3), 3);
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(failure1.tick(), BT::Status::SUCCESS);
  }
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(failure1.tick(), BT::Status::FAILURE);
  }

  BT::Status fail2[] = {BT::Status::SUCCESS, BT::Status::RUNNING,
                        BT::Status::SUCCESS, BT::Status::FAILURE};
  BT::Repeat failure2("Failure2", new SuccessionTestTick("Fail2", fail2, 4), 3);
  EXPECT_EQ(failure2.tick(), BT::Status::SUCCESS);
  EXPECT_EQ(failure2.tick(), BT::Status::RUNNING);
  EXPECT_EQ(failure2.tick(), BT::Status::SUCCESS);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(failure2.tick(), BT::Status::FAILURE);
  }
}
