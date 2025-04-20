#include "ip_filter.hpp"

namespace temt {
namespace ip {

std::vector<std::string> ipv4_split(const std::string& str, char d) {
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos) {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

std::list<std::vector<std::string>> lexicographically_sort(const std::vector<std::vector<std::string>>& ips) {
    std::list<std::vector<std::string>> r;

    if (ips.empty())
        return r;

    for (const auto& ip : ips) {
        auto r_ip = r.begin();

        while (r_ip != r.end()) {
            bool is_less = false;

            for (size_t i = 0; i < std::min(ip.size(), r_ip->size()); ++i) {
                int ip_octet = std::stoi(ip[i]);
                int r_ip_octet = std::stoi((*r_ip)[i]);

                if (ip_octet < r_ip_octet) {
                    is_less = true;
                    break;
                } else if (ip_octet > r_ip_octet) {
                    is_less = false;
                    break;
                }
            }

            if (is_less) {
                break;
            }

            ++r_ip;
        }

        r.insert(r_ip, ip);
    }

    return r;
}

std::list<std::vector<std::string>> ipv4_filter(const std::list<std::vector<std::string>>& ips,
                                                int octet_index,
                                                int octet_value) {
    std::list<std::vector<std::string>> r{};

    if (ips.size() == 0)
        return r;
    if (octet_index < 0)
        return r;
    if ((*ips.begin()).size() <= octet_index)
        return r;

    for (auto& ip : ips) {
        if (std::stoi(ip[octet_index]) == octet_value) {
            r.push_back(ip);
        }
    }

    return r;
}

std::list<std::vector<std::string>> filter_any(const std::list<std::vector<std::string>>& ips, int octet_value) {
    std::list<std::vector<std::string>> r{};

    if (ips.size() == 0)
        return r;

    for (auto& ip : ips) {
        for (auto octet_str : ip) {
            if (std::stoi(octet_str) == octet_value) {
                r.push_back(ip);
                break;
            }
        }
    }

    return r;
}

}  // namespace ip
}  // namespace temt
