#pragma once
#include <string>

//container for all data needed for products
//result of seach can be represented as a vector of products
struct product{
    //the full name of a product
    std::string name = "";
    //the price of a product
    float price = 0.0;
    //the number of stars the product has, from 0-5
    float rating = 0.0;
    //the ammount of items bought in the past month
    int bought = 0;
    //the url to go to a product
    std::string productURL = "";
    //the url to an image of the product
    std::string imageURL = "";
};