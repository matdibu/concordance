#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "config.h"

static std::string
occurs_to_comma_list(const std::map<std::size_t, std::size_t>& occur_map)
{
  std::ostringstream result_stream;

  bool is_first = true;
  for (const auto& map_pair : occur_map) {
      const auto sentence_no = map_pair.first;
    if (!is_first) {
      result_stream << "," << sentence_no;
    } else {
      result_stream << sentence_no;
      is_first = false;
    }
  }

  return result_stream.str();
}

static std::string
gen_con(const std::filesystem::path& path_input)
{
  if (!std::filesystem::exists(path_input)) {
    throw std::runtime_error("input file path is invalid");
  }

  std::map<std::string, std::map<std::size_t, std::size_t>> occur;

  std::ifstream file_input(path_input);
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

    occur[word][sentence_count]++;

    prev = word;
  }

  std::ostringstream result_stream;
  for (const auto& occ : occur) {
    std::size_t total_occur = 0;
    for (const auto freq : occ.second) {
      total_occur += freq.second;
    }
    result_stream << occ.first << " {" << total_occur << ":" << occurs_to_comma_list(occ.second) << "}" << std::endl;
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
