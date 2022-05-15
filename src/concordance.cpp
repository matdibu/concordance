#include "concordance.hpp"

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

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

[[nodiscard]] static std::string
to_lower(const std::string& string)
{
  std::string result(string.length(), '\0');

  std::transform(string.begin(), string.end(), result.begin(), [](unsigned char chr) {
    return std::tolower(chr);
  });

  return result;
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

    word = to_lower(word);

    if (is_sententce_tran || word.ends_with(',') || word.ends_with(':')) {
      word.erase(word.length() - 1);
    }

    occurance_map[word][sentence_count]++;

    if (is_sententce_tran) {
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

static std::ostream& // NOLINT(fuchsia-overloaded-operator)
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

std::string
path_to_concordance_string(const std::filesystem::path& path_input)
{
  if (!std::filesystem::exists(path_input)) {
    throw std::runtime_error("input file path is invalid");
  }

  std::ifstream file_input(path_input);

  return file_to_concordance_string(file_input);
}

