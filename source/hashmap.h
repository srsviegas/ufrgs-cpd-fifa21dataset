#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>

template <class T>
class HashMap {
private:
    uint16_t table_size;

    /**
     * Calculates a hash value for the given key.
     *
     * @param key The key for which to calculate the hash value (32-bit uint).
     * @return The calculated hash value (16-bit uint).
     */
    uint16_t hash(uint32_t key) {
        return key % table_size;
    }

public:
    std::vector<T>* table;

    /**
     * Constructs a HashMap instance with a specified table size.
     *
     * @param tsize The initial size of the hash table.
     */
    HashMap(uint16_t tsize) {
        table_size = tsize;
        table = new std::vector<T>[table_size];
    }

    /**
     * Inserts an item into the hash map.
     *
     * @param key The key associated with the item (32-bit uint).
     * @param item The item to insert into the hash map.
     */
    void insert(uint32_t key, T item) {
        uint16_t hash_value = hash(key);
        table[hash_value].push_back(item);
    }

    /**
     * Searches for an item in the hash map based on the key.
     *
     * @param key The key associated with the item to search for.
     * @return A pointer to the found item, or nullptr if not found.
     */
    T* search(uint32_t key) {
        uint16_t hash_value = hash(key);
        for (auto& item : table[hash_value]) {
            if (item == key) {
                return &item;
            }
        }
        return nullptr;
    }
};

#endif // HASH_H