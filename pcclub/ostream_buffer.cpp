#include <pcclub/ostream_buffer.hpp>

pc::ostream_buffer::ostream_buffer(std::ostream & out):
  __src(out.rdbuf()),
  __buffer(),
  __out(out)
{
  out.rdbuf(this);
}

pc::ostream_buffer::~ostream_buffer()
{
  if (__src)
  {
    __out.rdbuf(__src);
  }
}

void
pc::ostream_buffer::clear()
{
  __buffer.clear();
}

void
pc::ostream_buffer::flush()
{
  if (!__buffer.empty())
  {
    __src->sputn(__buffer.data(), __buffer.size());
    __buffer.clear();
  }
}

int
pc::ostream_buffer::overflow(int c)
{
  if (c != EOF)
  {
    __buffer.push_back(static_cast< char >(c));
  }

  return c;
}

std::streamsize
pc::ostream_buffer::xsputn(const char * s, std::streamsize n)
{
  __buffer.append(s, n);
  return n;
}
