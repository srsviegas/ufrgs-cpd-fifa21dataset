#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>
#include "parser.h"

template <class T>
class HashMap {
private:
    int table_size;

    /**
     * Calculates a hash value for the given key.
     *
     * @param key The key for which to calculate the hash value (32-bit uint).
     * @return The calculated hash value (16-bit uint).
     */
    uint16_t hash(uint32_t key) {
        return key % this->table_size;
    }

public:
    std::vector<T>* table;

    /**
     * Constructs a HashMap instance with a specified table size.
     *
     * @param table_size The initial size of the hash table.
     */
    HashMap(int table_size) {
        this->table_size = table_size;
        this->table = new std::vector<T>[table_size];
    }

    /**
     * Inserts an item into the hash map.
     *
     * @param key The key associated with the item (32-bit uint).
     * @param item The item to insert into the hash map.
     */
    void insert(uint32_t key, T item) {
        int hash = this->hash(key);
        this->table[hash].push_back(item);
    }

    /**
     * Searches for an item in the hash map based on the key.
     *
     * @param key The key associated with the item to search for.
     * @return A pointer to the found item, or nullptr if not found.
     */
    T* search(int key) {
        int hash = this->hash(key);
        for (auto& item : this->table[hash]) {
            if (item == key) {
                return &item;
            }
        }
        return nullptr;
    }
};

#endif // HASH_H