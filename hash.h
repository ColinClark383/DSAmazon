#pragma once
#include "product.h"
#include <vector>

class HashTable
{
    std::vector<product> data;

    //inserts product into hash table
    void insertProd(product product);

    //hashing function to assign index
    size_t hashFunc(product product);
    
public:

    //gets a vector of all products from a certain keyword
    std::vector<product> getKeyword(std::string keyword);
};