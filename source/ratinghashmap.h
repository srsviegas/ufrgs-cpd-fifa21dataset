#ifndef RATING_HASH_H
#define RATING_HASH_H

#include <iostream>
#include <vector>
#include <string>
#include "csv.h"
#include "hashmap.h"
#include "sort.cpp"

#include <algorithm>

struct Rating {
    uint32_t player_id;
    float score;

    bool operator<(const Rating& other) const {
        return score < other.score
            || (score == other.score && player_id > other.player_id);
    }
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

    /**
     * Checks if the ID of a User object corresponds to a given key.
     *
     * @param user The User object to compare.
     * @param key The key (ID) to compare against.
     * @return True if the ID of the User object is equal to the key, false otherwise.
     */
    bool equal(User user, uint32_t key) {
        return user.id == key;
    }

public:
    using HashMap<User>::HashMap;

    /**
     * Retrieves the top 20 ratings from a user's ratings.
     *
     * @param user_id The ID of the user whose ratings are to be retrieved.
     * @return A vector containing the top 20 ratings from the user.
     */
    std::vector<Rating> top20_from_user(uint32_t user_id) {
        User* user_ptr = search(user_id);
        if (!user_ptr) {
            // User not found, returning empty vector
            return std::vector<Rating>();
        }

        // Sort the ratings vector in descending order
        sort_descending(user_ptr->ratings);

        // Return the first 20 elements of the vector
        auto start = user_ptr->ratings.begin();
        auto end = start + std::min<size_t>(20, user_ptr->ratings.size());
        std::vector<Rating> ratings(start, end);
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