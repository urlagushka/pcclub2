#include <pcclub/club.hpp>

#include <algorithm>
#include <cmath>

pc::club::club(const ts & open, const ts & close, std::size_t price, std::size_t table_size):
  __client(0),
  __table(table_size),

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
    d_table->m_client->time.out = m_client.time.out;
    free_table(d_table->m_client.value());
  }

  auto d_client = get_client_it(m_client);
  __client.erase(d_client);  
}

std::optional< pc::club::client_table >
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
  std::size_t table_id = std::distance(__table.begin(), found) + 1;
  take_table(n_client, table_id);
  return std::make_pair(n_client.name, table_id);
}

void
pc::club::take_table(const client & m_client, std::size_t table_id)
{
  if (--table_id >= __table.size())
  {
    throw std::out_of_range("table_id is out of range!");
  }
  if (!is_client_inside(m_client))
  {
    throw std::runtime_error("ClientUnknown");
  }
  if (__table[table_id].m_client.has_value())
  {
    throw std::runtime_error("PlaceIsBusy");
  }

  auto old_place = get_table_it(m_client);
  if (old_place != __table.end())
  {
    old_place->m_client.value().time.out = m_client.time.in;
    free_table(old_place->m_client.value());
  }

  auto in_client = get_client_it(m_client);
  in_client->time.in = m_client.time.in;
  __table[table_id].m_client = *in_client;
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

  auto in = found->m_client.value().time.in.value().to_minutes();
  auto out = found->m_client.value().time.out.value().to_minutes();
  auto diff = out - in;

  found->total_time += diff;
  found->total_price += static_cast< std::size_t >(std::ceil(diff.count() / 60.0)) * __price;
  found->m_client = std::nullopt;
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

std::vector< pc::client >
pc::club::get_client_alphabet()
{
  std::vector< client > ret(__client.begin(), __client.end());

  for (const auto & i : __table)
  {
    if (i.m_client.has_value())
    {
      ret.push_back(i.m_client.value());
    }
  }

  std::sort(ret.begin(), ret.end(), [](const client & lhs, const client & rhs)
  {
    return lhs.name > rhs.name;
  });

  return ret;
}

std::vector< pc::club::profit >
pc::club::lock_in_profits()
{
  std::vector< pc::club::profit > profits;
  profits.reserve(__table.size());
  for (std::size_t i = 0; i < __table.size(); ++i)
  {
    profits.push_back(std::make_tuple(i + 1, __table[i].total_price, ts(__table[i].total_time)));
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

std::vector< pc::table >::iterator
pc::club::get_table_it(const client & m_client)
{
  return std::find_if(__table.begin(), __table.end(), [& m_client](table & lhs)
  {
    return lhs.m_client.has_value() && lhs.m_client->name == m_client.name;
  });
}

std::vector< pc::table >::const_iterator
pc::club::get_table_it(const client & m_client) const
{
  return std::find_if(__table.cbegin(), __table.cend(), [& m_client](const table & lhs)
  {
    return lhs.m_client.has_value() && lhs.m_client->name == m_client.name;
  });
}

std::vector< pc::table >::iterator
pc::club::get_free_table_it()
{
  auto found = std::find_if(__table.begin(), __table.end(), [](const table & lhs)
  {
    return !lhs.m_client.has_value();
  });

  return found;
}

std::vector< pc::table >::const_iterator
pc::club::get_free_table_it() const
{
  auto found = std::find_if(__table.cbegin(), __table.cend(), [](const table & lhs)
  {
    return !lhs.m_client.has_value();
  });

  return found;
}
