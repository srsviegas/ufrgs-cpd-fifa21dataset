#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>

template <class Item, class Key = uint32_t>
class HashMap {
protected:
    uint32_t table_size;

private:
    virtual uint32_t hash(Key key) = 0;
    virtual bool equal(Item item, Key key) = 0;

public:
    std::vector<Item>* table;

    /**
     * Constructs a HashMap instance with a specified table size.
     *
     * @param tsize The initial size of the hash table.
     */
    HashMap(uint32_t tsize) {
        table_size = tsize;
        table = new std::vector<Item>[table_size];
    }

    /**
     * Inserts an item into the hash map.
     *
     * @param key The key associated with the item (32-bit uint).
     * @param item The item to insert into the hash map.
     */
    void insert(Key key, Item item) {
        uint32_t hash_value = hash(key);
        table[hash_value].push_back(item);
    }

    /**
     * Searches for an item in the hash map based on the key.
     *
     * @param key The key associated with the item to search for.
     * @return A pointer to the found item, or nullptr if not found.
     */
    Item* search(Key key) {
        uint32_t hash_value = hash(key);
        for (auto& item : table[hash_value]) {
            if (equal(item, key)) {
                return &item;
            }
        }
        return nullptr;
    }

    float get_occupancy() {
        uint32_t occupied = 0;
        for (uint32_t i = 0; i < table_size; i++) {
            if (!(table[i].empty())) {
                occupied++;
            }
        }
        return static_cast<float>(occupied) / table_size;
    }
};

#endif // HASH_H