#ifndef PCCLUB_UTILS_HPP
#define PCCLUB_UTILS_HPP

#include <filesystem>
#include <vector>
#include <string_view>

namespace pc::utils
{
  namespace fs = std::filesystem;

  fs::path
  get_path_from_string(std::string_view path);

  void
  verify_file_path(const std::filesystem::path & path);
}

#endif
