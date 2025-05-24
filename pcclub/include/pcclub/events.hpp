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
  struct event_fields
  {
    pc::time_stamp time;
    std::size_t id;
    std::string str_data;
    std::optional< std::size_t > sub_data;
  };

  std::istream & operator>>(std::istream & in, event_fields & fields);
  std::ostream & operator<<(std::ostream & out, const event_fields & fields);

  struct event_ret
  {
    std::size_t next_event;
    std::string str_data;
    std::optional< std::size_t > sub_data;
  };

  using ts = time_stamp;

  event_ret
  event_call(club & m_club, const event_fields & fields);

  event_ret
  event_1(club & m_club, const ts & m_ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_2(club & m_club, const ts & m_ts, const std::string & str_data, std::size_t sub_data);

  event_ret
  event_3(club & m_club, const ts & m_ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_4(club & m_club, const ts & m_ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_11(club & m_club, const ts & m_ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_12(club & m_club, const ts & m_ts, const std::string & str_data, std::size_t sub_data = 0);

  event_ret
  event_13(club & m_club, const ts & m_ts, const std::string & str_data, std::size_t sub_data = 0);
}

#endif
