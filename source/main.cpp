#include <iostream>
#include <time.h>
#include "parser.h"
#include "trie.h"
#include "hashmap.h"
#include "playerhashmap.h"

void build_structures(PlayerNameTrie& player_names, PlayerHashMap& players);
void start_console(PlayerNameTrie player_names, PlayerHashMap players);


int main() {
    PlayerNameTrie player_names;
    PlayerHashMap players(1000);

    build_structures(player_names, players);
    start_console(player_names, players);

    return 0;
}


/**
 * Builds the necessary data structures for console mode by reading data from CSV files.
 *
 * @param player_names A reference to the PlayerNameTrie object.
 */
void build_structures(PlayerNameTrie& player_names, PlayerHashMap& players) {
    std::cout << "\n================================================================\n"
        << "Reading CSV Files And Building Data Structures\n"
        << "================================================================\n";

    clock_t start = clock();

    player_names.from_csv("../data/players.csv");
    clock_t end_trie = clock();
    std::cout << "[-] Player names trie initialization completed in "
        << double(end_trie - start) / double(CLOCKS_PER_SEC)
        << " seconds." << std::endl;

    players.from_csv("../data/players.csv");
    std::cout << "[-] Player hash initialization completed in "
        << double(clock() - end_trie) / double(CLOCKS_PER_SEC)
        << " seconds." << std::endl;

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
void start_console(PlayerNameTrie player_names, PlayerHashMap players) {
    std::cout << "\n================================================================\n"
        << "Starting Console Mode\n"
        << "================================================================\n";

    while (true) {
        std::string input;
        std::cout << "$ ";
        std::getline(std::cin, input);

        // placeholder
        for (auto& id : player_names.search(input)) {
            std::cout << players.search(id)->name << std::endl;
        }
    }
}