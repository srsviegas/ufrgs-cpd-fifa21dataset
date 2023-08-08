#include <iostream>
#include <time.h>
#include "parser.h"
#include "trie.h"

void build_structures(PlayerNameTrie& player_names);
void start_console();


int main() {
    PlayerNameTrie player_names;

    build_structures(player_names);

    return 0;
}


/**
 * Builds the necessary data structures for console mode by reading data from CSV files.
 *
 * @param player_names A reference to the PlayerNameTrie object.
 */
void build_structures(PlayerNameTrie& player_names) {
    clock_t start = clock();
    player_names.from_csv("../data/players.csv");
    std::cout << "[-] Player names trie initialization completed in "
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
void start_console() {
    // @todo: Implement console mode
    return;
}