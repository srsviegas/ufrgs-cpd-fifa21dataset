#include <iostream>
#include <time.h>
#include "parser.h"
#include "trie.h"
#include "hashmap.h"
#include "playerhashmap.h"
#include "taghashmap.h"

void build_structures(PlayerNameTrie& player_names, PlayerHashMap& players, TagHashMap& tags);
void start_console(PlayerNameTrie player_names, PlayerHashMap players, TagHashMap tags);


int main() {
    PlayerNameTrie player_names;
    PlayerHashMap players(12007);
    TagHashMap tags(809);

    build_structures(player_names, players, tags);
    start_console(player_names, players, tags);

    return 0;
}


/**
 * Builds the necessary data structures for console mode by reading data from CSV files.
 *
 * @param player_names A reference to the PlayerNameTrie object.
 */
void build_structures(
    PlayerNameTrie& player_names,
    PlayerHashMap& players,
    TagHashMap& tags
) {
    std::cout << "\n================================================================\n"
        << "Reading CSV Files And Building Data Structures\n"
        << "================================================================\n";

    clock_t start = clock();

    player_names.from_csv("data/players.csv");
    clock_t end_trie = clock();
    std::cout << "[-] Player Names Trie initialization completed in "
        << double(end_trie - start) / double(CLOCKS_PER_SEC)
        << " seconds." << std::endl;

    players.from_csv("data/players.csv");
    clock_t end_phash = clock();
    std::cout << "[-] Player Hash Map initialization completed in "
        << double(end_phash - end_trie) / double(CLOCKS_PER_SEC)
        << " seconds." << std::endl;
    std::cout << "    Occupancy rate of " << players.get_occupancy() * 100
        << "%." << std::endl;

    tags.from_csv("data/tags.csv");
    clock_t end_thash = clock();
    std::cout << "[-] Tag Hash Map initialization completed in "
        << double(end_thash - end_phash) / double(CLOCKS_PER_SEC)
        << " seconds." << std::endl;
    std::cout << "    Occupancy rate of " << tags.get_occupancy() * 100
        << "%." << std::endl;

    std::cout << "[-] Total time elapsed: "
        << double(clock() - start) / double(CLOCKS_PER_SEC)
        << " seconds." << std::endl;
}

/**
 * Initiates the console mode, allowing the user to execute various commands.
 * The available commands are:
 *   - player <name|prefix>
 *   - user <userID>
 *   - top<n> <position>
 *   - tags <list of tags>
 *   - exit
 */
void start_console(
    PlayerNameTrie player_names,
    PlayerHashMap players,
    TagHashMap tags
) {
    std::cout << "\n================================================================\n"
        << "Starting Console Mode\n"
        << "================================================================\n";

    while (true) {
        std::string input;
        std::cout << "$ ";
        std::getline(std::cin, input);

        // placeholder (search players by prefix)
        for (auto& id : player_names.search(input)) {
            Player player = *(players.search(id));
            std::cout << player.id << "," << player.name << ",";
            for (auto& position : player.positions) {
                std::cout << position << " ";
            }
            std::cout << std::endl;
        }
    }
}