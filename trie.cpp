#include "trie.h"
#include <queue>
#include <sstream>

trie::trie(){
    root = new trieNode;
}

trie::trie(std::vector<product> &products){
    root = new trieNode;
    for(int i = 0; i < products.size(); i++){
        std::stringstream ss(products[i].name);
        std::string word;
        while(ss >> word){
            insert(word, products[i]);
        }
    }
}

trie::~trie(){
    std::queue<trieNode*> q;
    q.push(root);
    while(!q.empty()){
        if(!q.front()->children.empty()){
            for(auto i = q.front()->children.begin(); i != q.front()->children.end(); i++){
                q.emplace(i->second);
            }
        }
        delete q.front();
        q.pop();
    }
}

void trie::insert(std::string keyword, product prod){
    trieNode* node = root;
    for(int i = 0; i < keyword.length(); i++){
        if(isalpha(keyword[i])){
            if(!node->children.count((char)tolower(keyword[i]))){
                node->children[(char)tolower(keyword[i])] = new trieNode;
            }
            node = node->children[(char)tolower(keyword[i])];
        }
    }
    node->products.push_back(prod);
}

std::vector<product> trie::getKeyword(std::string keyword){
    trieNode* node = root;
    for(int i = 0; i < keyword.length(); i++){
        if(isalpha(keyword[i])){
            if(!node->children.count((char)tolower(keyword[i]))){
                node->children[(char)tolower(keyword[i])] = new trieNode;
            }
            node = node->children[(char)tolower(keyword[i])];
        }
    }
    return node->products;
}