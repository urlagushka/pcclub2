#ifndef PC_CLUB_CLIENT_HPP
#define PC_CLUB_CLIENT_HPP

#include <chrono>
#include <optional>
#include <string>

#include <pcclub/time.hpp>

namespace pc
{
  struct client_time
  {
    std::optional< time_stamp > in;
    std::optional< time_stamp > out;
  };

  struct client
  {
    std::string name;
    client_time time;
  };
}

#endif
