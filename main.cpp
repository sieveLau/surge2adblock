#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <regex>

#include "include/curl_easy.h"
#include "include/curl_exception.h"
#include "include/curl_form.h"
#include "include/curl_ios.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::ostringstream;

using curl::curl_easy;
using curl::curl_easy_exception;
using curl::curl_ios;
using curl::curlcpp_traceback;

std::vector<std::string> manipulate(const std::string &origin) {

  std::regex pattern(R"((DOMAIN|DOMAIN-SUFFIX|DOMAIN-KEYWORD),(\S*),.*)");

  std::vector<std::string> return_val;
  std::istringstream str(origin);
  std::string line;
  while (std::getline(str, line)) {
    if (line.empty() || line.at(0) == '#')
      continue;

    std::cmatch m;

    std::regex_match(line.c_str(), m, pattern);

    if (m.empty())
      continue;

    auto directive = m[1];
    auto content = m[2];
    // cout << line << endl;
    // cout << "directive: " << directive << endl;
    // cout << "content: " << content << endl;

    const int k_buffer_size = 256;
    char domain_buffer[256] = {'\0'};
    const char *k_format_string = "||%s^";

    if (directive == "DOMAIN") {
      return_val.emplace_back(content);
    } else if (directive == "DOMAIN-SUFFIX") {
      snprintf(domain_buffer, k_buffer_size, k_format_string, content);
      return_val.emplace_back(domain_buffer);
      domain_buffer[0] = '\0';
    }
  }
  return return_val;
}

int main(int argc, char **argv) {

  if (argc < 2) {
    cout << "Usage: surge2adblock <source_file_url>" << endl;
    exit(1);
  }

  ostringstream str;
  curl_ios<ostringstream> writer(str);
  curl_easy easy(writer);

  easy.add<CURLOPT_URL>(argv[1]);
  easy.add<CURLOPT_FOLLOWLOCATION>(1L);
  try {
    easy.perform();
    for (auto &&line : manipulate(str.str())) {
      cout << line << endl;
    }
  } catch (curl_easy_exception &exception) {
    std::cerr << exception.what() << endl;
    exception.print_traceback();
  }
  return 0;
}
