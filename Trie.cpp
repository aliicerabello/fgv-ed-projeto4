#include "Trie.hpp"
using namespace std;

TrieNode::TrieNode(){
    isEndOfTitle = false;
    game = nullptr;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        children[i] = nullptr;
}

TrieNode::~TrieNode(){
    for (int i = 0; i < ALPHABET_SIZE; i++)
        delete children[i];
}

Trie::Trie(){
    root = new TrieNode();
}

Trie::~Trie(){
    delete root;
}

int Trie::indexCalculator(char k){
    if ('a' <= k && k <= 'z')
        return k - 'a';
    
    if ('0' <= k && k <= '9')
        return 26 + ( k - '0');

    return -1;
}

bool Trie::insert(Game* game){
    if (game == nullptr)
        return false;

    string title = toSearchKey(game->getTitle());

    if (title.empty())
        return false;

    if (contains(title))
        return false;

    TrieNode* curr = root;

    for (char k : title){
        int index_k = indexCalculator(k);

        if (index_k == -1)
            return false;

        if (curr->children[index_k] == nullptr){
            TrieNode* node = new TrieNode();
            curr->children[index_k] = node;
        }

        curr = curr->children[index_k]; 
    }
    
    curr->isEndOfTitle = true;
    curr->game = game;

    return true;
}


bool Trie::contains(string title){
    title = toSearchKey(title);

    if (title.empty())
        return false;

    TrieNode* curr = root;

    for (char k : title){
        int index_k = indexCalculator(k);

        if (index_k == -1)
            return false; 

        if (curr->children[index_k] == nullptr)
            return false;

        curr = curr->children[index_k];
    }

    return curr->isEndOfTitle;
}

string Trie::toSearchKey(string text){
    if (text.empty())
        return text;

    string text_new = "";

    for (char k : text){
        if (k == ' ')
            continue;

        else if ('a' <=  k && k <= 'z')
            text_new += k;

        else if ('A' <= k && k <= 'Z'){ 
            k = k - 'A' + 'a';  
            text_new +=k;
        }

        else if ('0' <= k && k <= '9')
            text_new += k;
    }
    return text_new;
}

bool Trie::comesBefore(Game* game1, Game* game2){
    int pop1 = game1->getPopularity();
    int pop2 = game2->getPopularity();
    
    if (pop1 > pop2)
        return true;

    if (pop2 > pop1)
        return false;

    string title1 = toSearchKey(game1->getTitle());
    string title2 = toSearchKey(game2->getTitle());

    return title1 < title2;
}


void Trie::sortResults(vector<Game*>& games){
    for (vector<Game*>::size_type i = 1; i < games.size(); i++){
        Game* temp = games[i];

        vector<Game*>::size_type j = i;

        while (j > 0 && comesBefore(temp, games[j - 1])){
            games[j] = games[j - 1];
            j--;
        }

        games[j] = temp;
    }
}

void Trie::collectGames(TrieNode* node, vector<Game*>& results, int k){
    if (node == nullptr)
        return;

    if (node->isEndOfTitle && node->game != nullptr){
        results.push_back(node->game);

        sortResults(results);

        if (results.size() > static_cast<vector<Game*>::size_type>(k))
            results.pop_back();
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
        collectGames(node->children[i], results, k);
}

vector<Game*> Trie::autocomplete(string prefix, int k){
    vector<Game*> results;

    if (k <= 0)
        return results;
    
    prefix = toSearchKey(prefix);

    if (prefix.empty())
        return results;

    TrieNode* curr = root;

    for (char c : prefix){
        int index = indexCalculator(c);

        if (index == -1)
            return results;

        if (curr->children[index] == nullptr)
            return results;

        curr = curr->children[index];
    }

    collectGames(curr, results, k);

    return results;
}