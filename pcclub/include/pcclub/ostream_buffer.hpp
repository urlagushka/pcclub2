#ifndef PCCLUB_OSTREAM_BUFFER_HPP
#define PCCLUB_OSTREAM_BUFFER_HPP

#include <ostream>
#include <streambuf>
#include <string>

namespace pc
{
  class ostream_buffer:
    public std::streambuf
  {
    public:
      ostream_buffer() = delete;
      ostream_buffer(std::ostream & src);

    private:
      std::streambuf * __src;
      std::string __buffer;

    protected:
    
  };
}

#endif
