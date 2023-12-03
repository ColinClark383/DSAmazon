#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

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

// excel file gives quote to name sometimes randomly so this removes it
std::string quoteRemove(std::string name)
{
	std::string res = "";
	for (int i = 0; i < name.size(); i++)
	{
		if (name[i] != '\"')
			res += name[i];
	}
	return res;
}

// returns vector of all products from excel data sheet
std::vector<product> readProducts()
{
	std::vector<product> data;  // To store CSV data
	std::ifstream file("amazon_products.txt");

	if (!file.is_open()) {
		std::cout << "Error opening file." << std::endl;
		return data;
	}

	bool start = true;
	int test = 0; //keep this if you only want to load part of the data
	std::string line;
	while (std::getline(file, line)) {
		std::vector<std::string> row;
		std::stringstream ss(line);
		std::string cell;

		if (start)
		{
			start = false;
			continue;
		}

		int count = 1;
		product temp;
		while (std::getline(ss, cell, '\t'))
		{
			if (count == 1)
				count = 1; 
			else if (count == 2)
				temp.name = quoteRemove(cell);
			else if (count == 3)
				temp.imageURL = cell;
			else if (count == 4)
				temp.productURL = cell;
			else if (count == 5)
				temp.rating = cell;
			else if (count == 7)
				temp.price = cell;
			else if (count == 11)
				temp.bought = cell;

			++count;
		}
		data.push_back(temp);
		++test;
		if (test == 50) // only loads 50 products for testing purposes
			break;
	}

	file.close();

	return data;
}