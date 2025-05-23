#include <iostream>
#include <filesystem>
#include <fstream>
#include <format>
#include <limits>

#include <pcclub/events.hpp>
#include <pcclub/club.hpp>
#include <pcclub/client.hpp>
#include <pcclub/time.hpp>
#include <pcclub/utils.hpp>
#include <pcclub/ostream_buffer.hpp>

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "args error!\n";
    return 1;
  }

  auto src = pc::utils::get_path_from_string(argv[1]);
  std::ifstream in(src);

  std::size_t table = 0;
  pc::time_stamp open{};
  pc::time_stamp close{};
  std::size_t price = 0;
  in >> table >> open >> close >> price;
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  if (!in)
  {
    std::cerr << "input error! (init stage)\n";
    return 1;
  }

  pc::ostream_buffer buf(std::cout);
  bool was_error = false;
  std::string line;
  pc::club club(open, close, price, table);
  std::cout << std::format("{}\n", open);
  while (std::getline(in, line))
  {
    std::stringstream iss(line);
    pc::event_fields fields;
    iss >> fields;
    if (!iss)
    {
      was_error = true;
      break;
    }

    std::cout << fields << "\n";
    while (fields.id != 0)
    {
      auto event_out = event_call(club, fields);
      fields.str_data = event_out.first;
      fields.id = event_out.second;
      if (fields.id != 0)
      {
        std::cout << fields << "\n";
      }
    }
  }
  if (was_error)
  {
    buf.clear();
    std::cout << line << "\n";
    buf.flush();
    return 1;
  }

  // event 11

  std::cout << std::format("{}\n", close);

  auto profits = club.lock_in_profits();
  for (const auto & profit : profits)
  {
    std::cout << std::format("{} {} {}\n", std::get< 0 >(profit), std::get< 1 >(profit), std::get< 2 >(profit));
  }
  buf.flush();

  return 0;
}
