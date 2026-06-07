#include <iostream>
#include <cstdlib>
#include <vector>

#include "Game.hpp"
#include "Trie.hpp"
#include "GamesDatabase.hpp"
using namespace std;

int main(int argc, char* argv[]) {

    Trie trie;

    trie.insert(&games[0]);
    trie.insert(&games[1]);
    trie.insert(&games[2]);

    cout << trie.contains("Counter Strike Global Offensive") << endl;
    cout << trie.contains("counterstrikeglobaloffensive") << endl;
    cout << trie.contains("COUNTER STRIKE GLOBAL OFFENSIVE") << endl;

    vector<Game*> teste;

    teste.push_back(&games[1]);
    teste.push_back(&games[0]);
    teste.push_back(&games[2]);

    trie.sortResults(teste);

    for (Game* g : teste){
        cout << g->getTitle() << " | " 
                << g->getPopularity() << endl;
    }

    Trie trie;

    Game jogo1("Halo", "FPS futurista", 90);
    Game jogo2("Half Life", "FPS classico", 90);
    Game jogo3("Hades", "Roguelike", 95);

    vector<Game*> teste;

    teste.push_back(&jogo1);
    teste.push_back(&jogo2);
    teste.push_back(&jogo3);

    trie.sortResults(teste);

    for (Game* g : teste){
        std::cout << g->getTitle() << " | "
                << g->getPopularity() << std::endl;
    }

    return 0;
}
