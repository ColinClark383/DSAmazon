#include "hash.h"
#include <cmath>

HashTable::HashTable(int capacity)
{
    this->capacity = capacity;
    container.resize(capacity);
    size = 0;
}

void HashTable::insertProd(product product)
{
    if (size >= (capacity * maxLoadFactor))
        return;

    int index = hashFunc(product.name);
    auto &bucket = container[index];
    for (int i = 0; i < bucket.size(); i++)
        if (bucket[i].name == product.name)
            return;
    bucket.push_back(product);
    ++size;
}

long HashTable::hashFunc(std::string name)
{
    long index = 0;
    for (int i = 0; i < name.size(); i++)
        index += name[i] * pow(31, i);
    return name.size() % capacity;
}

std::vector<product> HashTable::getKeyword(std::string keyword)
{
    std::vector<product> matches;
    int index = hashFunc(keyword);
    auto &bucket = container[index];
    for (int i = 0; i < bucket.size(); i++)
        if (bucket[i].name == keyword)
            matches.push_back(bucket[i]);

    return matches;
}