#include "hash.h"
#include <cmath>
#include <iostream>

HashTable::HashTable(const std::vector<product> &prod)
{
    // initialize table with capacity and insert values from data sheet
    container.resize(capacity);
    for (int i = 0; i < prod.size(); i++)
        insertProd(prod[i]);
}

void HashTable::reHash()
{
    capacity = capacity * 2;
    std::vector< std::vector< product>> newContainer(capacity);

    // Rehash all existing elements into the new table
    for (long i = 0; i < container.size(); ++i)
    {
        for (long j = 0; j < container[i].size(); ++j)
         {
            long newIndex = hashFunc(container[i][j].name);
            newContainer[newIndex].push_back(container[i][j]);
         }
    }

    // Update the hash table with the new one
    container = std::move(newContainer);
}

void HashTable::insertProd(product product)
{
    // rehash if load factor is exceeded
    if (size >= (capacity * maxLoadFactor))
        reHash();

    size_t index = hashFunc(product.name);
    auto &bucket = container[index];
    // find corresponding bucket and add new product if not already in bucket
    for (int i = 0; i < bucket.size(); i++)
        if (bucket[i].name == product.name)
            return;
    bucket.push_back(product);
    ++size;
}

size_t HashTable::hashFunc(std::string name)
{
    // takes characters of string and multiples the chars by ascending powers of 31 for hash 
    size_t index = 0;
    for (int i = 0; i < name.size(); i++)
        index += name[i] * pow(31, i);
    return index % capacity;
}

std::vector<product> HashTable::getKeyword(std::string keyword)
{
    // searches through corresponding bucket for the given products name to find matches
    std::vector<product> matches;
    int index = hashFunc(keyword);
    auto &bucket = container[index];
    for (int i = 0; i < bucket.size(); i++)
        if (bucket[i].name == keyword)
            matches.push_back(bucket[i]);

    return matches;
}