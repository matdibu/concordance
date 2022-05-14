#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.h"

static int
generate_concordance(const std::filesystem::path& path_input)
{
  std::ifstream file_input(path_input);
  std::string word;
  while (file_input >> word) {
    std::cout << word << std::endl;
  }
  return 0;
}

static int
verify_concordance(const std::filesystem::path& path_input, const std::filesystem::path& path_expected)
{
  (void)path_input;
  (void)path_expected;
  return 0;
}

int
main(int argc, char* argv[])
{
  int ret = 0;
  try {
    if (argc < 2) {
      std::cerr << "no input file given" << std::endl;
    } else if (argc == 2) {
      std::filesystem::path path_input(argv[1]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      ret = generate_concordance(path_input);
    } else if (argc == 3) {
      std::filesystem::path path_input(argv[1]);    // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      std::filesystem::path path_expected(argv[2]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      ret = verify_concordance(path_input, path_expected);
    }
  } catch (const std::exception& exc) {
    std::cerr << "exception in main " << exc.what() << std::endl;
    ret = -1;
  }

  return ret;
}
