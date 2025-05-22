#include <pcclub/time.hpp>

#include <chrono>
#include <string>
#include <format>
#include <sstream>
#include <iomanip>

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

  try
  {
    ts.hours = std::chrono::hours(std::stoi(tmp.substr(0, 2)));
    ts.minutes = std::chrono::minutes(std::stoi(tmp.substr(3, 5)));
  }
  catch (const std::invalid_argument &)
  {
    in.setstate(std::ios_base::failbit);
  }

  return in;
}

std::ostream &
pc::operator>>(std::ostream & out, const time_stamp & ts)
{
  out << std::setfill('0') << std::setw(2) << ts.hours.count() << ":";
  out << std::setfill('0') << std::setw(2) << ts.minutes.count() << "\n";
  return out;
}
