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
  using event_signature = event_ret (*)(club &, const uts &, const std::string &, std::size_t);
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
pc::event_1(club & ft_club, const uts & ts, const std::string & name, std::size_t)
{
  client ft_client = {
    name,
    {ts, std::nullopt}
  };

  try
  {
    ft_club.add_client(ft_client);
  }
  catch (const std::runtime_error & error)
  {
    return std::make_pair(error.what(), 13);
  }

  return std::make_pair(name, 0);
}

pc::event_ret
pc::event_2(club & ft_club, const uts & ts, const std::string & name, std::size_t table)
{
  client ft_client = {
    name,
    {ts, std::nullopt}
  };

  try
  {
    ft_club.take_table(ft_client, table);
  }
  catch (const std::runtime_error & error)
  {
    return std::make_pair(error.what(), 13);
  }

  return std::make_pair(std::format("{} {}", name, table), 0);
}

pc::event_ret
pc::event_3(club & ft_club, const uts & ts, const std::string & name, std::size_t sub_data)
{
  client ft_client = {
    name,
    {std::nullopt, std::nullopt}
  };

  if (ft_club.are_free_table())
  {
    return std::make_pair("ICanWaitNoLonger", 13);
  }
  if (ft_club.client_size() > ft_club.table_size())
  {
    return std::make_pair(name, 11);
  }

  return std::make_pair(name, 0);
}

pc::event_ret
pc::event_4(club & ft_club, const uts & ts, const std::string & name, std::size_t)
{
  client ft_client = {
    name,
    {std::nullopt, ts}
  };

  try
  {
    ft_club.pop_client(ft_client);
    return std::make_pair(name, 12);
  }
  catch (const std::runtime_error & error)
  {
    return std::make_pair(error.what(), 13);
  }

  return std::make_pair(name, 0);
}

pc::event_ret
pc::event_11(club & ft_club, const uts & ts, const std::string & name, std::size_t)
{
  client ft_client = {
    name,
    {std::nullopt, ts}
  };

  try
  {
    ft_club.pop_client(ft_client);
  }
  catch (const std::runtime_error & error)
  {
    return std::make_pair(error.what(), 13);
  }

  return std::make_pair(name, 0);
}

pc::event_ret
pc::event_12(club & ft_club, const uts & ts, const std::string & str_data, std::size_t)
{
  auto ret = ft_club.satisfy_queue();
  if (ret.has_value())
  {
    return std::make_pair(ret.value(), 0);
  }
  return std::make_pair("client queue not changed", 0);
}

pc::event_ret
pc::event_13(club & ft_club, const uts & ts, const std::string & error, std::size_t)
{
  return std::make_pair(error, 0);
}
