#include <pcclub/events.hpp>

#include <unordered_map>

std::istream &
pc::operator>>(std::istream & in, event_fields & fields)
{
  in >> fields.time >> fields.id >> fields.str_data;
  if (!in.eof())
  {
    std::size_t tmp = 0;
    in >> tmp;
    fields.sub_data = tmp;
  }

  return in;
}

std::ostream &
pc::operator<<(std::ostream & out, const event_fields & fields)
{
  out << std::format("{} {} {}", fields.time, fields.id, fields.str_data);
  if (fields.sub_data.has_value())
  {
    out << std::format(" {}", fields.sub_data.value());
  }

  return out;
}

pc::event_ret
pc::event_call(club & m_club, const event_fields & fields)
{
  using event_signature = event_ret (*)(club &, const ts &, const std::string &, std::size_t);
  static std::unordered_map< std::size_t, event_signature > event_map = {
    {1, event_1},
    {2, event_2},
    {3, event_3},
    {4, event_4},
    {11, event_11},
    {12, event_12},
    {13, event_13}
  };

  return event_map.at(fields.id)(m_club, fields.time, fields.str_data, fields.sub_data.value_or(0));
}

pc::event_ret
pc::event_1(club & m_club, const ts & m_ts, const std::string & name, std::size_t)
{
  client m_client = {
    name,
    {m_ts, std::nullopt}
  };

  try
  {
    m_club.add_client(m_client);
  }
  catch (const std::runtime_error & error)
  {
    return {13, error.what()};
  }

  return {0, name};
}

pc::event_ret
pc::event_2(club & m_club, const ts & m_ts, const std::string & name, std::size_t table_id)
{
  client m_client = {
    name,
    {m_ts, std::nullopt}
  };

  try
  {
    m_club.take_table(m_client, table_id);
  }
  catch (const std::runtime_error & error)
  {
    return {13, error.what()};
  }

  return {0, name, table_id};
}

pc::event_ret
pc::event_3(club & m_club, const ts & m_ts, const std::string & name, std::size_t)
{
  if (m_club.are_free_table())
  {
    return {13, "ICanWaitNoLonger!"};
  }
  if (m_club.client_size() > m_club.table_size())
  {
    return {11, name};
  }

  return {0, name};
}

pc::event_ret
pc::event_4(club & m_club, const ts & m_ts, const std::string & name, std::size_t)
{
  client m_client = {
    name,
    {std::nullopt, m_ts}
  };

  try
  {
    m_club.pop_client(m_client);
  }
  catch (const std::runtime_error & error)
  {
    return {13, error.what()};
  }

  if (m_club.client_size() != 0)
  {
    return {12, name};
  }
  return {0, name};
}

pc::event_ret
pc::event_11(club & ft_club, const ts & m_ts, const std::string & name, std::size_t)
{
  client m_client = {
    name,
    {std::nullopt, m_ts}
  };

  try
  {
    ft_club.pop_client(m_client);
  }
  catch (const std::runtime_error & error)
  {
    return {13, error.what()};
  }

  return {0, name};
}

pc::event_ret
pc::event_12(club & m_club, const ts & m_ts, const std::string &, std::size_t)
{
  auto ret = m_club.satisfy_queue(m_ts);
  if (ret.has_value())
  {
    return {0, ret.value().first, ret.value().second};
  }
  return {0, ""};
}

pc::event_ret
pc::event_13(club & m_club, const ts & m_ts, const std::string & error, std::size_t)
{
  return {0, error};
}
