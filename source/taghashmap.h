#ifndef TAG_HASH_H
#define TAG_HASH_H

#include <iostream>
#include <vector>
#include <string>
#include "parser.h"
#include "hashmap.h"

#define PRIME 31

struct TagVector {
    std::string name;
    std::vector<uint32_t> vec;
};

class TagHashMap : public HashMap<TagVector, std::string> {
private:
    /**
     * Calculates a hash value for the given key.
     *
     * @param key The key for which to calculate the hash value (string : tag).
     * @return The calculated hash value (16-bit uint).
     */
    uint16_t hash(std::string key) {
        uint16_t hash_key = 0;
        for (char& c : key) {
            int i = static_cast<int>(c);
            hash_key = (PRIME * hash_key + i) % table_size;
        }
        return hash_key;
    }

    bool equal(TagVector item, std::string key) {
        return item.name == key;
    }

public:
    using HashMap<TagVector, std::string>::HashMap;

    /**
     * Inserts a player ID into the vector of a certaing tag.
     *
     * @param player_id The player ID to be inserted into the vector.
     * @param tag The tag into which the player ID will be inserted.
    */
    void insert_player_to_tag(uint32_t player_id, std::string tag) {
        TagVector* item_ptr = search(tag);
        if (!item_ptr) {
            TagVector item;
            item.name = tag;
            item.vec = { player_id };
            insert(tag, item);
            return;
        }
        int i = 0;
        for (auto& stored_id : item_ptr->vec) {
            if (stored_id == player_id) {
                return;
            }
            else if (stored_id > player_id) {
                break;
            }
            i++;
        }
        item_ptr->vec.insert(item_ptr->vec.begin() + i, player_id);
    }

    /**
     * Populates the TagHashMap by reading and parsing data from a CSV file.
     *
     * @param csv_filename The path to the CSV file containing the players tag data.
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
            uint32_t player_id;
            std::string tag;
            uint8_t column = 0;

            for (auto& field : row) {
                switch (column) {
                case 1: player_id = stoull(field, nullptr, 10); break;
                case 2: tag = field; break;
                }
                column++;
            }
            insert_player_to_tag(player_id, tag);
        }
    }
};

#endif // TAG_HASH_H