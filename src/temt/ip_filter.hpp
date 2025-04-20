#include <cassert>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace temt {
namespace ip {
std::vector<std::string> ipv4_split(const std::string& str, char d);
std::list<std::vector<std::string>> lexicographically_sort(const std::vector<std::vector<std::string>>& ips);
std::list<std::vector<std::string>> ipv4_filter(const std::list<std::vector<std::string>>& ips,
                                                int octet_index,
                                                int octet_value);
std::list<std::vector<std::string>> ipv4_filter_any(const std::list<std::vector<std::string>>& ips, int octet_value);
}  // namespace ip
}  // namespace temt