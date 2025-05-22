#include <pcclub/club.hpp>
#include <iostream>

#include <algorithm>

pc::club::club(pc::time_stamp open, pc::time_stamp close, std::size_t price, std::size_t table):
  __client(0),
  __table(table + 1, std::make_pair(std::nullopt, std::chrono::minutes(0))),

  __open(open),
  __close(close),

  __price(price)
{}

void
pc::club::add_client(const client & rhs)
{
  if (is_client_inside(rhs))
  {
    throw std::runtime_error("YouShallNotPass");
  }

  if (!is_open(rhs.time.in_time.value()))
  {
    throw std::runtime_error("NotOpenYet");
  }

  __client.push_front(rhs);
}

void
pc::club::pop_client(const client & rhs)
{
  if (!is_client_inside(rhs))
  {
    throw std::runtime_error("ClientUnknown");
  }

  auto in_client = get_client_it(rhs);
  if (in_client != __client.end())
  {
    __client.erase(in_client);
    return;
  }

  auto in_table = get_table_it(rhs);
  in_table->first->time.out_time = rhs.time.out_time;
  free_table(in_table->first.value());
}

std::optional< std::string >
pc::club::satisfy_queue()
{
  if (__client.size() == 0)
  {
    return std::nullopt;
  }

  auto found = std::find_if(__table.begin() + 1, __table.end(), [](const table_vec & lhs)
  {
    return !lhs.first.has_value();
  });
  if (found != __table.end())
  {
    return std::nullopt;
  }

  auto to_ret = found->first.value().name;
  take_table(__client.back(), std::distance(__table.begin(), found));
  return to_ret;
}

void
pc::club::take_table(const client & rhs, std::size_t table)
{
  if (!is_client_inside(rhs))
  {
    throw std::runtime_error("ClientUnknown");
  }
  if (__table[table].first.has_value())
  {
    throw std::runtime_error("PlaceIsBusy");
  }

  auto old_place = get_table_it(rhs);
  if (old_place != __table.end())
  {
    old_place->first.value().time.out_time = rhs.time.in_time;
    free_table(old_place->first.value());
  }

  auto in_client = get_client_it(rhs);
  in_client->time.in_time = rhs.time.in_time;
  __table[table].first = *in_client;
  __client.erase(in_client);
}

void
pc::club::free_table(const client & rhs)
{
  auto found = get_table_it(rhs);
  if (found == __table.end())
  {
    return;
  }

  auto in = found->first.value().time.in_time.value().to_minutes();
  auto out = found->first.value().time.out_time.value().to_minutes();
  auto diff = out - in;
  found->second += diff;
  found->first = std::nullopt;
}

std::size_t
pc::club::table_size() const
{
  return __table.size();
}

std::size_t
pc::club::client_size() const
{
  return __client.size();
}

bool
pc::club::is_open(const time_stamp & rhs) const
{
  return (rhs > __open && rhs < __close);
}

bool
pc::club::is_client_inside(const client & rhs) const
{
  return get_client_it(rhs) != __client.end() || get_table_it(rhs) != __table.end();
}

bool
pc::club::is_free_table() const
{
  auto found = std::find_if(__table.begin() + 1, __table.end(), [](const table_vec & lhs)
  {
    return !lhs.first.has_value();
  });

  return found != __table.end();
}

std::deque< pc::client >::iterator
pc::club::get_client_it(const client & rhs)
{
  return std::find_if(__client.begin(), __client.end(), [& rhs](client & lhs)
  {
    return lhs.name == rhs.name;
  });
}

std::deque< pc::client >::const_iterator
pc::club::get_client_it(const client & rhs) const
{
  return std::find_if(__client.cbegin(), __client.cend(), [& rhs](const client & lhs)
  {
    return lhs.name == rhs.name;
  });
}

std::vector< pc::club::table_vec >::iterator
pc::club::get_table_it(const client & rhs)
{
  return std::find_if(__table.begin(), __table.end(), [& rhs](table_vec & lhs)
  {
    return lhs.first.has_value() && lhs.first->name == rhs.name;
  });
}

std::vector< pc::club::table_vec >::const_iterator
pc::club::get_table_it(const client & rhs) const
{
  return std::find_if(__table.cbegin(), __table.cend(), [& rhs](const table_vec & lhs)
  {
    return lhs.first.has_value() && lhs.first->name == rhs.name;
  });
}

std::vector< pc::club::profit >
pc::club::lock_in_profits()
{
  for (auto & table : __table)
  {
    if (table.first.has_value())
    {
      table.first->time.out_time = __close;

      auto in = table.first.value().time.in_time.value().to_minutes();
      auto out = table.first.value().time.out_time.value().to_minutes();
      auto diff = out - in;
      table.second += diff;
    }
  }

  std::vector< pc::club::profit > profits;
  profits.reserve(__table.size());
  for (std::size_t i = 1; i < __table.size(); ++i)
  {
    std::size_t price = __table[i].second.count() / 60 * __price;
    time_stamp time(__table[i].second);
    profits.push_back(std::make_tuple(i, price, time));
  }

  return profits;
}
