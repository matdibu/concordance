#include "config.h"

#include "concordance.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static int
compare_with_expected(
  const std::string& path_input, // NOLINT(bugprone-easily-swappable-parameters)
  const std::string& path_expected)
{
  if (!std::filesystem::exists(path_expected)) {
    std::cerr << "file " << path_expected << " does not exist" << std::endl;
    return -1;
  }

  const auto computed_str = path_to_concordance_string(path_input);
  const auto file_size = std::filesystem::file_size(path_expected);
  if (file_size != computed_str.length()) {
    std::cerr << "different sizes, file_expected:" << file_size
              << " computed_str:" << computed_str.length() << std::endl;
    return -1;
  }

  std::ifstream file_expected(path_expected);
  const auto computed_stream = std::istringstream(computed_str);
  const bool is_equal =
    std::equal(std::istreambuf_iterator<char>(file_expected.rdbuf()),
               std::istreambuf_iterator<char>(),
               std::istreambuf_iterator<char>(computed_stream.rdbuf()));

  return is_equal ? 0 : 1;
}

int
print_concordance(const std::string& path_input)
{
  auto result = path_to_concordance_string(path_input);
  std::cout << "result: " << std::endl << result << std::endl;
  return 0;
}

int
main(int argc, char* argv[])
{
  int ret = 0;

  try {

    const std::vector<std::string> args(argv, argv + argc);

    if (args.size() < 2) {
      std::cerr << "no input file given" << std::endl;
    } else if (args.size() == 2) {
      ret = print_concordance(args[1]);
    } else if (args.size() == 3) {
      ret = compare_with_expected(args[1], args[2]);
    }
  } catch (const std::ios_base::failure& fail) {
    std::cout << "exc code:" << fail.code() << " : " << fail.what() << std::endl;
  } catch (const std::exception& exc) {
    std::cerr << "exception in main " << exc.what() << std::endl;
    ret = -1;
  }

  return ret;
}
