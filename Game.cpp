#include "Game.hpp"
#include "Trie.hpp"

Game::Game(){

}

Game::Game(std::string title_, std::string shortDescription_, int popularity_){
    this->title = title_;
    this->shortDescription = shortDescription_;
    this->popularity = popularity_;
}

Game::~Game(){

}
int Game::getPopularity(){
    return popularity;

}

std::string Game::getTitle(){
    return title;
}

std::string Game::getShortDescription(){
    return shortDescription;
}