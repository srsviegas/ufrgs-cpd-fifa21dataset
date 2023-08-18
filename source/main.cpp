#include <iostream>
#include <time.h>
#include <sstream>
#include <string>
#include "trie.h"
#include "hashmap.h"
#include "playerhashmap.h"
#include "taghashmap.h"
#include "ratinghashmap.h"

void build_structures(
    PlayerNameTrie& player_names,
    PlayerHashMap& players,
    TagHashMap& tags,
    RatingHashMap& ratings);

void start_console(
    PlayerNameTrie player_names,
    PlayerHashMap players,
    TagHashMap tags,
    RatingHashMap ratings);

std::string parse_command(std::string line, std::vector<std::string>& arguments);

int main() {
    PlayerNameTrie player_names;
    PlayerHashMap players(12007);
    TagHashMap tags(809);
    RatingHashMap ratings(180043);

    build_structures(player_names, players, tags, ratings);
    start_console(player_names, players, tags, ratings);

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
    TagHashMap& tags,
    RatingHashMap& ratings
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

    ratings.from_csv("data/rating.csv");
    clock_t end_rhash = clock();
    std::cout << "[-] Ratings Hash Map initialization completed in "
        << double(end_rhash - end_phash) / double(CLOCKS_PER_SEC)
        << " seconds." << std::endl;
    std::cout << "    Occupancy rate of " << ratings.get_occupancy() * 100
        << "%." << std::endl;

    players.load_ratings(ratings);
    clock_t end_ratings = clock();
    std::cout << "[-] Ratings loaded into the Players Hash Map in "
        << double(end_ratings - end_rhash) / double(CLOCKS_PER_SEC)
        << " seconds." << std::endl;

    tags.from_csv("data/tags.csv");
    clock_t end_thash = clock();
    std::cout << "[-] Tag Hash Map initialization completed in "
        << double(end_thash - end_ratings) / double(CLOCKS_PER_SEC)
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
    TagHashMap tags,
    RatingHashMap ratings
) {
    std::cout << "\n================================================================\n"
        << "Starting Console Mode\n"
        << "================================================================\n";

    while (true) {
        std::string command;
        std::vector<std::string> arguments;

        std::cout << "$ ";
        std::getline(std::cin, command);
        command = parse_command(command, arguments);

        if (command == "player") {
            std::cout << "sofifa_id,name,player_positions,rating,count \n";
            for (auto& id : player_names.search(arguments[0])) {
                Player player = *(players.search(id));
                std::cout << player.id << "," << player.name << ",\"";
                for (auto& position : player.positions) {
                    std::cout << position << " ";
                }
                std::cout << "\"" << static_cast<double>(player.global_rating)
                    / static_cast<double>(player.rating_count) << ","
                    << player.rating_count << std::endl;
            }
        }
        else if (command == "user") {
            std::cout << "sofifa_id,name,global_rating,count,rating \n";
            for (auto& rating : ratings.top20_from_user(std::stoull(arguments[0]))) {
                Player player = *(players.search(rating.player_id));
                std::cout << player.id << ","
                    << player.name << ","
                    << player.global_rating << ","
                    << player.rating_count << ","
                    << rating.score << std::endl;
            }
        }
        else if (command == "tags") {
            std::cout << "sofifa_id,name,player_positions,rating,count \n";
            for (auto& id : tags.search_tags(arguments)) {
                Player player = *(players.search(id));
                std::cout << player.id << "," << player.name << ",\"";
                for (auto& position : player.positions) {
                    std::cout << position << " ";
                }
                std::cout << "\"" << static_cast<double>(player.global_rating)
                    / static_cast<double>(player.rating_count) << ","
                    << player.rating_count << std::endl;
            }
        }
    }
}

/**
 * Parses a command line and extracts the command and its arguments.
 *
 * @param line The command line input to be parsed.
 * @param arguments A reference to a vector where the parsed arguments will be stored.
 * @return The extracted command from the command line.
 */
std::string parse_command(std::string line, std::vector<std::string>& arguments) {
    std::string command;
    std::stringstream ss(line);

    std::getline(ss, command, ' ');
    if (command.back() == ' ') {
        // Remove trailing space
        command.pop_back();
    }

    std::string argument;
    while (std::getline(ss, argument, '\'')) {
        // Ignore if argument is empty
        if (argument != "" && argument != " ") {
            arguments.push_back(argument);
        }
    }

    return command;
}