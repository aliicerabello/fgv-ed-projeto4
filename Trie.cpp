#include "Trie.hpp"

//construtor
TrieNode::TrieNode(){
    isEndOfTitle = false;
    game = nullptr; //todo nó nasce sem jogo associado
    for (int i = 0; i < ALPHABET_SIZE; i++)
        children[i] = nullptr;
}

//destrutor
TrieNode::~TrieNode(){
    for (int i = 0; i < ALPHABET_SIZE; i++)
        delete children[i];
}

//construtor
Trie::Trie(){
    root = new TrieNode();
}

//destrutor
Trie::~Trie(){
    delete root;
}

int Trie::indexCalculator(char k){
    if ('a' <= k && k <= 'z')
        return k - 'a';
    
    if ('0' <= k && k <= '9')
        return 26 + ( k - '0');

    return -1; // talvez tornal isso um void?
}

bool Trie::insert(Game* game){
    if (game == nullptr)
        return false;

    std::string title = toSearchKey(game->getTitle());

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
            TrieNode* node = new TrieNode(); //cria um novo nó da trie
            curr->children[index_k] = node; // faz ele apontar p alguem novo
        }

        curr = curr->children[index_k]; //ja tem o char k
    }
    
    curr->isEndOfTitle = true; //fim de palavra
    curr->game = game;

    return true;
}


bool Trie::contains(std::string title){
    title = toSearchKey(title);
    
    //estamos supondo que o title ja ta ok
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
    
    return curr->isEndOfTitle; //se for fim de palavra, ta safe (pdoe ter casaco mas nao casa)
}

std::string Trie::toSearchKey(std::string text){
    //considerar que os títulos possuem apenas letras, números e espaços (enunciado)
    if (text.empty())
        return text;

    std::string text_new = "";

    for (char k : text){
        if (k == ' ') //pular o espaço
            continue;

        else if ('a' <=  k && k <= 'z') //minuscula
            text_new += k;

        else if ('A' <= k && k <= 'Z'){ //maiuscula
            k = k - 'A' + 'a'; //transformar em maiscula no seu respectivo lugar da tabela
            text_new += k;
        }

        else if ('0' <= k && k <= '9') //numero
            text_new += k;
    }
    return text_new;
}

bool Trie::comesBefore(Game* game1, Game* game2){
    int pop1 = game1->getPopularity();
    int pop2 = game2->getPopularity();
    
    if (pop1 > pop2)
        return true;

    else if (pop2 > pop1)
        return false;

    else {
        std::string title1 = game1->getTitle();
        std::string title2 = game2->getTitle();
        
        title1 = toSearchKey(title1);
        title2 = toSearchKey(title2);
    
        for (int k = 0; k < title1.size() && k < title2.size(); k++){
            if (title1[k] > title2[k])
                return false;
            else if (title1[k] < title2[k])
                return true;
        }
        return title1.size() < title2.size(); //casa < casaco
    }
}


void Trie::sortResults(std::vector<Game*>& games){ //usando o insertion
    //implementar o
}