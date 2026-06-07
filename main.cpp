#include <iostream>
#include <cstdlib>
#include <vector>

#include "Game.hpp"
#include "Trie.hpp"
#include "GamesDatabase.hpp"
using namespace std;

int main() {
    Trie trie;

    for (int i = 0; i < numberOfGames; i++)
        trie.insert(&games[i]);

    vector<Game*> results = trie.autocomplete("ha", 5);

    for (Game* g : results){
        std::cout << g->getTitle() << " | "
                << g->getPopularity() << std::endl;
    }

    return 0;
}
