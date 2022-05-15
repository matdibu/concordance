#include "config.h"

#include "concordance.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static int
two_arg(const std::filesystem::path& path_input,
        const std::filesystem::path& path_expected)
{
  auto computed_str = path_to_concordance_string(path_input);
  std::cout << "gen_con result: " << std::endl << computed_str << std::endl;
  auto computed = std::istringstream(computed_str);

  if (!std::filesystem::exists(path_expected)) {
    throw std::runtime_error("expected file path is invalid");
  }
  std::ifstream file_expected(path_expected);

  file_expected.seekg(0, std::ifstream::end);
  if (const auto file_size = static_cast<std::size_t>(file_expected.tellg());
      file_size != computed_str.length()) {
    std::cout << "file_expected:" << file_size
              << " computed_str:" << computed_str.length() << std::endl;
    return -1;
  }
  file_expected.seekg(0, std::ifstream::beg);

  const bool is_equal = std::equal(std::istreambuf_iterator<char>(file_expected.rdbuf()),
                                   std::istreambuf_iterator<char>(),
                                   std::istreambuf_iterator<char>(computed.rdbuf()));

  return is_equal ? 0 : 1;
}

int
one_arg(const std::filesystem::path& path_input)
{
  auto result = path_to_concordance_string(path_input);
  std::cout << "gen_con result: " << std::endl << result << std::endl;
  return 0;
}


int
main(int argc, char* argv[])
{
  int ret = 0;

  try {

    std::vector<std::string> args(argv, argv + argc);

    if (args.size() < 2) {
      std::cerr << "no input file given" << std::endl;
    } else if (args.size() == 2) {
      ret = one_arg(args[1]);
    } else if (args.size() == 3) {
      ret = two_arg(args[1], args[2]);
    }
  } catch (const std::ios_base::failure& fail) {
    std::cout << "exc code:" << fail.code() << " : " << fail.what() << std::endl;
  } catch (const std::exception& exc) {
    std::cerr << "exception in main " << exc.what() << std::endl;
    ret = -1;
  }

  return ret;
}
