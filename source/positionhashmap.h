#ifndef POS_HASH_H
#define POS_HASH_H

#include "taghashmap.h"
#include "playerhashmap.h"

class PositionHashMap : public TagHashMap {
private:
    /**
     * Sorts a vector in descending order by player rating using insertion sort.
     *
     * @param vector The vector to be sorted.
     * @param players The PlayerHashMap containing player information.
     */
    void sort_descending_by_rating(std::vector<uint32_t>& vector, PlayerHashMap players) {
        int i, j;
        for (j = 1; j < vector.size(); j++) {
            uint32_t key = vector[j];
            i = j - 1;
            double rating_key = players.search(key)->global_rating;
            while (i >= 0 && players.search(vector[i])->global_rating < rating_key) {
                vector[i + 1] = vector[i];
                i--;
            }
            vector[i + 1] = key;
        }
    }

public:
    PositionHashMap(uint32_t tsize) : TagHashMap(tsize) {};

    /**
     * Retrieves the top N player IDs for a given position.
     *
     * @param n The maximum number of player IDs to retrieve.
     * @param position The position for which to retrieve top players.
     * @return A vector containing the top N player IDs for the given position.
     */
    std::vector<uint32_t> topn(size_t n, std::string position) {
        TagVector* pos_ptr = search(position);

        // Return the first N elements of the vector
        auto start = pos_ptr->vector.begin();
        auto end = start + std::min<size_t>(n, pos_ptr->vector.size());
        std::vector<uint32_t> players(start, end);

        return players;
    }

    /**
     * Loads player data into tags based on their positions.
     *
     * @param players The PlayerHashMap containing player data.
     */
    void load_players(PlayerHashMap players) {
        // Insert players into their corresponding position vectors
        for (int i = 0; i < players.table_size; i++) {
            for (auto& player : players.table[i]) {
                for (auto& position : player.positions) {
                    if (player.rating_count >= 1000) {
                        insert_player_to_tag(player.id, position);
                    }
                }
            }
        }
        // Sort the position vectors in descending order
        for (int i = 0; i < table_size; i++) {
            for (auto& positions : table[i]) {
                sort_descending_by_rating(positions.vector, players);
            }
        }
    }
};

#endif // POS_HASH_H