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
    auto occurences = sentence_occurances_pair.second;

    while (occurences-- > 0) {
      if (!is_first) {
        result_stream << "," << sentence_no;
      } else {
        result_stream << sentence_no;
        is_first = false;
      }
    }
  }

  return result_stream.str();
}

static bool
is_sententce_transition(const std::string& lhs, const std::string& rhs)
{
  if (lhs.ends_with('.')) {
    if (!rhs.empty()) {
      if (0 != std::isupper(rhs.at(0))) {
        return true;
      }
    }
  }

  return false;
}

static void to_lower(std::string& string)
{
    std::transform(string.begin(), string.end(), string.begin(), [](unsigned char chr) {
      return std::tolower(chr);
    });
}

static occurance_map_t
file_to_occurance_map(std::ifstream& file_input)
{
  occurance_map_t occurance_map;

  std::string word;
  std::string next;
  std::size_t sentence_count = 1;

  file_input >> word;
  while (file_input >> next) {

    const bool is_sententce_tran = is_sententce_transition(word, next);

    to_lower(word);

    if (is_sententce_tran || word.ends_with(',') || word.ends_with(':'))
    {
      word.erase(word.length() - 1);
    }

    occurance_map[word][sentence_count]++;

    if (is_sententce_tran)
    {
        sentence_count++;
    }

    word = next;
  }

  if (!next.empty()) {
    if (next.ends_with('.')) {
      next.erase(next.length() - 1);
    }
    occurance_map[next][sentence_count]++;
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

#if 1
static std::ostream&
operator<<(std::ostream& ostream, const occurance_map_t::value_type& occurance_pair)
{
  const auto word = occurance_pair.first;
  const auto occurances = count_occurances(occurance_pair.second);
  const auto occurance_list = occurs_to_comma_list(occurance_pair.second);

  ostream << word << " {" << occurances << ":" << occurance_list << "}";

  return ostream;
}
#endif

static std::string
occurance_map_to_string(const occurance_map_t& occurance_map)
{
  std::ostringstream result_stream;

  for (const auto& occurance_pair : occurance_map) {
    result_stream << occurance_pair << std::endl;
    //const auto word = occurance_pair.first;
    //const auto occurances = count_occurances(occurance_pair.second);
    //const auto occurance_list = occurs_to_comma_list(occurance_pair.second);

    //std::cout << word << " {" << occurances << ":" << occurance_list << "}" << std::endl;

    //result_stream << word << " {" << occurances << ":" << occurance_list << "}" << std::endl;
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

int
main(int argc, char* argv[])
{
  return run_main(argc, argv);
}
