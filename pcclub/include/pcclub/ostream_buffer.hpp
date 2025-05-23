#ifndef PCCLUB_OSTREAM_BUFFER_HPP
#define PCCLUB_OSTREAM_BUFFER_HPP

#include <ostream>
#include <streambuf>
#include <string>

namespace pc
{
  class ostream_buffer final:
    public std::streambuf
  {
    public:
      ostream_buffer() = delete;
      ostream_buffer(std::ostream & src);
      ~ostream_buffer();
      
      void clear();
      void flush();

    private:
      std::streambuf * __src;
      std::string __buffer;
      std::ostream & __out;

      int overflow(int c) override;
      std::streamsize xsputn(const char * s, std::streamsize n) override;
  };
}

#endif
