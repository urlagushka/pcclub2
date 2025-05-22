#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <format>
#include <locale>

#include <pcclub/events.hpp>
#include <pcclub/club.hpp>
#include <pcclub/client.hpp>
#include <pcclub/time.hpp>
#include <pcclub/utils.hpp>

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

  if (!in)
  {
    std::cerr << "input error! (init stage)\n";
    return 1;
  }

  std::cout << std::format("{}\n", open);
  pc::club club(open, close, price, table);
  while (in)
  {
    pc::time_stamp time{};
    std::size_t event_id = 0;
    std::string str_data;
    std::size_t sub_data = 0;

    in >> time >> event_id >> str_data;
    if (event_id == 2)
    {
      in >> sub_data;
    }

    if (!in)
    {
      break;
    }

    if (sub_data)
    {
      std::cout << std::format("{} {} {} {}\n", time, event_id, str_data, sub_data);
    }
    else
    {
      std::cout << std::format("{} {} {}\n", time, event_id, str_data);
    }
    while (event_id != 0)
    {
      auto event_out = event_call(event_id, club, time, str_data, sub_data);
      str_data = event_out.first;
      event_id = event_out.second;
      if (event_id != 0)
      {
        std::cout << std::format("{} {} {}\n", time, event_id, str_data);
      }
    }
  }
  std::cout << std::format("{}\n", close);
  
  auto profits = club.lock_in_profits();
  for (const auto & profit : profits)
  {
    std::cout << std::format("{} {} {}\n", std::get< 0 >(profit), std::get< 1 >(profit), std::get< 2 >(profit));
  }

  return 0;
}
