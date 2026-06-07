#ifndef TRIE_HPP
#define TRIE_HPP

#include <stdlib.h>
#include <vector>
#include "Game.hpp"

const int ALPHABET_SIZE = 36; //letra e num

class TrieNode {

public:

    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfTitle;
    Game* game;

    TrieNode();
    ~TrieNode();
};

class Trie {

private:

    TrieNode* root;

public:

    Trie();
    ~Trie();

    int indexCalculator(char k); //aux
    bool insert(Game* game);
    bool contains(std::string title);
    bool comesBefore(Game* game1, Game* game2); //aux

    std::vector<Game*> autocomplete(std::string prefix, int k);

    std::string toSearchKey(std::string text);
    void sortResults(std::vector<Game*>& games);
};

#endif