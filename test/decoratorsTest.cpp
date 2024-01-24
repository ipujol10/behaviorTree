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
  BT::Repeat success0("Success0", new SeqTestTick("Succ0", succ0, 2), 3);
  EXPECT_EQ(success0.tick(), BT::Status::SUCCESS);

  BT::Status succ1[] = {BT::Status::SUCCESS, BT::Status::SUCCESS,
                        BT::Status::SUCCESS, BT::Status::FAILURE};
  BT::Repeat success1("Success1", new SeqTestTick("Succ1", succ1, 4), 3);
  EXPECT_EQ(success1.tick(), BT::Status::SUCCESS);

  BT::Status succ2[] = {BT::Status::SUCCESS, BT::Status::SUCCESS,
                        BT::Status::RUNNING, BT::Status::SUCCESS,
                        BT::Status::FAILURE};
  BT::Repeat success2("Success2", new SeqTestTick("Succ2", succ2, 5), 3);
  EXPECT_EQ(success2.tick(), BT::Status::RUNNING);
  EXPECT_EQ(success2.tick(), BT::Status::SUCCESS);

  BT::Status fail0[] = {BT::Status::FAILURE, BT::Status::SUCCESS};
  BT::Repeat failure0("Failure0", new SeqTestTick("Fail0", fail0, 2), 3);
  EXPECT_EQ(failure0.tick(), BT::Status::FAILURE);

  BT::Status fail1[] = {BT::Status::SUCCESS, BT::Status::SUCCESS,
                        BT::Status::FAILURE};
  BT::Repeat failure1("Failure1", new SeqTestTick("Fail1", fail1, 3), 3);
  EXPECT_EQ(failure1.tick(), BT::Status::FAILURE);

  BT::Status fail2[] = {BT::Status::SUCCESS, BT::Status::RUNNING,
                        BT::Status::SUCCESS, BT::Status::FAILURE};
  BT::Repeat failure2("Failure2", new SeqTestTick("Fail2", fail2, 4), 3);
  EXPECT_EQ(failure2.tick(), BT::Status::RUNNING);
  EXPECT_EQ(failure2.tick(), BT::Status::FAILURE);
}

TEST(RetryUntilSuccessfulTest, Tick) {
  BT::Status succ0[] = {BT::Status::SUCCESS};
  BT::RetryUntilSuccessful success0("Success0",
                                    new SeqTestTick("Succ0", succ0, 1), 3);
  EXPECT_EQ(success0.tick(), BT::Status::SUCCESS);

  BT::Status succ1[] = {BT::Status::FAILURE, BT::Status::FAILURE,
                        BT::Status::SUCCESS};
  BT::RetryUntilSuccessful success1("Success1",
                                    new SeqTestTick("Succ1", succ1, 3), 3);
  EXPECT_EQ(success1.tick(), BT::Status::SUCCESS);

  BT::Status succ2[] = {BT::Status::FAILURE, BT::Status::RUNNING,
                        BT::Status::FAILURE, BT::Status::SUCCESS};
  BT::RetryUntilSuccessful success2("Success2",
                                    new SeqTestTick("Succ1", succ2, 4), 3);
  EXPECT_EQ(success2.tick(), BT::Status::RUNNING);
  EXPECT_EQ(success2.tick(), BT::Status::SUCCESS);

  BT::Status fail0[] = {BT::Status::FAILURE, BT::Status::FAILURE,
                        BT::Status::FAILURE, BT::Status::SUCCESS};
  BT::RetryUntilSuccessful failure0("Failure0",
                                    new SeqTestTick("Fail0", fail0, 4), 3);
  EXPECT_EQ(failure0.tick(), BT::Status::FAILURE);

  BT::Status fail1[] = {BT::Status::FAILURE};
  BT::RetryUntilSuccessful failure1("Failure1",
                                    new SeqTestTick("Fail1", fail1, 1), 3);
  EXPECT_EQ(failure1.tick(), BT::Status::FAILURE);

  BT::Status fail2[] = {BT::Status::FAILURE, BT::Status::RUNNING,
                        BT::Status::FAILURE, BT::Status::FAILURE,
                        BT::Status::SUCCESS};
  BT::RetryUntilSuccessful failure2("Failure2",
                                    new SeqTestTick("Fail2", fail2, 5), 3);
  EXPECT_EQ(failure2.tick(), BT::Status::RUNNING);
  EXPECT_EQ(failure2.tick(), BT::Status::FAILURE);
}

TEST(KeepRunningUntilFailureTest, Tick) {
  BT::Status runn0[] = {BT::Status::SUCCESS};
  BT::KeepRunningUntilFailure running0("Running0",
                                       new SeqTestTick("Runn0", runn0, 1));
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(running0.tick(), BT::Status::RUNNING);
  }

  BT::Status runn1[] = {BT::Status::SUCCESS, BT::Status::RUNNING};
  BT::KeepRunningUntilFailure running1("Running1",
                                       new SeqTestTick("Runn1", runn1, 2));
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(running0.tick(), BT::Status::RUNNING);
  }

  BT::Status fail0[] = {BT::Status::SUCCESS, BT::Status::RUNNING,
                        BT::Status::FAILURE};
  BT::KeepRunningUntilFailure failure0("Failure0",
                                       new SeqTestTick("Fail0", fail0, 3));
  EXPECT_EQ(failure0.tick(), BT::Status::RUNNING);
  EXPECT_EQ(failure0.tick(), BT::Status::RUNNING);
  EXPECT_EQ(failure0.tick(), BT::Status::FAILURE);

  BT::KeepRunningUntilFailure failure1(
      "Failure1", new ControlTestTick("Fail1", BT::Status::FAILURE));
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(failure1.tick(), BT::Status::FAILURE);
  }
}
