#include <iostream>
#include <filesystem>
#include <fstream>
#include <format>
#include <limits>

#include <pcclub/events.hpp>
#include <pcclub/club.hpp>
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

  try
  {
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
      pc::event_ret event_out;
      try
      {
        event_out = pc::event_call(club, fields);
      }
      catch (const std::out_of_range & error)
      {
        was_error = true;
        break;
      }

      if (event_out.next_event != 0)
      {
        auto last_out = pc::event_call(club, {
          fields.time,
          event_out.next_event,
          event_out.str_data,
          event_out.sub_data
        });
        pc::event_fields tmp = {
          fields.time,
          event_out.next_event,
          last_out.str_data,
          last_out.sub_data
        };
        std::cout << tmp << "\n";
      }
    }
    if (was_error)
    {
      buf.clear();
      std::cout << line << "\n";
      buf.flush();
      return 1;
    }

    auto clients = club.get_client_alphabet();
    for (const auto & [name, _] : clients)
    {
      pc::event_fields fields = {close, 11, name};
      pc::event_call(club, fields);
      std::cout << fields << "\n";
    }
    std::cout << std::format("{}\n", close);

    auto profits = club.lock_in_profits();
    for (const auto & [id, time, price] : profits)
    {
      std::cout << std::format("{} {} {}\n", id, time, price);
    }
    buf.flush();
  }
  catch (const std::runtime_error & error)
  {
    std::cerr << std::format("runtime_error: {}\n", error.what());
  }
  catch (const std::exception & error)
  {
    std::cerr << std::format("unknown: {}\n", error.what());
  }

  return 0;
}
