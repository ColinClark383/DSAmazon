#pragma once
#include <string>


// hey just so you know I changed all attributes to string type cause it was easier

//container for all data needed for products
//result of seach can be represented as a vector of products
struct product {
    //the full name of a product
    std::string name = "";
    //the price of a product
    std::string price = "";
    //the number of stars the product has, from 0-5
    std::string rating = "";
    //the ammount of items bought in the past month
    std::string bought = "";
    //the url to go to a product
    std::string productURL = "";
    //the url to an image of the product
    std::string imageURL = "";
};