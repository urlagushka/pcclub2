#include <pcclub/events.hpp>

#include <unordered_map>

pc::event_ret
pc::event_call(std::size_t id, club & ft_club, const uts & ts, const std::string & str_data, std::size_t sub_data)
{
  using event_signature = event_ret (*)(club &, const uts &, const std::string &, std::size_t);
  std::unordered_map< std::size_t, event_signature > event_map = {
    {1, event_1},
    {2, event_2},
    {3, event_3},
    {4, event_4},
    {11, event_11},
    {12, event_12},
    {13, event_13}
  };

  return event_map.at(id)(ft_club, ts, str_data, sub_data);
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

  if (ft_club.is_free_table())
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
