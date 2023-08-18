#ifndef RATING_HASH_H
#define RATING_HASH_H

#include <iostream>
#include <vector>
#include <string>
#include "csv.h"
#include "hashmap.h"

#include <algorithm>

struct Rating {
    uint32_t player_id;
    float score;
};

struct User {
    uint32_t id;
    std::vector<Rating> ratings;
};

class RatingHashMap : public HashMap<User> {
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

    bool equal(User item, uint32_t key) {
        return item.id == key;
    }

public:
    using HashMap<User>::HashMap;

    std::vector<Rating> top20_from_user(uint32_t user_id) {
        User* user_ptr = search(user_id);
        if (!user_ptr) {
            // User not found, returning empty vector
            return std::vector<Rating>();
        }

        // Sort the ratings vector in descending order
        //sort_descending(user_ptr->ratings);
        std::sort(user_ptr->ratings.begin(), user_ptr->ratings.end(),
            [](const Rating& a, const Rating& b) {
                return a.score > b.score;
            });

        // Return the first 20 elements of the vector
        auto start = user_ptr->ratings.begin();
        std::vector<Rating> ratings(start, start + 20);
        return ratings;
    }

    /*
     * Inserts a rating into the ratings vector of a certaing user.
     *
     * @param rating The rating to be inserted into the vector.
     * @param user_id The user into which the rating will be inserted.
     */
    void insert_rating_to_user(Rating rating, uint32_t user_id) {
        User* user_ptr = search(user_id);
        if (!user_ptr) {
            // User vector was still not initialized
            User item;
            item.id = user_id;
            item.ratings = { rating };
            insert(user_id, item);
            return;
        }
        // Insert rating at the end of the vector
        user_ptr->ratings.push_back(rating);
    }

    /**
     * Populates the TagHashMap by reading and parsing data from a CSV file.
     *
     * @param csv_filename The path to the CSV file containing the players tag data.
     */
    void from_csv(std::string csv_filename) {
        io::CSVReader<3> in(csv_filename);
        uint32_t user_id;
        Rating rating;

        in.read_header(io::ignore_no_column, "user_id", "sofifa_id", "rating");

        while (in.read_row(user_id, rating.player_id, rating.score)) {
            insert_rating_to_user(rating, user_id);
        }
    }
};


#endif // RATING_HASH_H