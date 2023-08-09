#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <cctype>
#include "parser.h"

#include <iostream>

#define ALPHABET_SIZE 26 + 5  // 26 letters plus 5 special characters

class PlayerNameTrie {
private:
    PlayerNameTrie* links[ALPHABET_SIZE] = { 0 };
    std::vector<uint32_t> player_ids;

    /**
     * Recursively traverses the trie and gathers player IDs.
     *
     * @param id_vector The vector to store gathered IDs.
     */
    void gather_ids(std::vector<uint32_t>& id_vector) {
        if (!(this->player_ids.empty())) {
            // Concatenate both vectors
            id_vector.insert(
                id_vector.end(), this->player_ids.begin(), this->player_ids.end()
            );
        }
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (links[i]) {
                links[i]->gather_ids(id_vector);
            }
        }
    }

    /**
     * Converts an ASCII character to its corresponding alphabet index.
     *
     * @param c The ASCII character to convert.
     * @return The alphabet index.
     */
    int ascii_to_alphabet(char c) {
        switch (c) {
        case '"': return ALPHABET_SIZE - 5;
        case '\'': return ALPHABET_SIZE - 4;
        case '-': return ALPHABET_SIZE - 3;
        case ' ': return ALPHABET_SIZE - 2;
        case '.': return ALPHABET_SIZE - 1;
        default: return int(toupper(c)) - 65;
        }
    }

public:
    /**
     * Inserts a player's name into the trie along with the corresponding player's
     * sofifa_id at the leaf node.
     *
     * @param player_name The name of the player to be inserted.
     * @param player_id The sofifa_id associated with the player.
     */
    void insert(std::string player_name, uint32_t player_id) {
        PlayerNameTrie* ptr = this;
        for (auto& c : player_name) {
            int i = this->ascii_to_alphabet(c);
            if (!ptr->links[i]) {
                ptr->links[i] = new PlayerNameTrie;
            }
            ptr = ptr->links[i];
        }
        ptr->player_ids.push_back(player_id);
    }

    /**
     * Searches for players whose names have a given prefix and returns a vector
     * containing their respective sofifa_id's.
     *
     * @param prefix The prefix to search for in player names.
     * @return A vector containing the sofifa_id's of players whose names match the
     *         given prefix.
     */
    std::vector<uint32_t> search(std::string prefix) {
        PlayerNameTrie* ptr = this;
        // Search the prefix on the trie
        for (auto& c : prefix) {
            int i = this->ascii_to_alphabet(c);
            if (!ptr->links[i]) {
                return std::vector<uint32_t>();
            }
            ptr = ptr->links[i];
        }
        // Generate a list with all players on the prefix subtree
        std::vector<uint32_t> id_vector;
        ptr->gather_ids(id_vector);
        return id_vector;
    }

    void from_csv(std::string csv_filename) {
        std::ifstream csv_file(csv_filename);
        if (!csv_file) {
            std::cout << "[X] file " << csv_filename
                << " was not loaded." << std::endl;
        }
        csv_file.ignore(50, '\n');  // ignore header

        csv::CsvParser parser(csv_file);

        for (auto& row : parser) {
            std::string player_name;
            uint32_t player_id;
            int column = 0;

            for (auto& field : row) {
                switch (column) {
                case 0: player_id = stoull(field, nullptr, 10); break;
                case 1: player_name = field; break;
                }
                column++;
            }
            this->insert(player_name, player_id);
        }
    }

    ~PlayerNameTrie() {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (links[i]) {
                delete links[i];
            }
        }
    }
};

#endif // TRIE_H