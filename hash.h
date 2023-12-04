#pragma once
#include "product.h"
#include <vector>

class HashTable
{
    int capacity =1048575 * 1.5;
    int size = 0;
    int maxLoadFactor = 1.5;

    // hash table of products
    std::vector< std::vector< product>> container;

    // remake table in case load factor is exceeded to maintain speed
    void reHash();

    // inserts product into hash table
    void insertProd(product product);

    // hashing function to assign index
    long hashFunc(std::string name);

public:
    // constructor for hash table
    HashTable(const std::vector<product> &prod);

    // gets a vector of all products from a certain keyword
    std::vector<product> getKeyword(std::string keyword);

};