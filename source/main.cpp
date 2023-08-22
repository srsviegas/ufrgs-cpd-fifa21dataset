#include <iostream>
#include <time.h>
#include <sstream>
#include <string>
#include <iomanip>
#include "trie.h"
#include "hashmap.h"
#include "playerhashmap.h"
#include "taghashmap.h"
#include "ratinghashmap.h"
#include "positionhashmap.h"

void build_structures(
    PlayerNameTrie& player_names,
    PlayerHashMap& players,
    TagHashMap& tags,
    RatingHashMap& ratings,
    PositionHashMap& positions);

void start_console(
    PlayerNameTrie player_names,
    PlayerHashMap players,
    TagHashMap tags,
    RatingHashMap ratings,
    PositionHashMap positions);

std::string parse_command(std::string line, std::vector<std::string>& arguments);

template <typename T>
void printw(T object, size_t width);

std::string positions_to_str(std::vector<std::string> positions);

int main() {
    PlayerNameTrie player_names;
    PlayerHashMap players(12007);
    TagHashMap tags(809);
    RatingHashMap ratings(180043);
    PositionHashMap positions(41);

    build_structures(player_names, players, tags, ratings, positions);
    start_console(player_names, players, tags, ratings, positions);

    return 0;
}

const std::string line(100, '=');

/**
 * Builds the necessary data structures for console mode by reading data from CSV files.
 *
 * @param player_names A reference to the PlayerNameTrie object.
 * @param player A reference to the PlayerHashMap object.
 * @param tags A reference to the TagHashMap object.
 * @param ratings A reference to the RatingHashMap object.
 * @param positions A reference to the PositionsHashMap object.
 */
void build_structures(
    PlayerNameTrie& player_names,
    PlayerHashMap& players,
    TagHashMap& tags,
    RatingHashMap& ratings,
    PositionHashMap& positions
) {
    std::cout << "\n" << line << "\n"
        << "Reading CSV Files And Building Data Structures\n"
        << line << "\n";

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

    positions.load_players(players);
    clock_t end_poshash = clock();
    std::cout << "[-] Players loaded into the Positions Hash Map in "
        << double(end_poshash - end_ratings) / double(CLOCKS_PER_SEC)
        << " seconds." << std::endl;
    std::cout << "    Occupancy rate of " << positions.get_occupancy() * 100
        << "%." << std::endl;

    tags.from_csv("data/tags.csv");
    clock_t end_thash = clock();
    std::cout << "[-] Tag Hash Map initialization completed in "
        << double(end_thash - end_poshash) / double(CLOCKS_PER_SEC)
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
 * @param player_names The PlayerNameTrie object, containing the player names with their user ID`s.
 * @param player The PlayerHashMap object, containing player information.
 * @param tags The TagHashMap object, containing the tags and the IDs of the players that have them.
 * @param ratings The RatingHashMap object, containing the ratings given by each user.
 * @param positions The PositionsHashMap object, containing the positions and players that have them.
 */
void start_console(
    PlayerNameTrie player_names,
    PlayerHashMap players,
    TagHashMap tags,
    RatingHashMap ratings,
    PositionHashMap positions
) {
    std::cout << "\n" << line << "\n"
        << "Starting Console Mode\n"
        << line << "\n";

    while (true) {
        std::string command;
        std::vector<std::string> arguments;

        std::cout << "$ ";
        std::getline(std::cin, command);
        command = parse_command(command, arguments);

        if (command.empty()) {
            std::cout << "[X] No command was provided.\n";
            continue;
        }
        else if (arguments.empty()) {
            if (command == "exit") {
                return;
            }
            std::cout << "[X] No arguments were provided.\n";
            continue;
        }

        std::cout << "\n";

        if (command == "player") {
            const std::vector<std::string> headers = { "sofifa_id", "name", "player_positions", "rating", "count" };
            const std::vector<size_t> w = { 12, 50, 20, 10, 10 };
            for (size_t i = 0; i < headers.size(); i++) {
                printw(headers[i], w[i]);
            }
            std::cout << "\n";
            for (auto& id : player_names.search(arguments[0])) {
                Player player = *(players.search(id));
                std::string pos = positions_to_str(player.positions);
                printw(player.id, w[0]);
                printw(player.name, w[1]);
                printw(pos, w[2]);
                printw(player.global_rating, w[3]);
                printw(player.rating_count, w[4]);
                std::cout << "\n";
            }
        }
        else if (command == "user") {
            const std::vector<std::string> headers = { "sofifa_id", "name", "global_rating", "count", "rating" };
            const std::vector<size_t> w = { 12, 50, 18, 10, 10 };
            for (size_t i = 0; i < headers.size(); i++) {
                printw(headers[i], w[i]);
            }
            std::cout << "\n";
            for (auto& rating : ratings.top20_from_user(std::stoull(arguments[0]))) {
                Player player = *(players.search(rating.player_id));
                printw(player.id, w[0]);
                printw(player.name, w[1]);
                printw(player.global_rating, w[2]);
                printw(player.rating_count, w[3]);
                printw(rating.score, w[4]);
                std::cout << "\n";
            }
        }
        else if (command.rfind("top", 0) == 0) {
            size_t n = stoull(command.substr(3));
            const std::vector<std::string> headers = { "#", "sofifa_id", "name", "player_positions", "rating", "count" };
            const std::vector<size_t> w = { 5, 12, 50, 19, 10, 10 };
            for (size_t i = 0; i < headers.size(); i++) {
                printw(headers[i], w[i]);
            }
            std::cout << "\n";
            size_t i = 1;
            for (auto& id : positions.topn(n, arguments[0])) {
                Player player = *(players.search(id));
                std::string pos = positions_to_str(player.positions);
                printw(i++, w[0]);
                printw(player.id, w[1]);
                printw(player.name, w[2]);
                printw(pos, w[3]);
                printw(player.global_rating, w[4]);
                printw(player.rating_count, w[5]);
                std::cout << "\n";
            }
        }
        else if (command == "tags") {
            const std::vector<std::string> headers = { "sofifa_id", "name", "player_positions", "rating", "count" };
            const std::vector<size_t> w = { 12, 50, 19, 10, 10 };
            for (size_t i = 0; i < headers.size(); i++) {
                printw(headers[i], w[i]);
            }
            std::cout << "\n";
            for (auto& id : tags.search_tags(arguments)) {
                Player player = *(players.search(id));
                std::string pos = positions_to_str(player.positions);
                printw(player.id, w[0]);
                printw(player.name, w[1]);
                printw(pos, w[2]);
                printw(player.global_rating, w[3]);
                printw(player.rating_count, w[4]);
                std::cout << "\n";
            }
        }
        else {
            std::cout << "[X] Invalid command.\n";
        }

        std::cout << "\n";
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

template <typename T>
void printw(T object, size_t width) {
    std::cout << std::left << std::setw(width) << object;
}

std::string positions_to_str(std::vector<std::string> positions) {
    std::stringstream ss;
    ss << "\"";
    for (size_t i = 0; i < positions.size(); ++i) {
        ss << positions[i];
        if (i < positions.size() - 1) {
            ss << ", ";
        }
    }
    ss << "\"";
    return ss.str();
}