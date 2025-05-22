#include <pcclub/utils.hpp>

#include <fstream>
#include <string>

pc::utils::fs::path
pc::utils::get_path_from_string(std::string_view path)
{
  std::filesystem::path valid_path = path;
  verify_file_path(valid_path);

  return valid_path;
}

void
pc::utils::verify_file_path(const fs::path & path)
{
  if (!fs::exists(path))
  {
    throw std::runtime_error("verify_file_path: file does not exist!");
  }
  if (path.extension() != ".txt")
  {
    throw std::runtime_error("file extension is bad! (.txt required)");
  }
}

bool
pc::utils::verify_file_path_no_throw(const fs::path & path)
{
  try
  {
    verify_file_path(path);
  }
  catch (...)
  {
    return false;
  }

  return true;
}

bool
pc::utils::verify_file_data_no_throw(const fs::path & path, std::ostream & out)
{
  std::ifstream in(path);
  std::string line;

  while (getline(in, line))
  {

  }

  return true;
}
