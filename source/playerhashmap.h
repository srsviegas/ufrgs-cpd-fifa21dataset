#ifndef PLAYER_HASH_H
#define PLAYER_HASH_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "parser.h"
#include "hashmap.h"

struct Player {
    uint32_t id;
    std::string name;
    std::vector<std::string> positions;
    uint64_t rating_sum;
    uint32_t rating_count;
};


class PlayerHashMap : public HashMap<Player> {
private:
    /**
     * Calculates a hash value for the given key.
     *
     * @param key The key for which to calculate the hash value (32-bit uint).
     * @return The calculated hash value (16-bit uint).
     */
    uint16_t hash(uint32_t key) {
        return key % table_size;
    }

    std::vector<std::string> format_positions(std::string s) {
        std::vector<std::string> positions;
        std::stringstream ss(s);
        std::string token;

        // Remove quotation marks
        if (s[0] == '"' && s[s.size() - 1] == '"') {
            s = s.substr(1, s.size() - 2);
        }

        while (std::getline(ss, token, ',')) {
            // Remove leading space
            if (token[0] == ' ') {
                token.erase(token.begin());
            }
            positions.push_back(token);
        }

        return positions;
    }

    bool equal(Player item, uint32_t key) {
        return item.id == key;
    }

public:
    using HashMap<Player>::HashMap;

    /**
     * Populates the PlayerHashMap by reading and parsing data from a CSV file.
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
                case 2: player.positions = format_positions(field); break;
                }
                column++;
            }
            insert(player.id, player);
        }
    }
};

#endif // PLAYER_HASH_H