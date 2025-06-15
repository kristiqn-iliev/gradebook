#pragma once

#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class EnvParser {
public:
    std::unordered_map<std::string, std::string> values;

    EnvParser(const std::string& path = "../.env") {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Could not open .env file\n";
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Skip empty lines or comments
            if (line.empty() || line[0] == '#') continue;

            std::istringstream iss(line);
            std::string key, value;

            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                values[trim(key)] = trim(value);
            }
        }
    }

    std::string get(const std::string& key, const std::string& fallback = "") const {
        auto it = values.find(key);
        //std::cout << "[GET] " << key << " => " << (it != values.end() ? it->second : "NOT FOUND") << "\n";
        return it != values.end() ? it->second : fallback;
    }

private:
    static std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
    }
};
