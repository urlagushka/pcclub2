#include <gtest/gtest.h>
#include <pcclub/club.hpp>
#include <pcclub/events.hpp>

TEST(events_test, event1_success)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  auto result = pc::event_call(club, {{9, 30}, 1, "client1"});
  
  EXPECT_EQ(result.next_event, 0);
  EXPECT_EQ(result.str_data, "client1");
}

TEST(events_test, event1_not_opened_yet)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  auto result = pc::event_call(club, {{8, 30}, 1, "client1"});
  
  EXPECT_EQ(result.next_event, 13);
  EXPECT_EQ(result.str_data, "NotOpenYet");
}

TEST(events_test, event1_client_already_exists)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  pc::event_call(club, {{10, 30}, 1, "client1"});
  auto result = pc::event_call(club, {{11, 30}, 1, "client1"});

  EXPECT_EQ(result.next_event, 13);
  EXPECT_EQ(result.str_data, "YouShallNotPass");
}

TEST(events_test, event2_success)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  pc::event_call(club, {{10, 30}, 1, "client1"});
  auto result = pc::event_call(club, {{11, 30}, 2, "client1", 1});
  
  EXPECT_EQ(result.next_event, 0);
  EXPECT_EQ(result.str_data, "client1");
}

TEST(events_test, event2_client_unknown)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  auto result = pc::event_call(club, {{10, 30}, 2, "unknown", 1});

  EXPECT_EQ(result.next_event, 13);
  EXPECT_EQ(result.str_data, "ClientUnknown");
}

TEST(events_test, event2_place_is_busy)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  pc::event_call(club, {{10, 30}, 1, "client1"});
  pc::event_call(club, {{11, 30}, 1, "client2"});
  pc::event_call(club, {{12, 30}, 2, "client1", 1});
  auto result = pc::event_call(club, {{13, 30}, 2, "client2", 1});
  
  EXPECT_EQ(result.next_event, 13);
  EXPECT_EQ(result.str_data, "PlaceIsBusy");
}

TEST(events_test, event3_wait)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  pc::event_call(club, {{10, 30}, 1, "client1"});
  pc::event_call(club, {{11, 30}, 1, "client2"});
  pc::event_call(club, {{12, 30}, 1, "client3"});
  pc::event_call(club, {{13, 30}, 2, "client1", 1});
  pc::event_call(club, {{14, 30}, 2, "client2", 2});
  pc::event_call(club, {{15, 30}, 2, "client3", 3});
  pc::event_call(club, {{16, 30}, 1, "client4"});
  auto result = pc::event_call(club, {{17, 30}, 3, "client4"});
  
  EXPECT_EQ(result.next_event, 0);
  EXPECT_EQ(club.client_size(), 1);
}

TEST(events_test, event3_no_wait)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  pc::event_call(club, {{10, 30}, 1, "client1"});
  pc::event_call(club, {{11, 30}, 1, "client2"});
  pc::event_call(club, {{13, 30}, 2, "client1", 1});
  pc::event_call(club, {{14, 30}, 2, "client2", 2});
  pc::event_call(club, {{16, 30}, 1, "client4"});
  auto result = pc::event_call(club, {{17, 30}, 3, "client4"});
    
  EXPECT_EQ(result.next_event, 13);
  EXPECT_EQ(result.str_data, "ICanWaitNoLonger!");
}

TEST(events_test, event4)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  pc::event_call(club, {{10, 30}, 1, "client1"});
  pc::event_call(club, {{12, 30}, 2, "client1", 1});
  pc::event_call(club, {{13, 30}, 1, "client2"});
  auto result = pc::event_call(club, {{14, 30}, 4, "client1"});
  
  EXPECT_EQ(result.next_event, 12);
  EXPECT_EQ(result.str_data, "client1");
}

TEST(events_test, event11)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  pc::event_call(club, {{10, 30}, 1, "client1"});
  auto result = pc::event_call(club, {{11, 30}, 11, "client1"});
  
  EXPECT_EQ(result.next_event, 0);
  EXPECT_EQ(result.str_data, "client1");
}

TEST(events_test, event12)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  pc::event_call(club, {{10, 30}, 1, "client1"});
  auto result = pc::event_call(club, {{11, 30}, 12});

  EXPECT_EQ(result.next_event, 0);
  EXPECT_EQ(result.str_data, "client1");
  EXPECT_EQ(result.sub_data, 1);
}

TEST(events_test, event13)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  auto result = pc::event_call(club, {{9, 30}, 13, "error"});
  
  EXPECT_EQ(result.next_event, 0);
  EXPECT_EQ(result.str_data, "error");
}
