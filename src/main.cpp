#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "config.h"

static std::string
gen_con(const std::filesystem::path& path_input)
{

  if (!std::filesystem::exists(path_input)) {
    throw std::runtime_error("input file path is invalid");
  }

  std::ostringstream result_stream;
  std::ifstream file_input(path_input);
  // file_input.exceptions(std::ios_base::badbit | std::ios_base::failbit);
  std::string prev;
  std::string word;
  std::size_t sentence_count = 1;
  while (file_input >> word) {

    if (!prev.empty() && !word.empty()) {
      if (prev.ends_with('.')) {
        if (0 != std::isupper(word.at(0))) {
          sentence_count++;
        }
      }
    }

    std::cout << sentence_count << " : " << word << std::endl;
    result_stream << sentence_count << " : " << word << std::endl;
    prev = word;
  }

  return result_stream.str();
}

static int
generate_concordance(const std::filesystem::path& path_input)
{
  auto result = gen_con(path_input);
  std::cout << "gen_con result: " << std::endl << result << std::endl;
  return 0;
}

static int
verify_concordance(const std::filesystem::path& path_input, const std::filesystem::path& path_expected)
{
  auto computed_str = gen_con(path_input);
  std::cout << "gen_con result: " << std::endl << computed_str << std::endl;
  auto computed = std::istringstream(computed_str);

  if (!std::filesystem::exists(path_expected)) {
    throw std::runtime_error("expected file path is invalid");
  }
  std::ifstream file_expected(path_expected);

  file_expected.seekg(0, std::ifstream::end);
  if (const auto file_size = static_cast<std::size_t>(file_expected.tellg()); file_size != computed_str.length()) {
    std::cout << "file_expected:" << file_size << " computed_str:" << computed_str.length() << std::endl;
    return -1;
  }
  file_expected.seekg(0, std::ifstream::beg);

  const bool is_equal = std::equal(std::istreambuf_iterator<char>(file_expected.rdbuf()),
                                   std::istreambuf_iterator<char>(),
                                   std::istreambuf_iterator<char>(computed.rdbuf()));

  return is_equal ? 0 : 1;
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
  } catch (const std::ios_base::failure& fail) {
    std::cout << "exc code:" << fail.code() << " : " << fail.what() << std::endl;
  } catch (const std::exception& exc) {
    std::cerr << "exception in main " << exc.what() << std::endl;
    ret = -1;
  }

  std::cout << "returing " << ret << std::endl;

  return ret;
}
