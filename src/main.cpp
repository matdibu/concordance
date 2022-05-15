#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "config.h"

using sentence_freq_t = std::map<std::size_t, std::size_t>;
using occurance_map_t = std::map<std::string, sentence_freq_t>;

static std::string
occurs_to_comma_list(const sentence_freq_t& occurance_map)
{
  std::ostringstream result_stream;

  bool is_first = true;
  for (const auto& sentence_occurances_pair : occurance_map) {

    const auto sentence_no = sentence_occurances_pair.first;

    if (!is_first) {
      result_stream << "," << sentence_no;
    } else {
      result_stream << sentence_no;
      is_first = false;
    }
  }

  return result_stream.str();
}

constexpr static bool
is_sententce_transition(const std::string& prev, const std::string& word)
{
  if (prev.ends_with('.')) {
    if (!word.empty()) {
      if (0 != std::isupper(word.at(0))) {
        return true;
      }
    }
  }

  return false;
}

static occurance_map_t
file_to_occurance_map(std::ifstream& file_input)
{
  occurance_map_t occurance_map;

  std::string prev;
  std::string word;
  std::size_t sentence_count = 1;

  while (file_input >> word) {

    if (is_sententce_transition(prev, word)) {
      sentence_count++;
    }

    occurance_map[word][sentence_count]++;

    std::swap(prev, word);
  }

  return occurance_map;
}

static std::size_t
count_occurances(const sentence_freq_t& occurance_map)
{
  std::size_t total_occurrances = 0;

  for (const auto sentence_freq_pair : occurance_map) {
    total_occurrances += sentence_freq_pair.second;
  }

  return total_occurrances;
}

static std::ostream&
operator<<(std::ostream& ostream, const occurance_map_t::value_type& occurance_pair)
{
  const auto word = occurance_pair.first;
  const auto occurances = count_occurances(occurance_pair.second);
  const auto occurance_list = occurs_to_comma_list(occurance_pair.second);

  ostream << word << " {" << occurances << ":" << occurance_list << "}";

  return ostream;
}

static std::string
occurance_map_to_string(const occurance_map_t& occurance_map)
{
  std::ostringstream result_stream;

  for (const auto& occurance_pair : occurance_map) {
    result_stream << occurance_pair << std::endl;
  }

  return result_stream.str();
}

static std::string
file_to_concordance_string(std::ifstream& file_input)
{
  auto occur = file_to_occurance_map(file_input);

  return occurance_map_to_string(occur);
}

static std::string
path_to_concordance_string(const std::filesystem::path& path_input)
{
  if (!std::filesystem::exists(path_input)) {
    throw std::runtime_error("input file path is invalid");
  }

  std::ifstream file_input(path_input);

  return file_to_concordance_string(file_input);
}

static int
one_arg(const std::filesystem::path& path_input)
{
  auto result = path_to_concordance_string(path_input);
  std::cout << "gen_con result: " << std::endl << result << std::endl;
  return 0;
}

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

static int
run_main(int argc, char* argv[])
{
  int ret = 0;

  try {

    std::vector<std::string> args(argv, argv + argc);

    if (args.size() < 2) {
      std::cerr << "no input file given" << std::endl;
    } else if (args.size() == 2) {
      ret = one_arg(args[0]);
    } else if (args.size() == 3) {
      ret = two_arg(args[0], args[1]);
    }
  } catch (const std::ios_base::failure& fail) {
    std::cout << "exc code:" << fail.code() << " : " << fail.what() << std::endl;
  } catch (const std::exception& exc) {
    std::cerr << "exception in main " << exc.what() << std::endl;
    ret = -1;
  }

  return ret;
}

int
main(int argc, char* argv[])
{
  return run_main(argc, argv);
}
