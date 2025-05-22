#include <pcclub/utils.hpp>

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
