#ifndef PLAYER_HASH_H
#define PLAYER_HASH_H

#include <iostream>
#include <sstream>
#include "parser.h"
#include "hashmap.h"

struct Player {
    uint32_t id;
    std::string name;
    std::vector<std::string> positions;
    uint64_t rating_sum;
    uint32_t rating_count;

    bool operator==(const uint32_t other_id) const {
        return id == other_id;
    }
};


class PlayerHashMap : public HashMap<Player> {
private:
    std::vector<std::string> format_positions(std::string s) {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string token;

        // Remove quotation marks
        if (s[0] == '"' && s[s.size() - 1] == '"') {
            s = s.substr(1, s.size() - 2);
        }

        while (std::getline(ss, token, ',')) {
            result.push_back(token);
        }

        return result;
    }

public:
    using HashMap<Player>::HashMap;

    /**
     * Populates the PlayerNameTrie by reading and parsing data from a CSV file.
     *
     * @param csv_filename The path to the CSV file containing the FIFA players data.
     */
    void from_csv(std::string csv_filename) {
        std::ifstream csv_file(csv_filename);
        if (!csv_file) {
            std::cout << "[X] file " << csv_filename
                << " was not loaded." << std::endl;
        }
        csv_file.ignore(50, '\n');  // ignore header

        csv::CsvParser parser(csv_file);

        for (auto& row : parser) {
            Player player;
            uint8_t column = 0;

            for (auto& field : row) {
                switch (column) {
                case 0: player.id = stoull(field, nullptr, 10); break;
                case 1: player.name = field; break;
                case 3: player.positions = format_positions(field); break;
                }
                column++;
            }
            insert(player.id, player);
        }
    }
};

#endif // PLAYER_HASH_H