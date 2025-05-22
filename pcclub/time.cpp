#include <pcclub/time.hpp>

#include <chrono>
#include <string>
#include <format>
#include <sstream>
#include <iomanip>
#include <charconv>

pc::time_stamp::time_stamp(std::chrono::minutes rhs):
  hours(std::chrono::duration_cast< std::chrono::hours >(rhs)),
  minutes(rhs - hours)
{}

std::chrono::minutes
pc::time_stamp::to_minutes() const
{
  return hours + minutes;
}

bool
pc::time_stamp::operator==(const time_stamp & rhs) const
{
  return to_minutes() == rhs.to_minutes();
}

bool
pc::time_stamp::operator!=(const time_stamp & rhs) const
{
  return !(*this == rhs);
}

bool
pc::time_stamp::operator<(const time_stamp & rhs) const
{
  return to_minutes() < rhs.to_minutes();
}

bool
pc::time_stamp::operator>(const time_stamp & rhs) const
{
  return rhs < *this;
}

bool
pc::time_stamp::operator<=(const time_stamp & rhs) const
{
  return !(rhs < *this);
}

bool
pc::time_stamp::operator>=(const time_stamp & rhs) const
{
  return !(*this < rhs);
}

std::istream &
pc::operator>>(std::istream & in, time_stamp & ts)
{
  std::string tmp;
  in >> tmp;

  if (tmp.size() != 5 || tmp[2] != ':')
  {
    in.setstate(std::ios_base::failbit);
  }

  int hours = 0;
  int minutes = 0;

  auto [ptr_h, ec_h] = std::from_chars(tmp.data(), tmp.data() + 2, hours);
  auto [ptr_m, ec_m] = std::from_chars(tmp.data() + 3, tmp.data() + 5, minutes);

  if (ec_h != std::errc() || ec_m != std::errc())
  {
    in.setstate(std::ios_base::failbit);
  }
  if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59)
  {
    in.setstate(std::ios_base::failbit);
  }

  ts.hours = std::chrono::hours(hours);
  ts.minutes = std::chrono::minutes(minutes);

  return in;
}

std::ostream &
pc::operator<<(std::ostream & out, const time_stamp & ts)
{
  out << std::setfill('0') << std::setw(2) << ts.hours.count() << ":";
  out << std::setfill('0') << std::setw(2) << ts.minutes.count() << "\n";
  return out;
}
