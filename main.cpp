#include "trie.h"
#include "hash.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>



// excel file gives quote to name sometimes randomly so this removes it
std::string quoteRemove(std::string name)
{
	std::string res = "";
	for (int i = 0; i < name.size(); i++)
	{
		if (name[i] != '\"')
			res += tolower(name[i]);
	}
	return res;
}

// returns vector of all products from excel data sheet
std::vector<product> readProducts()
{
	std::vector<product> data;  // To store CSV data
	std::ifstream file("../amazon_products.txt");

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
		if (test == 25000) // only loads 50 products for testing purposes
			break;
	}

	file.close();

	return data;
}






int main()
{
    std::vector<product> products = readProducts();
    trie productTrie(products);
    HashTable productHash(products);

    // bool to determine if using trie or hash table
    // true = trie. false = hash table
    bool searchMode = true;
    bool drawLRButtons = false;

    //vector of products that match keyword
    std::vector<product> results;

    //string used for search keywords
    std::string displayKeyword = "";
    std::string keyword = "";


// SFML Shee Begins
    // Create variables to be used
    sf::Font FONT;
    std::chrono::microseconds duration;
    if (!FONT.loadFromFile("font.ttf")) std::cout << "Failed to open font" << std::endl;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Project_3", sf::Style::Close);
    int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;
    int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
    int currPage = 0;

    float RESULT_BOX_Xi = (WINDOW_WIDTH/1.4)-2;
    float RESULT_BOX_Xf = (WINDOW_WIDTH/4.67)+6;
    float RESULT_BOX_Yi = WINDOW_HEIGHT/1.4;
    float RESULT_BOX_Yf = (WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12)+4;
    
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

    sf::Text SEARCH_BOX_PROMPT("Enter Keyword(s):", FONT, 36);
	SEARCH_BOX_PROMPT.setFillColor(sf::Color::Cyan);
    SEARCH_BOX_PROMPT.setStyle(sf::Text::Bold);
	SEARCH_BOX_PROMPT.setPosition((WINDOW_WIDTH/4.67)+6, (((WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12))/2)-3);

    sf::RectangleShape GO_BOX(sf::Vector2f(WINDOW_HEIGHT/12, WINDOW_HEIGHT/12));
    GO_BOX.setFillColor(sf::Color::Transparent);
    GO_BOX.setOutlineColor(sf::Color::White);
    GO_BOX.setOutlineThickness(2);
    GO_BOX.setPosition((WINDOW_WIDTH/1.4)+(WINDOW_WIDTH/4.67)+6, WINDOW_HEIGHT/20);
    
    sf::Text GO_BOX_PROMPT("GO!", FONT, 32);
	GO_BOX_PROMPT.setFillColor(sf::Color::Cyan);
    GO_BOX_PROMPT.setStyle(sf::Text::Bold);
	GO_BOX_PROMPT.setPosition((WINDOW_WIDTH/1.4)+(WINDOW_WIDTH/4.67)+11, (WINDOW_HEIGHT/20)+10);

    sf::Text PROJECT_DESCRIPTION("Amazon\nSearcher!\n\nMade by:\nAddison Ferrell\nAugust Gould\nColin Clark", FONT, 24);
	PROJECT_DESCRIPTION.setFillColor(sf::Color::Cyan);
    PROJECT_DESCRIPTION.setStyle(sf::Text::Bold);
	PROJECT_DESCRIPTION.setPosition(12, (WINDOW_HEIGHT/2.67)+12);

    sf::RectangleShape RESULTS_MAIN_OUTLINE(sf::Vector2f(RESULT_BOX_Xi, RESULT_BOX_Yi));
    RESULTS_MAIN_OUTLINE.setFillColor(sf::Color::Transparent);
    RESULTS_MAIN_OUTLINE.setOutlineColor(sf::Color::White);
    RESULTS_MAIN_OUTLINE.setOutlineThickness(2);
    RESULTS_MAIN_OUTLINE.setPosition(RESULT_BOX_Xf, RESULT_BOX_Yf);

    sf::Text PRODUCTURL_PROMPT("Product URL: ", FONT, 36);
	PRODUCTURL_PROMPT.setFillColor(sf::Color::Cyan);
    PRODUCTURL_PROMPT.setStyle(sf::Text::Bold);
	PRODUCTURL_PROMPT.setPosition(RESULT_BOX_Xf, RESULT_BOX_Yf);

    sf::Text RESULT_1("", FONT, 24);
	RESULT_1.setFillColor(sf::Color::Cyan);
	RESULT_1.setPosition(RESULT_BOX_Xf+280, RESULT_BOX_Yf+12);

    sf::Text PRODUCT_NAME_PROMPT("Product Name: ", FONT, 36);
	PRODUCT_NAME_PROMPT.setFillColor(sf::Color::Cyan);
    PRODUCT_NAME_PROMPT.setStyle(sf::Text::Bold);
	PRODUCT_NAME_PROMPT.setPosition(RESULT_BOX_Xf, (RESULT_BOX_Yf+(RESULT_BOX_Yi/5)));

    sf::Text RESULT_2("", FONT, 24);
	RESULT_2.setFillColor(sf::Color::Cyan);
	RESULT_2.setPosition(RESULT_BOX_Xf+300, (RESULT_BOX_Yf+(RESULT_BOX_Yi/5))+12);

    sf::Text PRODUCT_PRICE_PROMPT("Product Price: ", FONT, 36);
	PRODUCT_PRICE_PROMPT.setFillColor(sf::Color::Cyan);
    PRODUCT_PRICE_PROMPT.setStyle(sf::Text::Bold);
	PRODUCT_PRICE_PROMPT.setPosition(RESULT_BOX_Xf, (RESULT_BOX_Yf+(2*(RESULT_BOX_Yi/5))));

    sf::Text RESULT_3("", FONT, 24);
	RESULT_3.setFillColor(sf::Color::Cyan);
	RESULT_3.setPosition(RESULT_BOX_Xf+325, (RESULT_BOX_Yf+(2*(RESULT_BOX_Yi/5)))+12);

    sf::Text PRODUCT_RATING_PROMPT("Product Rating: ", FONT, 36);
	PRODUCT_RATING_PROMPT.setFillColor(sf::Color::Cyan);
    PRODUCT_RATING_PROMPT.setStyle(sf::Text::Bold);
	PRODUCT_RATING_PROMPT.setPosition(RESULT_BOX_Xf, (RESULT_BOX_Yf+(3*(RESULT_BOX_Yi/5))));

    sf::Text RESULT_4("", FONT, 24);
	RESULT_4.setFillColor(sf::Color::Cyan);
	RESULT_4.setPosition(RESULT_BOX_Xf+345, (RESULT_BOX_Yf+(3*(RESULT_BOX_Yi/5)))+12);


    sf::RectangleShape PAGE_LEFT_BUTTON(sf::Vector2f(WINDOW_WIDTH/9.33, WINDOW_HEIGHT/12));
    PAGE_LEFT_BUTTON.setFillColor(sf::Color::Transparent);
    PAGE_LEFT_BUTTON.setOutlineColor(sf::Color::White);
    PAGE_LEFT_BUTTON.setOutlineThickness(2);
    PAGE_LEFT_BUTTON.setPosition((WINDOW_WIDTH/4.67)+6+(WINDOW_WIDTH/24), (WINDOW_HEIGHT/1.4)+(WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12)+8);
    
    sf::Text PAGE_LEFT_BUTTON_PROMPT("<", FONT, 32);
	PAGE_LEFT_BUTTON_PROMPT.setFillColor(sf::Color::Cyan);
    PAGE_LEFT_BUTTON_PROMPT.setStyle(sf::Text::Bold);
	PAGE_LEFT_BUTTON_PROMPT.setPosition((WINDOW_WIDTH/4.67)+66+(WINDOW_WIDTH/24), (WINDOW_HEIGHT/1.4)+(WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12)+16);

    sf::RectangleShape PAGE_RIGHT_BUTTON(sf::Vector2f(WINDOW_WIDTH/9.33, WINDOW_HEIGHT/12));
    PAGE_RIGHT_BUTTON.setFillColor(sf::Color::Transparent);
    PAGE_RIGHT_BUTTON.setOutlineColor(sf::Color::White);
    PAGE_RIGHT_BUTTON.setOutlineThickness(2);
    PAGE_RIGHT_BUTTON.setPosition((WINDOW_WIDTH/1.4)+(WINDOW_WIDTH/4.67)-(WINDOW_WIDTH/9.33)-(WINDOW_WIDTH/24)+6, (WINDOW_HEIGHT/1.4)+(WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12)+8);

    sf::Text PAGE_RIGHT_BUTTON_PROMPT(">", FONT, 32);
	PAGE_RIGHT_BUTTON_PROMPT.setFillColor(sf::Color::Cyan);
    PAGE_RIGHT_BUTTON_PROMPT.setStyle(sf::Text::Bold);
	PAGE_RIGHT_BUTTON_PROMPT.setPosition((WINDOW_WIDTH/1.4)+(WINDOW_WIDTH/4.67)-(WINDOW_WIDTH/9.33)-(WINDOW_WIDTH/24)+66, (WINDOW_HEIGHT/1.4)+(WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12)+16);

    sf::Text SEARCH_SELECTOR_TEXT("Select Search Mode", FONT, 25);
    SEARCH_SELECTOR_TEXT.setFillColor(sf::Color::Cyan);
    SEARCH_SELECTOR_TEXT.setStyle(sf::Text::Bold);
    SEARCH_SELECTOR_TEXT.setPosition(4, 4);

    sf::Text SEARCH_SELECTOR_HT_TEXT("Hash Table", FONT, 20);
    SEARCH_SELECTOR_HT_TEXT.setFillColor(sf::Color::Cyan);
    SEARCH_SELECTOR_HT_TEXT.setStyle(sf::Text::Bold);
    SEARCH_SELECTOR_HT_TEXT.setPosition(40, ((2+WINDOW_HEIGHT/2.67)/2)/2);

    sf::CircleShape HT_RADIO_BUTTON(8);
    HT_RADIO_BUTTON.setFillColor(sf::Color::Transparent);
    HT_RADIO_BUTTON.setOutlineColor(sf::Color::White);
    HT_RADIO_BUTTON.setOutlineThickness(2);
    HT_RADIO_BUTTON.setPosition(185, (((2+WINDOW_HEIGHT/2.67)/2)/2)+5);

    sf::CircleShape HT_RADIO_BUTTON_SELECTED(6);
    HT_RADIO_BUTTON_SELECTED.setFillColor(sf::Color::White);
    HT_RADIO_BUTTON_SELECTED.setPosition(187, (((2+WINDOW_HEIGHT/2.67)/2)/2)+7);

    sf::Text SEARCH_SELECTOR_TREE_TEXT("26ary Tree", FONT, 20);
    SEARCH_SELECTOR_TREE_TEXT.setFillColor(sf::Color::Cyan);
    SEARCH_SELECTOR_TREE_TEXT.setStyle(sf::Text::Bold);
    SEARCH_SELECTOR_TREE_TEXT.setPosition(40, ((2+WINDOW_HEIGHT/2.67)/2)/.75);

    sf::CircleShape TREE_RADIO_BUTTON(8);
    TREE_RADIO_BUTTON.setFillColor(sf::Color::Transparent);
    TREE_RADIO_BUTTON.setOutlineColor(sf::Color::White);
    TREE_RADIO_BUTTON.setOutlineThickness(2);
    TREE_RADIO_BUTTON.setPosition(185, (((2+WINDOW_HEIGHT/2.67)/2)/.75)+5);

    sf::CircleShape TREE_RADIO_BUTTON_SELECTED(6);
    TREE_RADIO_BUTTON_SELECTED.setFillColor(sf::Color::White);
    TREE_RADIO_BUTTON_SELECTED.setPosition(187, (((2+WINDOW_HEIGHT/2.67)/2)/.75)+7);

    sf::Text KEYWORD_TEXT("|", FONT, 36);
	KEYWORD_TEXT.setFillColor(sf::Color::Cyan);
	KEYWORD_TEXT.setPosition((WINDOW_WIDTH/4.67)+400, (((WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12))/2)-3);

    sf::Text TIME_TEXT("Total Time Taken:", FONT, 24);
	TIME_TEXT.setFillColor(sf::Color::Cyan);
    TIME_TEXT.setStyle(sf::Text::Bold);
	TIME_TEXT.setPosition((((((WINDOW_WIDTH/4.67))/2)+((WINDOW_WIDTH/1.4)))/2)+24, (WINDOW_HEIGHT/1.4)+(WINDOW_HEIGHT/20)+(WINDOW_HEIGHT/12)+16);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
            // Code taken from August Gould COP 3503 Project3
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace && displayKeyword.size() != 1) {
                    displayKeyword = displayKeyword.substr(0, displayKeyword.size() - 2);
                    displayKeyword += "|";
                    KEYWORD_TEXT.setString(displayKeyword);
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    window.close();
                }

            }
            if (event.type == sf::Event::TextEntered) {
                if ((event.text.unicode > 64 && event.text.unicode < 123 || event.text.unicode == 32 || event.text.unicode == 45 || event.text.unicode == 39)) {
                    displayKeyword = displayKeyword.substr(0, displayKeyword.size() - 1);
                    displayKeyword += static_cast<char>(event.text.unicode);
                    if (displayKeyword.size() == 0) {
                        break;
                    }

                    displayKeyword += "|";
                    KEYWORD_TEXT.setString(displayKeyword);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if ((event.mouseButton.x > 40 && event.mouseButton.x < 200) && (event.mouseButton.y > (((2+WINDOW_HEIGHT/2.67)/2)/2) && event.mouseButton.y < ((((2+WINDOW_HEIGHT/2.67)/2)/2)+30))) {
                    searchMode = false;
                } else if ((event.mouseButton.x > 40 && event.mouseButton.x < 200) && (event.mouseButton.y > (((2+WINDOW_HEIGHT/2.67)/2)/.75) && event.mouseButton.y < (((2+WINDOW_HEIGHT/2.67)/2)/.75)+30)) {
                    searchMode = true;
                } else if ((event.mouseButton.x >= GO_BOX.getPosition().x) && (event.mouseButton.x <= (GO_BOX.getPosition().x + GO_BOX.getSize().x))
                        && (event.mouseButton.y >= GO_BOX.getPosition().y) && (event.mouseButton.y <= (GO_BOX.getPosition().y + GO_BOX.getSize().y)))
                {
                    // std::chrono::time_point<std::chrono::high_resolution_clock> start;
                    // std::chrono::time_point<std::chrono::high_resolution_clock> end;
                    // std::chrono::microseconds duration;
                    keyword = displayKeyword.substr(0, displayKeyword.size() - 1);

                    if(searchMode){
                        auto start = std::chrono::high_resolution_clock::now();
                        std::cout << "TREE start: " << start.time_since_epoch().count() << std::endl;
                        results = productTrie.getKeyword(keyword);
                        auto end = std::chrono::high_resolution_clock::now();
                        std::cout << "TREE end:   " << end.time_since_epoch().count() << std::endl;
                        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                        if (results.size() > 1) {
                            drawLRButtons = true;
                        }
                    }
                    else{
                        auto start = std::chrono::high_resolution_clock::now();
                        std::cout << "start: " << start.time_since_epoch().count() << std::endl;
                        results = productHash.getKeyword(keyword);
                        auto end = std::chrono::high_resolution_clock::now();
                        std::cout << "end: " << end.time_since_epoch().count() << std::endl;
                        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                    }
                    std::cout << keyword << std::endl;
                    std::cout << "result size: " << results.size() << std::endl;
                    std::cout << keyword << ": " << results.at(0).name << std::endl;
                    
                    std::string tempString = "";
                    std::string printString = "";
                    tempString = results.at(currPage).productURL;
                    while (tempString.size() > 47) {
                        printString += tempString.substr(0, 47) + "\n";
                        tempString = tempString.substr(47);
                    }
                    printString += tempString;
                    RESULT_1.setString(printString);

                    printString = "";
                    tempString = results.at(currPage).name;
                    while (tempString.size() > 47) {
                        printString += tempString.substr(0, 47) + "\n";
                        tempString = tempString.substr(47);
                    }
                    printString += tempString;
                    RESULT_2.setString(printString);
                    
                    RESULT_3.setString("$" + results.at(currPage).price);
                    RESULT_4.setString(results.at(currPage).rating);

                    // duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                    tempString = ("Total Time Taken: ");
                    tempString += std::to_string(duration.count());
                    std::cout << duration.count() << std::endl;
                    TIME_TEXT.setString(tempString);
                } else if ((event.mouseButton.x >= PAGE_LEFT_BUTTON.getPosition().x) && (event.mouseButton.x <= (PAGE_LEFT_BUTTON.getPosition().x + PAGE_LEFT_BUTTON.getSize().x))
                        && (event.mouseButton.y >= PAGE_LEFT_BUTTON.getPosition().y) && (event.mouseButton.y <= (PAGE_LEFT_BUTTON.getPosition().y + PAGE_LEFT_BUTTON.getSize().y)))
                {
                    if (currPage == 0) {break;}
                    currPage--;
                    std::string tempString = "";
                    std::string printString = "";
                    tempString = results.at(currPage).productURL;
                    while (tempString.size() > 47) {
                        printString += tempString.substr(0, 47) + "\n";
                        tempString = tempString.substr(47);
                    }
                    printString += tempString;
                    RESULT_1.setString(printString);

                    printString = "";
                    tempString = results.at(currPage).name;
                    while (tempString.size() > 47) {
                        printString += tempString.substr(0, 47) + "\n";
                        tempString = tempString.substr(47);
                    }
                    printString += tempString;
                    RESULT_2.setString(printString);
                    
                    RESULT_3.setString("$" + results.at(currPage).price);
                    RESULT_4.setString(results.at(currPage).rating);
                } else if ((event.mouseButton.x >= PAGE_RIGHT_BUTTON.getPosition().x) && (event.mouseButton.x <= (PAGE_RIGHT_BUTTON.getPosition().x + PAGE_RIGHT_BUTTON.getSize().x))
                        && (event.mouseButton.y >= PAGE_RIGHT_BUTTON.getPosition().y) && (event.mouseButton.y <= (PAGE_RIGHT_BUTTON.getPosition().y + PAGE_RIGHT_BUTTON.getSize().y)))
                {
                    if (currPage == results.size()-1) {break;}
                    currPage++;
                    std::string tempString = "";
                    std::string printString = "";

                    tempString = results.at(currPage).productURL;
                    while (tempString.size() > 47) {
                        printString += tempString.substr(0, 47) + "\n";
                        tempString = tempString.substr(47);
                    }
                    printString += tempString;
                    RESULT_1.setString(printString);

                    printString = "";
                    tempString = results.at(currPage).name;
                    while (tempString.size() > 47) {
                        printString += tempString.substr(0, 47) + "\n";
                        tempString = tempString.substr(47);
                    }
                    printString += tempString;
                    RESULT_2.setString(printString);
                    
                    RESULT_3.setString("$" + results.at(currPage).price);
                    RESULT_4.setString(results.at(currPage).rating);
                }
            }
        }
        window.clear();
        window.draw(SEARCH_MODE_SELECT_BOX);
        window.draw(SEARCH_BOX);
        window.draw(GO_BOX);
        window.draw(RESULTS_MAIN_OUTLINE);
        window.draw(KEYWORD_TEXT);
        window.draw(SEARCH_SELECTOR_TEXT);
        window.draw(SEARCH_SELECTOR_HT_TEXT);
        window.draw(SEARCH_SELECTOR_TREE_TEXT);
        window.draw(HT_RADIO_BUTTON);
        window.draw(TREE_RADIO_BUTTON);
        if (searchMode) {
            window.draw(TREE_RADIO_BUTTON_SELECTED);
        } else {
            window.draw(HT_RADIO_BUTTON_SELECTED);
        }
        if (drawLRButtons) {
            window.draw(PAGE_LEFT_BUTTON);
            window.draw(PAGE_LEFT_BUTTON_PROMPT);
            window.draw(PAGE_RIGHT_BUTTON);
            window.draw(PAGE_RIGHT_BUTTON_PROMPT);
        }
        window.draw(SEARCH_BOX_PROMPT);
        window.draw(GO_BOX_PROMPT);
        window.draw(PROJECT_DESCRIPTION);
        window.draw(TIME_TEXT);
        window.draw(RESULT_1);
        window.draw(PRODUCTURL_PROMPT);
        window.draw(PRODUCT_NAME_PROMPT);
        window.draw(RESULT_2);
        window.draw(PRODUCT_PRICE_PROMPT);
        window.draw(RESULT_3);
        window.draw(PRODUCT_RATING_PROMPT);
        window.draw(RESULT_4);
        window.display();
    }
    return 0;
}

/*
-copy this code into event to switch between search modes

searchMode = !searchMode;

-copy this code to load in vector of products based on keyword

if(searchMode){
    results = productTrie.getKeyword(keyword);
}
else{
    results = productHash.getKeyword(keyword);
}
*/