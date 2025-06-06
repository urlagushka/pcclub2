#ifndef PCCLUB_CLUB_HPP
#define PCCLUB_CLUB_HPP

#include <chrono>
#include <deque>
#include <optional>
#include <tuple>

#include <pcclub/club_structs.hpp>
#include <pcclub/time.hpp>

namespace pc
{
  class club
  {
    using ts = time_stamp;
    using client_table = std::pair< std::string, std::size_t >;
    using profit = std::tuple< std::size_t, std::size_t, ts >;
    public:
      club() = delete;
      club(const ts & open, const ts & close, std::size_t price, std::size_t table_size);

      void add_client(const client & m_client);
      void pop_client(const client & m_client);
      std::optional< client_table > satisfy_queue(const ts & m_ts);

      void take_table(const client & m_client, std::size_t table_id);
      void free_table(const client & m_client);

      std::size_t table_size() const;
      std::size_t client_size() const;

      bool is_open(const ts & m_ts) const;
      bool is_client_inside(const client & m_client) const;
      bool are_free_table() const;

      std::vector< client > get_client_alphabet();
      std::vector< profit > lock_in_profits();

    private:
      std::deque< client > __client;
      std::vector< table > __table;

      time_stamp __open;
      time_stamp __close;

      std::size_t __price;

      std::deque< client >::iterator get_client_it(const client & m_client);
      std::deque< client >::const_iterator get_client_it(const client & m_client) const;
      
      std::vector< table >::iterator get_table_it(const client & m_client);
      std::vector< table >::const_iterator get_table_it(const client & m_client) const;

      std::vector< table >::iterator get_free_table_it();
      std::vector< table >::const_iterator get_free_table_it() const;
  };
}

#endif
