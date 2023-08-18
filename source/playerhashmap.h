#ifndef PLAYER_HASH_H
#define PLAYER_HASH_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "csv.h"
#include "hashmap.h"
#include "ratinghashmap.h"

struct Player {
    uint32_t id;
    std::string name;
    std::vector<std::string> positions;
    double global_rating = 0;
    uint32_t rating_count = 0;
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

    /**
     * Formats a string of comma-separated positions into a vector of strings.
     *
     * @param string The input string containing comma-separated positions.
     * @return A vector of strings containing the formatted positions.
     */
    std::vector<std::string> format_positions(std::string string) {
        std::vector<std::string> positions;
        std::stringstream ss(string);
        std::string token;

        // Remove quotation marks
        if (string[0] == '"' && string[string.size() - 1] == '"') {
            string = string.substr(1, string.size() - 2);
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

    /**
     * Checks if the ID of a Player object corresponds to a given key.
     *
     * @param item The Player object to compare.
     * @param key The key (ID) to compare against.
     * @return True if the ID of the Player object is equal to the key, false otherwise.
     */
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

    /**
     * Loads ratings data into player global ratings and ratings count.
     *
     * @param ratings The RatingHashMap containing user ratings data.
     */
    void load_ratings(RatingHashMap ratings) {
        for (uint32_t i = 0; i < ratings.table_size; i++) {
            for (auto& user : ratings.table[i]) {
                for (auto& rating : user.ratings) {
                    Player* player = search(rating.player_id);
                    player->global_rating += (static_cast<double>(rating.score) - player->global_rating)
                        / static_cast<double>(++player->rating_count);
                }
            }
        }
    }
};

#endif // PLAYER_HASH_H