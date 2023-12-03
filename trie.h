#pragma once
#include "product.h"
#include <unordered_map>

struct trieNode{
    std::vector<product> products;
    std::unordered_map<char, trieNode*> children;
};
class trie{
    trieNode* root;

 

    public:
    //constructor
    //loads in all data from .csv file
    trie();
    //destructor
    //deletes nodes in a postorder fashion
    ~trie();

    //gets a vector of all products from a certian keyword
    std::vector<product> getKeyword(std::string keyword);

    //inserts a product into the trie
    void insert(std::string keyword, product prod);
};