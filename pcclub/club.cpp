#include <pcclub/club.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>

pc::club::club(const ts & open, const ts & close, std::size_t price, std::size_t table_size):
  __client(0),
  __table(table_size, std::make_pair(std::nullopt, std::chrono::minutes(0))),

  __open(open),
  __close(close),

  __price(price)
{}

void
pc::club::add_client(const client & m_client)
{
  if (is_client_inside(m_client))
  {
    throw std::runtime_error("YouShallNotPass");
  }

  if (!is_open(m_client.time.in.value()))
  {
    throw std::runtime_error("NotOpenYet");
  }

  __client.push_back(m_client);
}

void
pc::club::pop_client(const client & m_client)
{
  if (!is_client_inside(m_client))
  {
    throw std::runtime_error("ClientUnknown");
  }

  auto d_table = get_table_it(m_client);
  if (d_table != __table.end())
  {
    d_table->first->time.out = m_client.time.out;
    free_table(d_table->first.value());
  }

  auto d_client = get_client_it(m_client);
  __client.erase(d_client);  
}

std::optional< std::string >
pc::club::satisfy_queue(const ts & m_ts)
{
  if (__client.size() == 0)
  {
    return std::nullopt;
  }

  auto found = get_free_table_it();
  if (found == __table.end())
  {
    return std::nullopt;
  }
  auto n_client = __client.front();
  n_client.time.in = m_ts;
  take_table(n_client, std::distance(__table.begin(), found) + 1);
  return n_client.name;
}

void
pc::club::take_table(const client & m_client, std::size_t table_id)
{
  if (--table_id > __table.size())
  {
    throw std::out_of_range("table_id is out of range!");
  }
  if (!is_client_inside(m_client))
  {
    throw std::runtime_error("ClientUnknown");
  }
  if (__table[table_id].first.has_value())
  {
    throw std::runtime_error("PlaceIsBusy");
  }

  auto old_place = get_table_it(m_client);
  if (old_place != __table.end())
  {
    old_place->first.value().time.out = m_client.time.in;
    free_table(old_place->first.value());
  }

  auto in_client = get_client_it(m_client);
  in_client->time.in = m_client.time.in;
  __table[table_id].first = *in_client;
  __client.erase(in_client);
}

void
pc::club::free_table(const client & m_client)
{
  auto found = get_table_it(m_client);
  if (found == __table.end())
  {
    return;
  }

  auto in = found->first.value().time.in.value().to_minutes();
  auto out = found->first.value().time.out.value().to_minutes();
  auto diff = out - in;

  found->second += diff;
  found->first = std::nullopt;
  __client.push_back(m_client);
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
pc::club::is_open(const ts & m_ts) const
{
  return (m_ts > __open && m_ts < __close);
}

bool
pc::club::is_client_inside(const client & m_client) const
{
  return get_client_it(m_client) != __client.end() || get_table_it(m_client) != __table.end();
}

bool
pc::club::are_free_table() const
{
  return get_free_table_it() != __table.end();
}

std::vector< pc::club::profit >
pc::club::lock_in_profits()
{
  for (auto & table : __table)
  {
    if (table.first.has_value())
    {
      std::cout << table.first->name << "\n";
      table.first->time.out = __close;

      auto in = table.first.value().time.in.value().to_minutes();
      auto out = table.first.value().time.out.value().to_minutes();
      auto diff = out - in;
      table.second += diff;
    }
  }

  std::vector< pc::club::profit > profits;
  profits.reserve(__table.size());
  for (std::size_t i = 0; i < __table.size(); ++i)
  {
    std::size_t price = static_cast< std::size_t >(std::ceil(__table[i].second.count() / 60.0)) * __price;
    time_stamp time(__table[i].second);
    profits.push_back(std::make_tuple(i + 1, price, time));
  }

  return profits;
}

std::deque< pc::client >::iterator
pc::club::get_client_it(const client & m_client)
{
  return std::find_if(__client.begin(), __client.end(), [& m_client](client & lhs)
  {
    return lhs.name == m_client.name;
  });
}

std::deque< pc::client >::const_iterator
pc::club::get_client_it(const client & m_client) const
{
  return std::find_if(__client.cbegin(), __client.cend(), [& m_client](const client & lhs)
  {
    return lhs.name == m_client.name;
  });
}

std::vector< pc::club::table >::iterator
pc::club::get_table_it(const client & m_client)
{
  return std::find_if(__table.begin(), __table.end(), [& m_client](table & lhs)
  {
    return lhs.first.has_value() && lhs.first->name == m_client.name;
  });
}

std::vector< pc::club::table >::const_iterator
pc::club::get_table_it(const client & m_client) const
{
  return std::find_if(__table.cbegin(), __table.cend(), [& m_client](const table & lhs)
  {
    return lhs.first.has_value() && lhs.first->name == m_client.name;
  });
}

std::vector< pc::club::table >::iterator
pc::club::get_free_table_it()
{
  auto found = std::find_if(__table.begin(), __table.end(), [](const table & lhs)
  {
    return !lhs.first.has_value();
  });

  return found;
}

std::vector< pc::club::table >::const_iterator
pc::club::get_free_table_it() const
{
  auto found = std::find_if(__table.cbegin(), __table.cend(), [](const table & lhs)
  {
    return !lhs.first.has_value();
  });

  return found;
}
