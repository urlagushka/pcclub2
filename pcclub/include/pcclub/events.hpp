#ifndef PCCLUB_EVENTS_HPP
#define PCCLUB_EVENTS_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include <chrono>
#include <utility>

#include <pcclub/club.hpp>
#include <pcclub/time.hpp>

namespace pc
{
  using uts = time_stamp;
  using event_ret = std::pair< std::string, std::size_t >;

  event_ret
  event_call(std::size_t id, club & ft_club, const uts & ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_1(club & ft_club, const uts & ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_2(club & ft_club, const uts & ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_3(club & ft_club, const uts & ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_4(club & ft_club, const uts & ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_11(club & ft_club, const uts & ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_12(club & ft_club, const uts & ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_13(club & ft_club, const uts & ts, const std::string & str_data, std::size_t sub_data = 0);
}

#endif
