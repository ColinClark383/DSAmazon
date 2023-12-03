#include "trie.h"
#include "hash.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>



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






int main()
{

    std::cout << "pretend that this is the project" << std::endl;
    std::cout << "sounds good" << std::endl;
    std::cout << "peepeepoopoo" << std::endl;

    std::cout << "testing trie object without loading in csv" << std::endl;
    //trie test;

    /*
    product airpods;
    airpods.name = "apple airpods";
    airpods.price = "99.99";
    airpods.rating = "4.8";
    airpods.bought = "1500";
    airpods.imageURL = "https://m.media-amazon.com/images/I/61sxnqv5FdL._AC_UL320_.jpg";
    airpods.productURL = "https://www.amazon.com/dp/B0B4NQBZ4Q";
    test.insert("airpod", airpods);
    std::vector<product> p = test.getKeyword("airpod");
    product p1 = p[0];
    std::cout << p1.name << " " << p1.price << std::endl;
    std::cout << p1.rating << " " << p1.bought << std::endl;
    std::cout << p1.productURL << std::endl;
    std::cout << p1.imageURL << std::endl;
    */

// SFML Shee Begins
    // Create variables to be used
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Project_3", sf::Style::Close);
    int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;
    int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;

    
    sf::RectangleShape SEARCH_MODE_SELECT_BOX(sf::Vector2f(WINDOW_WIDTH/4.67, WINDOW_HEIGHT/2.67));
    SEARCH_MODE_SELECT_BOX.setFillColor(sf::Color::Transparent);
    SEARCH_MODE_SELECT_BOX.setOutlineColor(sf::Color::White);
    SEARCH_MODE_SELECT_BOX.setOutlineThickness(2);
    SEARCH_MODE_SELECT_BOX.move(2,2);

    sf::RectangleShape SEARCH_BOX(sf::Vector2f(WINDOW_WIDTH/1.4, WINDOW_HEIGHT/12));
    SEARCH_BOX.setFillColor(sf::Color::Transparent);
    SEARCH_BOX.setOutlineColor(sf::Color::White);
    SEARCH_BOX.setOutlineThickness(2);
    SEARCH_BOX.setPosition((WINDOW_WIDTH/4.67)+4, WINDOW_HEIGHT/20);

    sf::RectangleShape GO_BOX(sf::Vector2f(WINDOW_HEIGHT/12, WINDOW_HEIGHT/12));
    GO_BOX.setFillColor(sf::Color::Transparent);
    GO_BOX.setOutlineColor(sf::Color::White);
    GO_BOX.setOutlineThickness(2);
    GO_BOX.setPosition((WINDOW_WIDTH/1.4)+(WINDOW_WIDTH/4.67)+6, WINDOW_HEIGHT/20);

    sf::RectangleShape RESULTS_MAIN_OUTLINE(sf::Vector2f((WINDOW_WIDTH/1.4)-2, WINDOW_HEIGHT/1.4));
    RESULTS_MAIN_OUTLINE.setFillColor(sf::Color::Transparent);
    RESULTS_MAIN_OUTLINE.setOutlineColor(sf::Color::White);
    RESULTS_MAIN_OUTLINE.setOutlineThickness(2);
    RESULTS_MAIN_OUTLINE.setPosition((WINDOW_WIDTH/4.67)+6, (WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12)+4);

    sf::RectangleShape PAGE_LEFT_BUTTON(sf::Vector2f(WINDOW_WIDTH/9.33, WINDOW_HEIGHT/12));
    PAGE_LEFT_BUTTON.setFillColor(sf::Color::Transparent);
    PAGE_LEFT_BUTTON.setOutlineColor(sf::Color::White);
    PAGE_LEFT_BUTTON.setOutlineThickness(2);
    PAGE_LEFT_BUTTON.setPosition((WINDOW_WIDTH/4.67)+6+(WINDOW_WIDTH/24), (WINDOW_HEIGHT/1.4)+(WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12)+8);
    
    sf::RectangleShape PAGE_RIGHT_BUTTON(sf::Vector2f(WINDOW_WIDTH/9.33, WINDOW_HEIGHT/12));
    PAGE_RIGHT_BUTTON.setFillColor(sf::Color::Transparent);
    PAGE_RIGHT_BUTTON.setOutlineColor(sf::Color::White);
    PAGE_RIGHT_BUTTON.setOutlineThickness(2);
    PAGE_RIGHT_BUTTON.setPosition((WINDOW_WIDTH/1.4)+(WINDOW_WIDTH/4.67)-(WINDOW_WIDTH/9.33)-(WINDOW_WIDTH/24)+6, (WINDOW_HEIGHT/1.4)+(WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12)+8);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.key.code == sf::Keyboard::Enter) {
                window.close();
            }
        }
        window.clear();
        window.draw(SEARCH_MODE_SELECT_BOX);
        window.draw(SEARCH_BOX);
        window.draw(GO_BOX);
        window.draw(RESULTS_MAIN_OUTLINE);
        window.draw(PAGE_LEFT_BUTTON);
        window.draw(PAGE_RIGHT_BUTTON);
        window.display();
    }
    return 0;
}
