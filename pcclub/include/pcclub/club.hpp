#ifndef PCCLUB_CLUB_HPP
#define PCCLUB_CLUB_HPP

#include <chrono>
#include <deque>
#include <optional>
#include <tuple>

#include <pcclub/client.hpp>
#include <pcclub/time.hpp>

namespace pc
{
  class club
  {
    using table_vec = std::pair< std::optional< client >, std::chrono::minutes >;
    using profit = std::tuple< std::size_t, size_t, time_stamp >;
    public:
      club() = delete;
      club(time_stamp open, time_stamp close, std::size_t price, std::size_t table);

      void add_client(const client & rhs);
      void pop_client(const client & rhs);
      std::optional< std::string > satisfy_queue();

      void take_table(const client & rhs, std::size_t table);
      void free_table(const client & rhs);

      std::size_t table_size() const;
      std::size_t client_size() const;

      bool is_open(const time_stamp & rhs) const;
      bool is_client_inside(const client & rhs) const;
      bool is_free_table() const;

      std::vector< profit > lock_in_profits();

    private:
      std::deque< client > __client;
      std::vector< table_vec > __table;

      time_stamp __open;
      time_stamp __close;

      std::size_t __price;

      std::deque< client >::iterator get_client_it(const client & rhs);
      std::deque< client >::const_iterator get_client_it(const client & rhs) const;
      
      std::vector< table_vec >::iterator get_table_it(const client & rhs);
      std::vector< table_vec >::const_iterator get_table_it(const client & rhs) const;
  };
}

#endif
