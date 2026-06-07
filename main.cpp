#include <iostream>
#include <cstdlib>
#include <vector>

#include "Game.hpp"
#include "Trie.hpp"
#include "GamesDatabase.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: ./app k prefixo" << endl;
        return 1;
    }

    int k = atoi(argv[1]);
    string prefix = argv[2];

    Trie trie;

    for (int i = 0; i < numberOfGames; i++) {
        trie.insert(&games[i]);
    }

    vector<Game*> results = trie.autocomplete(prefix, k);

    if (results.empty()) {
        cout << "No results found" << endl;
        return 0;
    }

    for (Game* game : results) {
        cout << game->getTitle() << " | "
                  << game->getShortDescription() << " | "
                  << game->getPopularity() << endl;
    }

    return 0;
}