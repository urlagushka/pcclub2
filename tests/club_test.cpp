#include <gtest/gtest.h>
#include <pcclub/club.hpp>
#include <pcclub/club_structs.hpp>
#include <pcclub/time.hpp>

TEST(club_test, init)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);

  EXPECT_EQ(club.table_size(), 3);
  EXPECT_EQ(club.client_size(), 0);
  EXPECT_TRUE(club.are_free_table());
}

TEST(club_test, add_client_success)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);
  pc::client client{"client1", {pc::time_stamp{9, 30}, std::nullopt}};

  club.add_client(client);
  
  EXPECT_EQ(club.client_size(), 1);
  EXPECT_TRUE(club.is_client_inside(client));
}


TEST(club_test, add_client_not_opened_yet)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);
  pc::client client{"client1", {pc::time_stamp{8, 30}, std::nullopt}};

  EXPECT_THROW(club.add_client(client), std::runtime_error);
}

TEST(club_test, add_client_already_inside)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);
  pc::client client{"client1", {pc::time_stamp{9, 30}, std::nullopt}};

  club.add_client(client);
  EXPECT_THROW(club.add_client(client), std::runtime_error);
}

TEST(club_test, take_table_success)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);
  pc::client client{"client1", {pc::time_stamp{9, 30}, std::nullopt}};

  club.add_client(client);
  club.take_table(client, 1);
  
  EXPECT_EQ(club.client_size(), 0);
  EXPECT_TRUE(club.is_client_inside(client));
  EXPECT_TRUE(club.are_free_table());
}

TEST(club_test, take_table_out_of_range)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);
  pc::client client{"client1", {pc::time_stamp{9, 30}, std::nullopt}};

  club.add_client(client);
  EXPECT_THROW(club.take_table(client, 4), std::out_of_range);
}

TEST(club_test, take_table_place_is_busy)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);
  pc::client client1{"client1", {pc::time_stamp{9, 30}, std::nullopt}};
  pc::client client2{"client2", {pc::time_stamp{10, 30}, std::nullopt}};

  club.add_client(client1);
  club.add_client(client2);
  club.take_table(client1, 1);
  EXPECT_THROW(club.take_table(client2, 1), std::runtime_error);
}

TEST(club_test, free_table_with_profit)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);
  pc::client client{"client1", {pc::time_stamp{9, 30}, std::nullopt}};

  club.add_client(client);
  club.take_table(client, 1);
  pc::client leaving_client = client;
  leaving_client.time.out = {10, 30};
  club.free_table(leaving_client);
  
  EXPECT_EQ(club.client_size(), 1);
  auto profits = club.lock_in_profits();
  EXPECT_EQ(std::get<1>(profits[0]), 10);
}

TEST(club_test, satisfy_queue)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);
  pc::client client1{"client1", {pc::time_stamp{9, 30}, std::nullopt}};
  pc::client client2{"client2", {pc::time_stamp{10, 30}, std::nullopt}};
  pc::client client3{"client3", {pc::time_stamp{11, 30}, std::nullopt}};
  pc::client client4{"client4", {pc::time_stamp{12, 30}, std::nullopt}};

  club.add_client(client1);
  club.add_client(client2);
  club.add_client(client3);

  club.take_table(client1, 1);
  club.take_table(client2, 2);
  club.take_table(client3, 3);
  
  club.add_client(client4);
  
  client1.time.out = {13, 0};
  club.free_table(client1);

  auto result = club.satisfy_queue({14, 0});
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->first, "client4");
  EXPECT_EQ(result->second, 1);
}

TEST(club_test, pop_client)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);
  pc::client client{"client1", {pc::time_stamp{9, 30}, std::nullopt}};

  club.add_client(client);
  club.take_table(client, 1);
  
  client.time.out = {10, 0};
  club.pop_client(client);
  
  EXPECT_FALSE(club.is_client_inside(client));
  EXPECT_TRUE(club.are_free_table());
}

TEST(club_test, is_open)
{
  pc::club club({9, 0}, {19, 0}, 10, 3);
  EXPECT_FALSE(club.is_open({8, 0}));
  EXPECT_TRUE(club.is_open({9, 1}));
  EXPECT_TRUE(club.is_open({18, 59}));
  EXPECT_FALSE(club.is_open({19, 0}));
  EXPECT_FALSE(club.is_open({20, 0}));
}
