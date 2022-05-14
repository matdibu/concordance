#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.h"

static int
generate_concordance(const std::filesystem::path& /*lmao*/)
{
  return 0;
}

static int
verify_concordance(const std::filesystem::path& /*lmao*/,
                   const std::filesystem::path& /*lol*/)
{
  return 0;
}

int
main(int argc, char* argv[])
{
  int ret = 0;
  try
  {
    if (argc < 2) {
      std::cerr << "no input file given" << std::endl;
    } else if (argc == 2) {
      std::filesystem::path input =
        argv[1]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      ret = generate_concordance(input);
    } else if (argc == 3) {
      std::filesystem::path input =
        argv[1]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      std::filesystem::path expected =
        argv[2]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      ret = verify_concordance(input, expected);
    }
  } catch (const std::exception& exc) {
    std::cerr << "exception in main " << exc.what() << std::endl;
    ret = -1;
  }

  return ret;
}
