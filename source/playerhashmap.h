#ifndef PLAYER_HASH_H
#define PLAYER_HASH_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "csv.h"
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
    uint32_t hash(uint32_t key) {
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
        io::CSVReader<3, io::trim_chars<' '>, io::double_quote_escape<',', '\"'> > in(csv_filename);
        Player player;
        std::string positions;

        in.read_header(io::ignore_extra_column, "sofifa_id", "name", "player_positions");

        while (in.read_row(player.id, player.name, positions)) {
            player.positions = format_positions(positions);
            insert(player.id, player);
        }
    }
};

#endif // PLAYER_HASH_H