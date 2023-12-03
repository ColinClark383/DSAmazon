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
    trie();

    //constructor with vector of products
    //products from vector are inserted into trie
    trie(std::vector<product> &products);
    //destructor
    //deletes nodes in a postorder fashion
    ~trie();

    //gets a vector of all products from a certian keyword
    std::vector<product> getKeyword(std::string keyword);

    //inserts a product into the trie
    void insert(std::string keyword, product prod);
};