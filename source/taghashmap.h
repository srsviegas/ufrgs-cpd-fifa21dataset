#ifndef TAG_HASH_H
#define TAG_HASH_H

#include <iostream>
#include <vector>
#include <string>
#include "csv.h"
#include "hashmap.h"

#define PRIME 31

struct TagVector {
    std::string name;
    std::vector<uint32_t> vector;
};

class TagHashMap : public HashMap<TagVector, std::string> {
private:
    /**
     * Calculates a hash value for the given key.
     *
     * @param key The key for which to calculate the hash value (string : tag).
     * @return The calculated hash value (16-bit uint).
     */
    uint32_t hash(std::string key) {
        uint32_t hash_key = 0;
        for (char& c : key) {
            uint32_t i = static_cast<uint32_t>(c);
            hash_key = (PRIME * hash_key + i) % table_size;
        }
        return hash_key;
    }

    /**
     * Checks if the name of a TagVector object corresponds to a given key.
     *
     * @param tag The TagVector object to compare.
     * @param key The key (tag name) to compare against.
     * @return True if the name of the TagVector object is equal to the key, false otherwise.
     */
    bool equal(TagVector tag, std::string key) {
        return tag.name == key;
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
            // Tag vector was still not initialized
            TagVector item;
            item.name = tag;
            item.vector = { player_id };
            insert(tag, item);
            return;
        }
        int i = 0;
        for (auto& stored_id : item_ptr->vector) {
            if (stored_id == player_id) {
                // Player ID already inside tag vector
                return;
            }
            else if (stored_id > player_id) {
                // Correct position for player ID found
                break;
            }
            i++;
        }
        item_ptr->vector.insert(item_ptr->vector.begin() + i, player_id);
    }

    /**
     * Searches for the intersection of player IDs based on provided tags.
     *
     * @note It is expected that the tag vectors are in ascending order.
     * @param tags A vector of strings representing the tags to search for.
     * @return A vector of uint32_t containing the common player IDs.
     */
    std::vector<uint32_t> search_tags(std::vector<std::string> tags) {
        std::vector<uint32_t> intersection;
        std::vector<std::vector<uint32_t>> vectors;
        std::vector<uint16_t> indexes(tags.size(), 0);

        // Find all tag vectors
        size_t i = 0;
        for (auto& tag : tags) {
            try {
                vectors.push_back(search(tag)->vector);
            }
            catch (...) {
                // If a tag is not found, return an empty vector
                return std::vector<uint32_t>();
            }
            i++;
        }

        // Find intersection of the tag vectors
        for (auto& player : vectors[0]) {
            bool present_in_all = true;
            for (i = 1; i < indexes.size(); i++) {
                while (vectors[i][indexes[i]] < player) {
                    indexes[i]++;
                    if (indexes[i] >= vectors[i].size()) {
                        return intersection;
                    }
                }
                if (vectors[i][indexes[i]] != player) {
                    present_in_all = false;
                }
            }
            if (present_in_all) {
                intersection.push_back(player);
            }
        }

        return intersection;
    }

    /**
     * Populates the TagHashMap by reading and parsing data from a CSV file.
     *
     * @param csv_filename The path to the CSV file containing the players tag data.
     */
    void from_csv(std::string csv_filename) {
        io::CSVReader<2> in(csv_filename);
        uint32_t player_id;
        std::string tag;

        in.read_header(io::ignore_extra_column, "sofifa_id", "tag");

        while (in.read_row(player_id, tag)) {
            insert_player_to_tag(player_id, tag);
        }
    }
};

#endif // TAG_HASH_H