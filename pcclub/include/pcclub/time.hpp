#ifndef PCCLUB_TIME_HPP
#define PCCLUB_TIME_HPP

#include <chrono>
#include <format>
#include <ostream>
#include <istream>

namespace pc
{
  struct time_stamp
  {
    std::chrono::hours hours;
    std::chrono::minutes minutes;

    time_stamp() = default;
    time_stamp(std::chrono::minutes rhs);
    std::chrono::minutes to_minutes() const;

    bool operator==(const time_stamp & rhs) const;
    bool operator!=(const time_stamp & rhs) const;
    bool operator<(const time_stamp & rhs) const;
    bool operator>(const time_stamp & rhs) const;
    bool operator<=(const time_stamp & rhs) const;
    bool operator>=(const time_stamp & rhs) const;
  };

  std::istream & operator>>(std::istream & in, time_stamp & ts);
  std::ostream & operator>>(std::ostream & out, const time_stamp & ts);
}

template <>
struct std::formatter< pc::time_stamp >
{
  constexpr auto parse(std::format_parse_context & ctx)
  {
    return ctx.begin();
  }

  auto format(const pc::time_stamp & ts, std::format_context & ctx) const
  {
    auto hours = ts.hours.count();
    auto minutes = ts.minutes.count();
    return std::format_to(ctx.out(), "{:02d}:{:02d}", hours, minutes);
  }
};

#endif
