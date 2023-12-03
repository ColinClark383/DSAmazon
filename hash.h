#pragma once
#include "product.h"

class HashTable
{
    int capacity;
    int size;
    int maxLoadFactor = 1.5;

    // hash table of products
    std::vector< std::vector< product>> container;

public:
    //inserts product into hash table
    void insertProd(product product);

    //hashing function to assign index
    long hashFunc(std::string name);

    std::vector<product> test;

//public:
    HashTable(int capacity);

    //gets a vector of all products from a certain keyword
    std::vector<product> getKeyword(std::string keyword);

};